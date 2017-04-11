#include "Blocks.h"
#include "BlockWithElectricity.h"

UBlockWithElectricity::UBlockWithElectricity(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void IBlockWithElectricity::UpdateHealth(UElectricityComponent* comp, UBlockInfo* info)
{

}