// Caps Collective 2020


#include "LevelGenerator.h"
#include "MapHandle.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "MapAttachmentPoint.h"
#include "Runtime/Engine/Classes/Engine/LevelStreaming.h"

ALevelGenerator::ALevelGenerator()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ALevelGenerator::BeginPlay()
{
    UE_LOG(LogTemp, Warning, TEXT("Level gen online"))

    // Add all open attachment points
    for (AMapAttachmentPoint* AttachmentPoint : TActorRange<AMapAttachmentPoint>(GetWorld()))
    {
        OpenAttachmentPoints.Add(AttachmentPoint);
    }

    while (OpenAttachmentPoints.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Open attachments: %i"), OpenAttachmentPoints.Num())
        AMapHandle* MapHandle = LoadRandomLevel();

        TArray<AMapAttachmentPoint*> AttachmentPoints = MapHandle->GetAttachmentPoints();
        if (AttachmentPoints.Num() == 0) { return; }

        UE_LOG(LogTemp, Warning, TEXT("Found attachment points"))
        if (OpenAttachmentPoints.Num() > 0)
        {
            AMapAttachmentPoint* NewAttachmentPoint = AttachmentPoints.Pop();
            AMapAttachmentPoint* ExistingAttachmentPoint = OpenAttachmentPoints.Pop();

            UE_LOG(LogTemp, Warning, TEXT("Rotating map"))

            FRotator NewRotation;

            UE_LOG(LogTemp, Warning, TEXT("Rotation Exist: %s"), *ExistingAttachmentPoint->GetActorRotation().ToString())
            UE_LOG(LogTemp, Warning, TEXT("Rotation New: %s"), *NewAttachmentPoint->GetActorRotation().ToString())

            NewRotation.Yaw = 180.f + (ExistingAttachmentPoint->GetActorRotation().Yaw + NewAttachmentPoint->GetActorRotation().Yaw);

            UE_LOG(LogTemp, Warning, TEXT("Rotation Final: %s"), *NewRotation.ToString())

            MapHandle->SetActorRotation(NewRotation);
            MapHandle->SetActorLocation(
                    ExistingAttachmentPoint->GetActorLocation() +
                    (MapHandle->GetActorLocation() - NewAttachmentPoint->GetActorLocation()));
        }
        OpenAttachmentPoints.Append(AttachmentPoints);
    }
}

AMapHandle* ALevelGenerator::LoadRandomLevel()
{
    check(MapHandleBlueprints.Num() > 0)
    return GetWorld()->SpawnActor<AMapHandle>(MapHandleBlueprints[FMath::RandRange(0, MapHandleBlueprints.Num()-1)]);
}