#include "CSVReader.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

TArray<FTransformData> UCSVReader::ReadCSV(FString FilePath)
{
    TArray<FTransformData> Data;

    FString FullPath = FPaths::ProjectDir() + FilePath;
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FullPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load file: %s"), *FullPath);
        return Data;
    }

    TArray<FString> Lines;
    FileContent.ParseIntoArray(Lines, TEXT("\n"), true);

    for (int32 i = 1; i < Lines.Num(); i++) // Skip header line
    {
        TArray<FString> Columns;
        Lines[i].ParseIntoArray(Columns, TEXT(","), true);

        if (Columns.Num() == 7) // Expected number of columns
        {
            FTransformData TransformData;
            TransformData.TimeStep = FCString::Atof(*Columns[0]);
            TransformData.Position = 100 * FVector(FCString::Atof(*Columns[1]), FCString::Atof(*Columns[3]), -FCString::Atof(*Columns[2]));
            TransformData.Rotation = FRotator(FCString::Atof(*Columns[5]), FCString::Atof(*Columns[6]), FCString::Atof(*Columns[4]));

            Data.Add(TransformData);
        }
    }

    return Data;
}
