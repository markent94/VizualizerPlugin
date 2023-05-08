#include "CSVDataVisualizerEditorWidget.h"
#include "CSVData.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorFramework/AssetImportData.h"
#include "Framework/Application/SlateApplication.h"
#include "DesktopPlatformModule.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Images/SImage.h"
#include "Engine/Selection.h"
#include "GameFramework/Actor.h"

void SCsvDataVisualizerEditorWidget::Construct ( const FArguments& InArgs ) {
    UpdateAvailableActorsList();

    TSharedPtr<SEditableTextBox> FilePathTextBox;
    TSharedPtr<SButton> AddCsvButton;
    TSharedPtr<SVerticalBox> CsvListBox;

    ChildSlot
    [
        SNew ( SVerticalBox )
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding ( 2.0f )
        [
            SAssignNew ( ActorDropdown, SComboBox<TSharedPtr<FString>> )
            .OptionsSource ( &AvailableActors )
    .OnGenerateWidget_Lambda ( [] ( TSharedPtr<FString> InItem ) {
        return SNew ( STextBlock ).Text ( FText::FromString ( *InItem ) );
    } )
    .OnComboBoxOpening_Lambda ( [this]() {
        // Refresh the list of available actors when the dropdown is opened
        UpdateAvailableActorsList();
    } )
    .Content()
    [
        SNew ( STextBlock )
    .Text_Lambda ( [this]() {
        TSharedPtr<FString> SelectedItem = ActorDropdown->GetSelectedItem();
        return FText::FromString ( SelectedItem.IsValid() ? *SelectedItem : TEXT ( "Select an Actor" ) );
    } )
    ]
        ]
    + SVerticalBox::Slot()
    .AutoHeight()
    .Padding ( 2.0f )
    [
        SAssignNew ( FilePathTextBox, SEditableTextBox )
        .HintText ( FText::FromString ( TEXT ( "Enter CSV file path or click 'Browse'" ) ) )
    ]
    + SVerticalBox::Slot()
    .AutoHeight()
    .Padding ( 2.0f )
    [
        SNew ( SHorizontalBox )
        + SHorizontalBox::Slot()
        .AutoWidth()
        .Padding ( 2.0f )
        [
            SNew ( SButton )
            .Text ( FText::FromString ( TEXT ( "Browse" ) ) )
    .OnClicked_Lambda ( [FilePathTextBox]() {
        IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
        if ( DesktopPlatform ) {
            FString DefaultPath = FPaths::ProjectContentDir();
            TArray<FString> OpenFilePath;
            if ( DesktopPlatform->OpenFileDialog (
                        nullptr,
                        FString ( "Select CSV file" ),
                        DefaultPath,
                        FString ( "" ),
                        FString ( "CSV files (*.csv)|*.csv" ),
                        EFileDialogFlags::None,
                        OpenFilePath ) ) {
                FilePathTextBox->SetText ( FText::FromString ( OpenFilePath[0] ) );
            }
        }
        return FReply::Handled();
    } )
        ]
    + SHorizontalBox::Slot()
    .AutoWidth()
    .Padding ( 2.0f )
    [
        SAssignNew ( AddCsvButton, SButton )
        .Text ( FText::FromString ( TEXT ( "Add CSV" ) ) )
    .OnClicked_Lambda ( [this, FilePathTextBox]() {
        TSharedPtr<FString> SelectedActor = ActorDropdown->GetSelectedItem();
        if ( SelectedActor.IsValid() ) {
            for ( TObjectIterator<AActor> It; It; ++It ) {
                if ( It->GetName() == *SelectedActor ) {
                    AddCsvData ( FilePathTextBox->GetText().ToString(), *It );
                    break;
                }
            }
        }
        return FReply::Handled();
    } )
    ]
    ]
    + SVerticalBox::Slot()
    .AutoHeight()
    .Padding ( 2.0f )
    [
        SAssignNew ( CsvListBox, SVerticalBox )
    ]
    + SVerticalBox::Slot()
    .AutoHeight()
    .Padding ( 2.0f )
    [
        SNew ( SHorizontalBox )
        + SHorizontalBox::Slot()
        .AutoWidth()
        .Padding ( 2.0f )
        [
            SAssignNew ( PlayButton, SButton )
    .Text_Lambda ( [this]() {
        return FText::FromString ( bIsPlaying ? TEXT ( "Stop" ) : TEXT ( "Play" ) );
    } )
    .OnClicked_Raw ( this, &SCsvDataVisualizerEditorWidget::TogglePlayback )
        ]
    + SHorizontalBox::Slot()
    .FillWidth ( 1.0f )
    .Padding ( 2.0f )
    [
        SAssignNew ( PlaybackSlider, SSlider )
    .Value_Lambda ( [this]() {
        return PlaybackSpeed;
    } ) // Use Value_Lambda instead of Value
    .OnValueChanged_Lambda ( [this] ( float NewValue ) {
        PlaybackSpeed = NewValue;
    } )
    .Orientation ( EOrientation::Orient_Horizontal )
    ]
    + SHorizontalBox::Slot()
    .AutoWidth()
    .Padding ( 2.0f )
    [
        SNew ( STextBlock )
    .Text_Lambda ( [this]() {
        TSharedPtr<FString> SelectedActor = ActorDropdown->GetSelectedItem();
        int32 NumFrames = 0;
        if ( SelectedActor.IsValid() && CsvDataMap.Contains ( SelectedActor ) ) {
            UCSVData* SelectedCsvData = CsvDataMap[SelectedActor];
            NumFrames = SelectedCsvData->GetTransformData().Num() - 1;
        }

        return FText::Format ( FText::FromString ( "Frame: {0}" ),
                               FText::AsNumber ( FMath::RoundToInt ( PlaybackProgress * NumFrames ) ) );
    } )
    ]
    ]
    ];

    CsvList = CsvListBox;
    PlaybackSpeed = 1.0f;
    bIsPlaying = false;
}

void SCsvDataVisualizerEditorWidget::UpdateAvailableActorsList() {
    AvailableActors.Empty();
    for ( TObjectIterator<AActor> It; It; ++It ) {
        if ( It->IsSelected() ) {
            AvailableActors.Add ( MakeShared<FString> ( It->GetName() ) );
        }
    }
}

void SCsvDataVisualizerEditorWidget::AddCsvData ( const FString& FilePath, AActor* Actor ) {
    UCSVData* CsvData = NewObject<UCSVData>();
    if ( CsvData->LoadCSVDataFromFile ( FilePath ) ) {
        TSharedPtr<FString> ActorName = MakeShared<FString> ( Actor->GetName() );
        CsvDataMap.Add ( ActorName, CsvData );

        CsvList->AddSlot()
        .AutoHeight()
        .Padding ( 2.0f )
        [
            SNew ( SHorizontalBox )
            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding ( 2.0f )
            [
                SNew ( STextBlock )
                .Text ( FText::FromString ( *ActorName ) )
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding ( 2.0f )
            [
                SNew ( SButton )
                .Text ( FText::FromString ( TEXT ( "Remove" ) ) )
        .OnClicked_Lambda ( [this, ActorName]() {
            CsvDataMap.Remove ( ActorName );
            UpdateAvailableActorsList();
            return FReply::Handled();
        } )
            ]
        ];
    }
}

// Add the TogglePlayback() function
FReply SCsvDataVisualizerEditorWidget::TogglePlayback() {
    bIsPlaying = !bIsPlaying;

    if ( bIsPlaying ) {
        TickDelegateHandle = FTicker::GetCoreTicker().AddTicker (
        FTickerDelegate::CreateLambda ( [this] ( float DeltaTime ) -> bool {
            return this->OnEditorTickHandler ( DeltaTime );
        } )
                             );
    } else {
        FTicker::GetCoreTicker().RemoveTicker ( TickDelegateHandle );
    }

    return FReply::Handled();
}




bool SCsvDataVisualizerEditorWidget::OnEditorTickHandler ( float DeltaTime ) {
    PlaybackProgress += PlaybackSpeed * DeltaTime;

    // Iterate through the CSV data map and apply the transformation data to the corresponding actors
    for ( const auto& CsvDataEntry : CsvDataMap ) {
        AActor* Actor = FindObject<AActor> ( ANY_PACKAGE, **CsvDataEntry.Key.Get(), true );
        if ( Actor ) {
            const TArray<FTransform>& TransformData = CsvDataEntry.Value->GetTransformDataForActor ( Actor );
            if ( TransformData.Num() > 0 ) {
                int32 CurrentFrame = FMath::Clamp ( FMath::RoundToInt ( PlaybackProgress * TransformData.Num() ), 0,
                                                    TransformData.Num() - 1 );
                Actor->SetActorTransform ( TransformData[CurrentFrame] );
            }
        }
    }

    // Reset playback progress if it exceeds 1.0
    if ( PlaybackProgress >= 1.0f ) {
        PlaybackProgress = 0.0f;
    }

    // Return true to keep the ticker running, false to stop it
    return bIsPlaying;
}



