#include "PlaybackCamera.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"

APlaybackCamera::APlaybackCamera()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    RootComponent = CameraComponent;
}

void APlaybackCamera::BeginPlay()
{
    Super::BeginPlay();
}

void APlaybackCamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TargetActor)
    {
        FVector Direction = TargetActor->GetActorLocation() - GetActorLocation();
        FRotator NewRotation = Direction.Rotation();
        SetActorRotation(NewRotation);
    }
}
