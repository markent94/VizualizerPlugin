#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaybackCamera.generated.h"

UCLASS()
class CSVDATAVISUALIZER_API APlaybackCamera : public AActor
{
    GENERATED_BODY()

public:
    APlaybackCamera();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
        class AActor* TargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
        class UCameraComponent* CameraComponent;
};
