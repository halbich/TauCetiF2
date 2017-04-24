#include "TauCetiF2.h"
#include "BaseControlWidget.h"

UBaseControlWidget::UBaseControlWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBaseControlWidget::InitForBlock_Implementation(ABlock* block)
{
	Super::InitForBlock_Implementation(block);

	auto terminal = Cast<ATerminalBlock>(block);
	if (terminal && terminal->IsValidLowLevel() && terminal->ElectricityComponent && terminal->ElectricityComponent->IsValidLowLevel())
		Network = terminal->ElectricityComponent->Network;

	ensure(Network);

	ContainingTerminalWidgets.Empty();

	ensure(wTerminalBase);
	ensure(wBlockConstructor);

	auto w = GetWorld();

	if (!w)
		return;

	auto TerminalBase = CreateWidget<UTerminalBaseWidget>(w, wTerminalBase);
	TerminalBase->InitForBlock(block);
	ContainingTerminalWidgets.Add(TerminalBase);

	TArray<UCreatorPatternGroupInfo*> creators;

	for (auto constrPat : Network->PatternBlocks)
	{
		auto pi = Cast<UCreatorPatternGroupInfo>(constrPat->WorldObjectComponent->PatternGroupInfo);
		if (pi && pi->IsValidLowLevel())
			creators.AddUnique(pi);
	}

	for (auto cr : creators)
	{
		auto crWidg = CreateWidget<UBlockConstructor>(w, wBlockConstructor);
		crWidg->RelatedToPatternGroup = cr;
		crWidg->InitForBlock(block);
		ContainingTerminalWidgets.Add(crWidg);
	}
}