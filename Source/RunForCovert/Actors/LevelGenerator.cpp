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
    MaxFragments = 5;
    MaxAttemptsPerFragment = 10;
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


    GetWorldTimerManager().SetTimer(TimerHandle, this, &ALevelGenerator::TrySpawnFragment, .1f, true);
}

void ALevelGenerator::TrySpawnFragment()
{
    if ((OpenAttachmentPoints.Num() > 0 || CurrentAttachmentPoint))
    {
        UE_LOG(LogTemp, Warning, TEXT("Open attachments: %i"), OpenAttachmentPoints.Num())
        AMapFragment* MapFragment = LoadRandomLevel();

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
            CurrentAttachmentPoint = OpenAttachmentPoints.Num() > 0 ? OpenAttachmentPoints.Pop() : nullptr;
        }
        else
        {
            CurrentAttachmentPoint = OpenAttachmentPoints.Num() > 0 ? OpenAttachmentPoints.Pop() : nullptr;
            UE_LOG(LogTemp, Error, TEXT("Found no valid placements for map fragment"))
        }

        Attempts++;

        if (ActiveFragments >= MaxFragments || Attempts >= MaxAttemptsPerFragment * MaxFragments)
        {
            GetWorldTimerManager().ClearTimer(TimerHandle);
        }
    }
}

AMapAttachmentPoint* ALevelGenerator::TryPlaceFragment(AMapFragment* MapFragment, AMapAttachmentPoint* CurrentAttachmentPoint)
{
    TArray<AMapAttachmentPoint*> AttachmentPoints = MapFragment->GetAttachmentPoints();
    if (AttachmentPoints.Num() == 0) { return nullptr; }
    UE_LOG(LogTemp, Warning, TEXT("Found %i attachment points"), AttachmentPoints.Num())

    for (AMapAttachmentPoint* NewAttachmentPoint : MapFragment->GetAttachmentPoints())
    {
        if (!TryAttachPoint(MapFragment, NewAttachmentPoint, CurrentAttachmentPoint)) { continue; }

        TArray<AActor*> OverlappingActors;
        MapFragment->GetOverlappingActors(OUT OverlappingActors);

        if (OverlappingActors.Num() == 0)
        {
            return NewAttachmentPoint;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Found overlapping actors"))
        }
    }
    UE_LOG(LogTemp, Error, TEXT("Could not place fragment"))
    MapFragment->Destroy();
    return nullptr;
}

bool ALevelGenerator::TryAttachPoint(AMapFragment* MapFragment, AMapAttachmentPoint* NewAttachmentPoint,
                                     AMapAttachmentPoint* CurrentAttachmentPoint)
{
    UE_LOG(LogTemp, Warning, TEXT("Yaw Exist: %f"), CurrentAttachmentPoint->GetActorRotation().Yaw)
    UE_LOG(LogTemp, Warning, TEXT("Yaw New: %f"), NewAttachmentPoint->GetActorRotation().Yaw)
    float NewYaw = 180.f + (CurrentAttachmentPoint->GetActorRotation().Yaw - NewAttachmentPoint->GetActorRotation().Yaw);
    NewYaw = NewYaw > 180 ? NewYaw-360 : NewYaw;

    UE_LOG(LogTemp, Warning, TEXT("Calculated Rotation: %f"), NewYaw)

    MapFragment->SetActorRotation(FRotator::ZeroRotator);
    MapFragment->SetActorRotation(FRotator(0.f, NewYaw, 0.f));
    MapFragment->SetActorLocation(
            CurrentAttachmentPoint->GetActorLocation() +
            (MapFragment->GetActorLocation() - NewAttachmentPoint->GetActorLocation()));

    UE_LOG(LogTemp, Warning, TEXT("Yaw Final: %f"), NewYaw)

    return MapFragment->AttachmentPointsClear(NewAttachmentPoint);
}

AMapFragment* ALevelGenerator::LoadRandomLevel()
{
    check(MapFragments.Num() > 0)
    return GetWorld()->SpawnActor<AMapFragment>(MapFragments[FMath::RandRange(0, MapFragments.Num()-1)]);
}