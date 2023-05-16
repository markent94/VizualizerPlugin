#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "PlaybackWidget.h"
#include "UIManager.generated.h"

UCLASS()
class CSVDATAVISUALIZER_API AUIManager : public AActor
{
    GENERATED_BODY()

public:
    AUIManager();

    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    TSubclassOf<UUserWidget> PlaybackWidgetClass;

    UPROPERTY()
    UUserWidget* PlaybackWidget; // The instance of PlaybackWidgetClass

private:

};
