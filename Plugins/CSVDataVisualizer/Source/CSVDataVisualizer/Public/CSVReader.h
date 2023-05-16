#pragma once

#include "CoreMinimal.h"
#include "CSVReader.generated.h"

USTRUCT(BlueprintType)
struct FTransformData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float TimeStep;

    UPROPERTY(BlueprintReadOnly)
    FVector Position;

    UPROPERTY(BlueprintReadOnly)
    FRotator Rotation;
};

UCLASS()
class CSVDATAVISUALIZER_API UCSVReader : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "CSV")
    static TArray<FTransformData> ReadCSV(FString FilePath);
};
