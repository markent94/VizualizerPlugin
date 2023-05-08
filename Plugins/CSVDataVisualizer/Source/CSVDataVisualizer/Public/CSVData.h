#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSVActorTransformData.h"
#include "CSVData.generated.h"

USTRUCT( BlueprintType )
struct FTransformData {
    GENERATED_BODY()

    UPROPERTY ( BlueprintReadOnly, Category = "Transform Data" )
    float Time;

    UPROPERTY ( BlueprintReadOnly, Category = "Transform Data" )
    FVector Location;

    UPROPERTY ( BlueprintReadOnly, Category = "Transform Data" )
    FRotator Rotation;

    UPROPERTY ( BlueprintReadOnly, Category = "Transform Data" )
    FVector Scale;
};

UCLASS( Blueprintable )
class CSVDATAVISUALIZER_API UCSVData : public UObject {
    GENERATED_BODY()

  public:
    UFUNCTION ( BlueprintCallable, Category = "CSV Data" )
    bool LoadCSVDataFromFile ( const FString& FilePath );

    UFUNCTION ( BlueprintPure, Category = "CSV Data" )
    const TArray<FTransformData>& GetTransformData() const;

    UFUNCTION ( BlueprintCallable, Category = "CSV Data" )
    const TArray<FTransform>& GetTransformDataForActor ( AActor* Actor ) const;

    UPROPERTY ( BlueprintReadOnly, Category = "CSV Data" )
    TMap<AActor*, UCSVActorTransformData*> ActorToTransformData;

  private:
    TArray<FTransformData> TransformDataArray;
};
