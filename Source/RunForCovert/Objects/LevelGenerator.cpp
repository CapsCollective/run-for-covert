// Caps Collective 2020


#include "LevelGenerator.h"
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

    FName LevelToLoad = FName(TEXT("SimpleRoom_Streaming"));
    FLatentActionInfo LatentInfo;
    UGameplayStatics::LoadStreamLevel(World, LevelToLoad,
                                      true,true, LatentInfo);

    for (AMapAttachmentPoint* AttachmentPoint : TActorRange<AMapAttachmentPoint>(World))
    {
        AttachmentPoints.Add(AttachmentPoint);
    }
}

void ULevelGenerator::RegisterAttachmentPoint(AMapAttachmentPoint* AttachmentPoint)
{
    UE_LOG(LogTemp, Warning, TEXT("Attachment point: %s"), *AttachmentPoint->GetName())
    if (AttachmentPoints.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("More than 0"))
        AttachmentPoint->SetActorLocation(AttachmentPoints.Last()->GetActorLocation());
    }
    AttachmentPoints.Add(AttachmentPoint);

    UE_LOG(LogTemp, Warning, TEXT("Found actors: %i"), AttachmentPoints.Num())
}