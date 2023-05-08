#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "CSVActorTransformData.generated.h"

UCLASS(Blueprintable)
class CSVDATAVISUALIZER_API UCSVActorTransformData : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "CSV Data")
    TArray<FTransform> TransformData;

    // Add this function
    const TArray<FTransform>& GetTransformData() const
    {
        return TransformData;
    }
};
