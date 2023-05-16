#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "CSVReader.h"
#include "PlaybackCamera.h"
#include "PlaybackWidget.generated.h"

UCLASS()
class CSVDATAVISUALIZER_API UPlaybackWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual bool Initialize() override;

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UComboBoxString* ComboBox_CSVFiles;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class USlider* PlaybackSlider;


private:    
    UFUNCTION(BlueprintCallable)
    void OnPlayButtonClicked();

    UFUNCTION(BlueprintCallable)
    void OnPauseButtonClicked();

    UFUNCTION(BlueprintCallable)
    void OnRestartButtonClicked();

    UFUNCTION(BlueprintCallable)
    void OnSliderValueChanged(float Value);

    UFUNCTION(BlueprintCallable)
    void HandleOnComboBoxOpening();

    UFUNCTION(BlueprintCallable)
    void OnCSVFileSelected(FString SelectedItem, ESelectInfo::Type SelectionType);

    UFUNCTION(BlueprintCallable)
    void InitCSV(FString FilePath);

    UPROPERTY()
    TArray<FTransformData> CSVData;

    UPROPERTY()
    class AActor* AnimatedObject;

    UPROPERTY()
        class APlaybackCamera* PlaybackCamera;

};
