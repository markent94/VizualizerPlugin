#include "CSVData.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

bool UCSVData::LoadCSVDataFromFile ( const FString& FilePath ) {
    FString FileContent;
    if ( !FFileHelper::LoadFileToString ( FileContent, *FilePath ) ) {
        UE_LOG ( LogTemp, Warning, TEXT ( "Failed to load CSV file." ) );
        return false;
    }

    TArray<FString> Lines;
    FileContent.ParseIntoArrayLines ( Lines, true );

    TMap<AActor*, UCSVActorTransformData*> LoadedTransformData;

    for ( int32 i = 1; i < Lines.Num(); ++i ) { // Skip header line
        TArray<FString> Columns;
        Lines[i].ParseIntoArray ( Columns, TEXT ( "," ), true );

        if ( Columns.Num() == 8 ) { // Check if there are enough columns
            FString ActorName = Columns[0];
            AActor* Actor = FindObject<AActor> ( ANY_PACKAGE, *ActorName );
            if ( Actor ) {
                FTransform Transform;
                Transform.SetLocation ( FVector ( FCString::Atof ( *Columns[2] ), FCString::Atof ( *Columns[3] ),
                                                  FCString::Atof ( *Columns[4] ) ) );
                Transform.SetRotation ( FQuat ( FCString::Atof ( *Columns[5] ), FCString::Atof ( *Columns[6] ),
                                                FCString::Atof ( *Columns[7] ), FCString::Atof ( *Columns[1] ) ) );

                if ( !LoadedTransformData.Contains ( Actor ) ) {
                    UCSVActorTransformData* NewTransformData = NewObject<UCSVActorTransformData>();
                    LoadedTransformData.Add ( Actor, NewTransformData );
                }
                LoadedTransformData[Actor]->TransformData.Add ( Transform );
            }
        }
    }

    ActorToTransformData = LoadedTransformData;
    return true;
}

const TArray<FTransformData>& UCSVData::GetTransformData() const {
    return TransformDataArray;
}

const TArray<FTransform>& UCSVData::GetTransformDataForActor ( AActor* Actor ) const {
    if ( ActorToTransformData.Contains ( Actor ) ) {
        UCSVActorTransformData* TransformDataObject = *ActorToTransformData.Find ( Actor );
        return TransformDataObject->GetTransformData();
    }

    // Return an empty array if the actor is not in the map
    static const TArray<FTransform> EmptyArray;
    return EmptyArray;
}



