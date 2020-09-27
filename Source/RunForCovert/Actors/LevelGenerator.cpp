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
    bCompletedGeneration = false;
    CurrentAttachmentPoint = nullptr;
}

void ALevelGenerator::BeginPlay()
{
    // Add all currently open attachment points
    for (AMapAttachmentPoint* AttachmentPoint : TActorRange<AMapAttachmentPoint>(GetWorld()))
    {
        OpenAttachmentPoints.Add(AttachmentPoint);
    }

    // Do not attempt generation if any necessary lists are empty
    if (OpenAttachmentPoints.Num() == 0 || OpeningFragments.Num() == 0 || ClosingFragments.Num() == 0) { return; }

    // Set the current attachment point from open and run the fragment spawning process
    CurrentAttachmentPoint = OpenAttachmentPoints.Pop();
    if (bSlowGeneration)
    {
        // Run the process on a timer:
        // "Yes, I know what you're thinking but it's essentially a cast to a void member function reference.
        // I will be damned if I ever have to forward a function out of just returning a bool!"
        GetWorldTimerManager().SetTimer(TimerHandle, this,
                                        (void(ALevelGenerator::*)())&ALevelGenerator::RunFragmentSpawn,
                                        SlowGenerationRate, true);
    }
    else
    {
        // Run the process on a loop
        while (RunFragmentSpawn());
    }
}

bool ALevelGenerator::RunFragmentSpawn()
{
    UE_LOG(LogTemp, Warning, TEXT("Open attachments: %i"), OpenAttachmentPoints.Num())

    // Switch fragment list based on target number of fragments
    TArray<TSubclassOf<AMapFragment>>* FragmentList =
            (ActiveFragments + OpenAttachmentPoints.Num() < TargetFragments) ? &OpeningFragments : &ClosingFragments;

    // Try placing for each fragment in the list in a randomised order
    bool bFoundAttachment = false;
    for (int32 Index : GetRandomisedIndices(FragmentList->Num()))
    {
        if (TryAddFragment(FragmentList, Index))
        {
            bFoundAttachment = true;
            break;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Found no valid placements for map fragment"))
        }
    }

    // If no valid attachments are found, fallback on adding a closing fragment
    if (!bFoundAttachment && !TryAddFragment(&ClosingFragments, FMath::RandRange(0, ClosingFragments.Num()-1)))
    {
        UE_LOG(LogTemp, Error, TEXT("Found no valid placements for ANY map fragments"))
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
        bCompletedGeneration = true;
        return false;
    }
    return true;
}

bool ALevelGenerator::TryAddFragment(TArray<TSubclassOf<AMapFragment>>* FragmentList, int32 Index)
{
    // Spawn the map fragment and attempt to place it for each of its orientations
    AMapFragment* MapFragment = GetWorld()->SpawnActor<AMapFragment>((*FragmentList)[Index]);
    AMapAttachmentPoint* AttachedPoint = TryPlaceFragment(MapFragment, CurrentAttachmentPoint);
    if (AttachedPoint)
    {
        // Add all new, open attachment points
        TArray<AMapAttachmentPoint*> NewAttachmentPoints = MapFragment->GetAttachmentPoints();
        NewAttachmentPoints.Remove(AttachedPoint);
        OpenAttachmentPoints.Append(NewAttachmentPoints);

        // Increment active fragments
        ActiveFragments++;
        return true;
    }
    else
    {
        // Destroy the invalid fragment
        MapFragment->Destroy();
        return false;
    }
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
    return bCompletedGeneration;
}