// Caps Collective 2020


#include "LevelGenerator.h"
#include "MapFragment.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "MapAttachmentPoint.h"
#include "TimerManager.h"

ALevelGenerator::ALevelGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    ActiveFragments = 0;
    TargetFragments = 5;
    SlowGenerationRate = .1f;
    bSlowGeneration = false;
    CurrentAttachmentPoint = nullptr;
}

void ALevelGenerator::BeginPlay()
{
    // Add all currently open attachment points
    for (AMapAttachmentPoint* AttachmentPoint : TActorRange<AMapAttachmentPoint>(GetWorld()))
    {
        OpenAttachmentPoints.Add(AttachmentPoint);
    }

    // Set the current attachment point from open
    if (OpenAttachmentPoints.Num() == 0) { return; }
    CurrentAttachmentPoint = OpenAttachmentPoints.Pop();

    // Run the fragment spawning process
    if (bSlowGeneration)
    {
        // Run the process on a timer:
        // "Yes, I know what you're thinking but it's essentially a cast to a void member function reference.
        // I will be damned if I ever have to forward a function out of just returning a bool!"
        GetWorldTimerManager().SetTimer(TimerHandle, this,
                                        (void(ALevelGenerator::*)())&ALevelGenerator::TrySpawnFragment,
                                        SlowGenerationRate, true);

    }
    else
    {
        // Run the process on a loop
        while (TrySpawnFragment());
    }
}

bool ALevelGenerator::TrySpawnFragment()
{
    UE_LOG(LogTemp, Warning, TEXT("Open attachments: %i"), OpenAttachmentPoints.Num())

    // Switch fragment list based on target number of fragments
    TArray<TSubclassOf<AMapFragment>>* FragmentList =
            (ActiveFragments + OpenAttachmentPoints.Num() < TargetFragments) ? &OpeningFragments : &ClosingFragments;

    // Try placing for each fragment in the list in a randomised order
    for (int32 Index : GetRandomisedIndices(FragmentList->Num()))
    {
        // Spawn the map fragment
        AMapFragment* MapFragment = GetWorld()->SpawnActor<AMapFragment>((*FragmentList)[Index]);

        UE_LOG(LogTemp, Warning, TEXT("Placing fragment"))

        // Attempt to place the fragment for each of its orientations
        AMapAttachmentPoint* AttachedPoint = TryPlaceFragment(MapFragment, CurrentAttachmentPoint);
        if (AttachedPoint)
        {
            // Add all new, open attachment points
            TArray<AMapAttachmentPoint*> NewAttachmentPoints = MapFragment->GetAttachmentPoints();
            NewAttachmentPoints.Remove(AttachedPoint);
            OpenAttachmentPoints.Append(NewAttachmentPoints);

            // Increment active fragments end the loop
            ActiveFragments++;
            break;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Found no valid placements for map fragment"))

            // Destroy the invalid fragment
            MapFragment->Destroy();

            // TODO fall back on closing fragment
        }
    }

    // Get the next attachment point
    CurrentAttachmentPoint = OpenAttachmentPoints.Num() > 0 ? OpenAttachmentPoints.Pop() : nullptr;

    // End the fragment spawning process once there are no more open attachment points
    if (OpenAttachmentPoints.Num() <= 0 && !CurrentAttachmentPoint) {
        UE_LOG(LogTemp, Warning, TEXT("Final fragment count: %i"), ActiveFragments)

        // End the timer, and notify the completion delegate
        if (bSlowGeneration)
        {
            GetWorldTimerManager().ClearTimer(TimerHandle);
        }
        OnGenerationComplete.Broadcast();
        return false;
    }
    return true;
}

AMapAttachmentPoint* ALevelGenerator::TryPlaceFragment(AMapFragment* MapFragment, AMapAttachmentPoint* CurrentAttachmentPoint)
{
    // Try attaching at each point on the map fragment
    for (AMapAttachmentPoint* NewAttachmentPoint : MapFragment->GetAttachmentPoints())
    {
        // Try attach at the current attachment point and check for overlaps, else reset and continue
        if (TryAttachPoint(MapFragment, NewAttachmentPoint, CurrentAttachmentPoint) && MapFragment->HasNoOverlaps())
        {
            return NewAttachmentPoint;
        }
        MapFragment->ResetLocationRotation();
    }
    return nullptr;
}

bool ALevelGenerator::TryAttachPoint(AMapFragment* MapFragment, AMapAttachmentPoint* NewAttachmentPoint,
                                     AMapAttachmentPoint* CurrentAttachmentPoint)
{
    // Calculate the rotational difference "delta" between the two attachment points
    float NewYaw = 180.f + (CurrentAttachmentPoint->GetActorRotation().Yaw - NewAttachmentPoint->GetActorRotation().Yaw);
    NewYaw = NewYaw > 180 ? NewYaw - 360 : NewYaw;

    // Place the fragment as rotated by the attachment points
    MapFragment->SetActorRotation(FRotator(0.f, NewYaw, 0.f));
    MapFragment->SetActorLocation(CurrentAttachmentPoint->GetActorLocation() +
            (MapFragment->GetActorLocation() - NewAttachmentPoint->GetActorLocation()));

    // Check if the attachment points have a clear space to place children
    return MapFragment->AttachmentPointsClear(NewAttachmentPoint);
}

TArray<int32> ALevelGenerator::GetRandomisedIndices(int32 ArrayLength)
{
    // Generate a list of indices to the array length
    TArray<int32> Indices;
    for (int32 I = 0; I < ArrayLength; I++)
    {
        Indices.Add(I);
    }

    // Shuffle the list of indices
    Indices.Sort([](int32 A, int32 B) {
        return FMath::FRand() < 0.5f;
    });
    return Indices;
}

bool ALevelGenerator::IsGenerationComplete() const
{
    return OpenAttachmentPoints.Num() <= 0;
}