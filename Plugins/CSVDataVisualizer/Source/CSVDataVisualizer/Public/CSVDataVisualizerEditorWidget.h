#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class CSVDATAVISUALIZER_API SCsvDataVisualizerEditorWidget : public SCompoundWidget,
    public TSharedFromThis<SCsvDataVisualizerEditorWidget> {
  public:
    SLATE_BEGIN_ARGS ( SCsvDataVisualizerEditorWidget ) {}
    SLATE_END_ARGS()

    void Construct ( const FArguments& InArgs );

  private:
    TSharedPtr<class SComboBox<TSharedPtr<FString>>> ActorDropdown;
    TSharedPtr<class SButton> PlayButton;
    TSharedPtr<class SSlider> PlaybackSlider;

    TArray<TSharedPtr<FString>> AvailableActors;
    void UpdateAvailableActorsList();

    TSharedPtr<class SVerticalBox> CsvList;
    TMap<TSharedPtr<FString>, class UCSVData*> CsvDataMap;
    void AddCsvData ( const FString& FilePath, AActor* Actor );

    float PlaybackProgress;

    // Add these private members
    float PlaybackSpeed;
    bool bIsPlaying;
    FDelegateHandle TickDelegateHandle;
    bool OnEditorTickHandler ( float DeltaTime );

    // Add this function
    FReply TogglePlayback();
};
