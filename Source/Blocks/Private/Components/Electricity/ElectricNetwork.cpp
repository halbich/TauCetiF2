

#include "Blocks.h"
#include "ElectricNetwork.h"





void UElectricNetwork::RegisterEntity(UElectricityComponent* comp)
{
	entities.Add(comp);
}


void UElectricNetwork::UnregisterEntity(UElectricityComponent* comp)
{
	entities.Remove(comp);
}