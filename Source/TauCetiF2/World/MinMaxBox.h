

#pragma once

#include "Object.h"
#include "Helpers/Helpers.h"
#include "MinMaxBox.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class TAUCETIF2_API UMinMaxBox : public UObject
{
	GENERATED_BODY()

public:

	UMinMaxBox* InitBox(FVector min, FVector max);
	UMinMaxBox* InitBoxChecked(FVector min, FVector max);

	UPROPERTY()
		UMinMaxBox* ParentNode;

	UPROPERTY()
		FVector Min;

	UPROPERTY()
		FVector Max;

	template <typename T>
		T* GetParentNode( bool allowSelf = false)
	{
		auto returnValue = ParentNode;
		if (returnValue == nullptr)
			return allowSelf ? Cast<T>(this) : nullptr;

		auto newReturn = returnValue->ParentNode;
		while (newReturn)
		{
			returnValue = newReturn;
			newReturn = returnValue->ParentNode;
		}

		return Cast<T>(returnValue);
	}

};
