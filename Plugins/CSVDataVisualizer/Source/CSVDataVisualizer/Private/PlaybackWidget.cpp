#include "EngineUtils.h"
#include "HAL/FileManager.h"
#include "PlaybackWidget.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

bool UPlaybackWidget::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) return false;

    UWorld* World = GetWorld(); // get a reference to the world
    if (World) // make sure the pointer is valid
    {
        // Define spawn parameters
        FActorSpawnParameters SpawnParams;

        // Define the spawn location and rotation
        FVector SpawnLocation = FVector(0.0f, 0.0f, 0.0f);
        FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

        // Spawn the camera
        PlaybackCamera = World->SpawnActor<APlaybackCamera>(APlaybackCamera::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);

        // Change the active camera
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
        if (PlayerController)
        {
            PlayerController->SetViewTargetWithBlend(PlaybackCamera, 0.5f);
        }
    }

    return true;
}

void UPlaybackWidget::OnCSVFileSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Type::OnMouseClick || SelectionType == ESelectInfo::Type::OnKeyPress)
    {
        // Call InitCSV with the selected item
        InitCSV(SelectedItem);
    }
}

void UPlaybackWidget::InitCSV(FString FilePath)
{
    CSVData = UCSVReader::ReadCSV(+"/CSVs/" + FilePath);

    if (PlaybackSlider)
    {
        // Set the slider range
        PlaybackSlider->SetMinValue(0);
        PlaybackSlider->SetMaxValue(CSVData.Num() - 1);
    }

    for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        // Check the actor's name, replace "YourActorName" with the name of the actor you're looking for
        FString name = ActorItr->GetActorLabel();
        UE_LOG(LogTemp, Warning, TEXT("%s"), *name);
        if (name.Equals("AnimatedObject"))
        {
            // Cast the actor to UStaticMeshComponent and assign it to AnimatedObject
            AnimatedObject = *ActorItr;
            break;
        }

        // Set paused flag
        bIsPaused = true;
    }

    const FTransformData& Data = CSVData[0];

    // Set the transform of the animated object
    AnimatedObject->SetActorLocation(Data.Position);
    AnimatedObject->SetActorRotation(Data.Rotation);

    // Get the bounds of the target actor
    FBox Bounds = AnimatedObject->GetComponentsBoundingBox();

    // Compute a suitable position for the camera: 10 times the diagonal length of the bounding box away from the target, in the direction of the camera's forward vector
    float Distance = 10.0f * Bounds.GetExtent().Size();

    // Set the transform of the animated object
    PlaybackCamera->SetActorLocation(Data.Position + (Distance * FVector(1, 1, 0)));

    // Set the target for the camera
    PlaybackCamera->TargetActor = AnimatedObject;
}

void UPlaybackWidget::OnPauseButtonClicked()
{
    // Set paused flag
    bIsPaused = true;
}

void UPlaybackWidget::OnRestartButtonClicked()
{
    // Reset slider position
    if (PlaybackSlider)
    {
        PlaybackSlider->SetValue(0.f);
    }

    // Set paused flag
    bIsPaused = true;
}

void UPlaybackWidget::OnPlayButtonClicked()
{
    // Remove paused flag
    bIsPaused = false;
}

void UPlaybackWidget::OnSliderValueChanged(float Value)
{
    // Use the new slider value as the index to access the corresponding FTransformData
    int32 Index = FMath::RoundToInt(Value);

    // Make sure the index is valid
    if (CSVData.IsValidIndex(Index))
    {
        const FTransformData& Data = CSVData[Index];

        // Set the transform of the animated object
        AnimatedObject->SetActorLocation(Data.Position);
        AnimatedObject->SetActorRotation(Data.Rotation);
    }
}

void UPlaybackWidget::HandleOnComboBoxOpening()
{
    if (ComboBox_CSVFiles)
    {
        // Clear all options
        ComboBox_CSVFiles->ClearOptions();

        // Populate the ComboBox with the names of the CSV files
        IFileManager& FileManager = IFileManager::Get();
        FString FolderPath = FPaths::ProjectDir() + "/CSVs";  // replace with your folder path

        TArray<FString> Filenames;
        FileManager.FindFiles(Filenames, *FolderPath, TEXT("csv"));  // only find csv files

        for (FString& Filename : Filenames)
        {
            ComboBox_CSVFiles->AddOption(Filename);
        }
    }
}

void UPlaybackWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!bIsPaused && PlaybackSlider && AnimatedObject)
    {
        // Get slider value (assumed to be between 0 and CSVData.Num() - 1)
        int32 SliderValue = FMath::RoundToInt(PlaybackSlider->GetValue());

        // Make sure the index is valid
        if (CSVData.IsValidIndex(SliderValue))
        {
            // Get the transform data from the CSVData array
            FTransformData TransformData = CSVData[SliderValue];

            // Apply the transform to the AnimatedObject
            AnimatedObject->SetActorLocationAndRotation(TransformData.Position, TransformData.Rotation);
        }

        PlaybackSlider->SetValue(SliderValue + 1);
    }
}

