// Caps Collective 2020


#include "LevelGenerator.h"
#include "MapFragment.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "MapAttachmentPoint.h"
#include "Runtime/Engine/Classes/Engine/LevelStreaming.h"
#include "TimerManager.h"

ALevelGenerator::ALevelGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    // Set field default values
    bGenerationComplete = false;
    Attempts = 0;
    ActiveFragments = 0;
    MaxFragments = 5;
    MaxAttemptsPerFragment = 10;
    bSlowGeneration = false;
    SlowGenerationRate = .1f;
    CurrentAttachmentPoint = nullptr;
}

void ALevelGenerator::BeginPlay()
{
    UE_LOG(LogTemp, Warning, TEXT("Level gen online"))

    // Add all open attachment points
    for (AMapAttachmentPoint* AttachmentPoint : TActorRange<AMapAttachmentPoint>(GetWorld()))
    {
        OpenAttachmentPoints.Add(AttachmentPoint);
    }

    if (OpenAttachmentPoints.Num() == 0) { return; }
    CurrentAttachmentPoint = OpenAttachmentPoints.Pop();

    if (bSlowGeneration)
    {
        // Yes, I know what you're thinking but it's essentially a cast to a void member function reference.
        // I will be damned if I ever have to forward a function out of just returning a bool!
        GetWorldTimerManager().SetTimer(TimerHandle, this,
                                        (void(ALevelGenerator::*)())&ALevelGenerator::TrySpawnFragment,
                                        SlowGenerationRate, true);
    }
    else
    {
        while (TrySpawnFragment());
        bGenerationComplete = true;
        OnGenerationComplete.Broadcast();
    }
}

bool ALevelGenerator::TrySpawnFragment()
{
    UE_LOG(LogTemp, Warning, TEXT("Open attachments: %i"), OpenAttachmentPoints.Num())
    for (int32 Index : GetRandomisedIndices(MapFragments.Num()))
    {
        AMapFragment* MapFragment = GetWorld()->SpawnActor<AMapFragment>(MapFragments[Index]);

        UE_LOG(LogTemp, Warning, TEXT("Placing fragment"))
        AMapAttachmentPoint* AttachedPoint = TryPlaceFragment(MapFragment, CurrentAttachmentPoint);
        if (AttachedPoint)
        {
            // Add all new, open attachment points
            TArray<AMapAttachmentPoint*> NewAttachmentPoints = MapFragment->GetAttachmentPoints();
            NewAttachmentPoints.Remove(AttachedPoint);
            OpenAttachmentPoints.Append(NewAttachmentPoints);

            // Increment the active fragments and move to the next attachment point
            ActiveFragments++;
            break;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Found no valid placements for map fragment"))
            MapFragment->Destroy();
        }
    }

    CurrentAttachmentPoint = OpenAttachmentPoints.Num() > 0 ? OpenAttachmentPoints.Pop() : nullptr;

    if (OpenAttachmentPoints.Num() <= 0 && !CurrentAttachmentPoint) {
        if (bSlowGeneration)
        {
            GetWorldTimerManager().ClearTimer(TimerHandle);
        }
        bGenerationComplete = true;
        OnGenerationComplete.Broadcast();
        return false;
    }
    return true;
}

AMapAttachmentPoint* ALevelGenerator::TryPlaceFragment(AMapFragment* MapFragment, AMapAttachmentPoint* CurrentAttachmentPoint)
{
    TArray<AMapAttachmentPoint*> AttachmentPoints = MapFragment->GetAttachmentPoints();
    if (AttachmentPoints.Num() == 0) { return nullptr; }

    for (AMapAttachmentPoint* NewAttachmentPoint : MapFragment->GetAttachmentPoints())
    {
        if (!TryAttachPoint(MapFragment, NewAttachmentPoint, CurrentAttachmentPoint))
        {
            // Reset the fragment position and continue
            MapFragment->SetActorRotation(FRotator::ZeroRotator);
            MapFragment->SetActorLocation(FVector::ZeroVector);
            continue;
        }

        TArray<AActor*> OverlappingActors;
        MapFragment->GetOverlappingActors(OUT OverlappingActors);

        if (OverlappingActors.Num() == 0)
        {
            return NewAttachmentPoint;
        }
    }
    return nullptr;
}

bool ALevelGenerator::TryAttachPoint(AMapFragment* MapFragment, AMapAttachmentPoint* NewAttachmentPoint,
                                     AMapAttachmentPoint* CurrentAttachmentPoint)
{
    // Calculate the rotational difference between the two attachment points
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
    TArray<int32> Indices;
    for (int32 I = 0; I < ArrayLength; I++)
    {
        Indices.Add(I);
    }

    Indices.Sort([](int32 A, int32 B) {
        return FMath::FRand() < 0.5f;
    });

    return Indices;
}

bool ALevelGenerator::IsGenerationComplete() const
{
    return bGenerationComplete;
}