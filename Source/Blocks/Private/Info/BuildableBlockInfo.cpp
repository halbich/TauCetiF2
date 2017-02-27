#include "Blocks.h"
#include "BuildableBlockInfo.h"

UBuildableBlockInfo::UBuildableBlockInfo() : UBlockBaseInfo(), Action(EBuildableObjectAction::None), AllowOutlineOnSelected(true)
{
}

void UBuildableBlockInfo::DefinitionSet()
{
	switch (ID)
	{
	case EmptyHandID: {
		Action = EBuildableObjectAction::None;
		break;
	}
	case DeleteID: {
		StencilOverride = STENCIL_DELETE_OUTLINE;
		Action = EBuildableObjectAction::DeleteObject;
		break;
	}
	default:
		Action = EBuildableObjectAction::ConstructObject;
		break;
	}

}