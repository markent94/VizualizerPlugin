#include "UIManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AUIManager::AUIManager()
{
    PrimaryActorTick.bCanEverTick = false;

    //static const FString WidgetPath = TEXT("/Game/CSVDataVisualizer/UI_Widgets/PlaybackWidgetBP.PlaybackWidgetBP_C");
    //UClass* WidgetClass = LoadObject<UClass>(nullptr, *WidgetPath);

    //if (WidgetClass != nullptr)
    //{
    //    PlaybackWidgetClass = WidgetClass;
    //}
}

void AUIManager::BeginPlay()
{
    Super::BeginPlay();

    if (PlaybackWidgetClass)
    {
        PlaybackWidget = CreateWidget<UUserWidget>(GetWorld(), PlaybackWidgetClass);
        if (PlaybackWidget)
        {
            PlaybackWidget->AddToViewport();
        }
    }
}
