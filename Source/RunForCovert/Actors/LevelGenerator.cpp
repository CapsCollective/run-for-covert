// Caps Collective 2020


#include "LevelGenerator.h"
#include "../Actors/MapHandle.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "../Actors/MapAttachmentPoint.h"
#include "Runtime/Engine/Classes/Engine/LevelStreaming.h"

ULevelGenerator::ULevelGenerator()
{
    // Set field default values
    World = nullptr;
}

void ULevelGenerator::Initialise(UWorld* InWorld)
{
    World = InWorld;
    UE_LOG(LogTemp, Warning, TEXT("Level gen online"))

    for (AMapAttachmentPoint* AttachmentPoint : TActorRange<AMapAttachmentPoint>(World))
    {
        OpenAttachmentPoints.Add(AttachmentPoint);
    }

    LoadRandomLevel();
}

void ULevelGenerator::RegisterMap(AMapHandle* MapHandle)
{
    UE_LOG(LogTemp, Warning, TEXT("Registering map"))
    TArray<AMapAttachmentPoint*> AttachmentPoints = MapHandle->GetAttachmentPoints();
    if (AttachmentPoints.Num() == 0) { return; }

    UE_LOG(LogTemp, Warning, TEXT("Found attachment points"))

    UE_LOG(LogTemp, Warning, TEXT("Open attachments: %i"), OpenAttachmentPoints.Num())

    if (OpenAttachmentPoints.Num() > 0)
    {
        LoadRandomLevel();
        if (OpenAttachmentPoints.Num() > 0)
        {
            AMapAttachmentPoint* NewAttachmentPoint = AttachmentPoints.Pop();
            AMapAttachmentPoint* ExistingAttachmentPoint = OpenAttachmentPoints.Pop();

            UE_LOG(LogTemp, Warning, TEXT("Rotating map"))

            MapHandle->SetActorRotation(
                    FRotator(0.f, 180.f, 0.f) -
                    NewAttachmentPoint->GetActorRotation() - ExistingAttachmentPoint->GetActorRotation());
            MapHandle->SetActorLocation(
                    ExistingAttachmentPoint->GetActorLocation() +
                    (MapHandle->GetActorLocation() - NewAttachmentPoint->GetActorLocation()));
        }
        OpenAttachmentPoints.Append(AttachmentPoints);
    }
}

void ULevelGenerator::LoadRandomLevel()
{
    FLatentActionInfo LatentInfo;
    ULevelStreaming* StreamingLevel =
            World->GetStreamingLevels()[FMath::RandRange(0, World->GetStreamingLevels().Num()-1)];
    UGameplayStatics::LoadStreamLevel(World, StreamingLevel->GetWorldAssetPackageFName(),
                                      true, true, LatentInfo);
}