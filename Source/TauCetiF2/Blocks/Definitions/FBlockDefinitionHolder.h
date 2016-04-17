

#pragma once

#include "Blocks/Definitions/FBlockDefinition.h"
#include "Blocks/Definitions/FBlockMaterialDefinition.h"

/**
 *
 */
class TAUCETIF2_API FBlockDefinitionHolder
{
public:


	static FBlockDefinitionHolder& Instance();

	FORCEINLINE FBlockDefinition* GetDefinition(uint32 id, bool ensure = true) {
		auto definition = definitions.Find(id);

		if (!ensure)
			return definition;

		ensureMsgf(definition != nullptr, TEXT("Definition (%d) was not found!"), id);

		return definition;
	}


private:

	FBlockDefinitionHolder();

	FBlockDefinitionHolder(const FBlockDefinitionHolder&) = delete;
	FBlockDefinitionHolder& operator= (const FBlockDefinitionHolder&) = delete;

	TMap<uint32, FBlockDefinition> definitions;
};
