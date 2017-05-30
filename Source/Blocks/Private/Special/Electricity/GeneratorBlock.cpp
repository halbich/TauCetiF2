#include "Blocks.h"
#include "GeneratorBlock.h"

#define RED 2
#define GREEN 1
#define BLUE 0
#define ALPHA 3

AGeneratorBlock::AGeneratorBlock()
	:Super()
{
	PrimaryActorTick.bCanEverTick = false;

	GetRootComponent()->SetMobility(EComponentMobility::Stationary);

	GeneratorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeneratorMesh"));
	GeneratorMesh->SetupAttachment(GetRootComponent());
	GeneratorMesh->SetMobility(EComponentMobility::Stationary);


	ElectricityComponent = CreateDefaultSubobject<UElectricityComponent>(TEXT("ElectricityComponent"));
	AddOwnedComponent(ElectricityComponent);

}


void AGeneratorBlock::BeginPlay()
{
	world = GetWorld();
	Super::BeginPlay();
}

void AGeneratorBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void  AGeneratorBlock::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);

	bool anEn = false;
	if (UCommonHelpers::GetSettingsValueBool(EGameUserSettingsVariable::GeneratorVisualizationEnabled, anEn))
	{
		AnimationEnabled = anEn;
	}
	else
		AnimationEnabled = false;


	Scale = GetBlockScale();
	particles.AddDefaulted(Scale.X*Scale.Y);

}


UStaticMeshComponent* AGeneratorBlock::GetMeshStructureComponent_Implementation(int32 BlockMeshStructureDefIndex)
{
	if (BlockMeshStructureDefIndex == 0)
		return GeneratorMesh;

	return Super::GetMeshStructureComponent_Implementation(BlockMeshStructureDefIndex);
}

UPrimitiveComponent* AGeneratorBlock::GetComponentForObjectOutline_Implementation() {
	return GeneratorMesh;
}

void AGeneratorBlock::WasHitByStorm(const FVector& blockHitLocation, const float amount, const FVector& hitWorldLocation)
{

	if (AnimationEnabled) {

		if (!world || !world->IsValidLowLevel() || world->IsPendingKill())
			world = GetWorld();

		auto index = getParticleIndex(blockHitLocation);
		ensure(particles.IsValidIndex(index));
		auto p = particles[index];
		if (!p || !p->IsValidLowLevel())
		{
			p = particles[index] = UGameplayStatics::SpawnEmitterAtLocation(world, ParticleEmitter, hitWorldLocation, FRotator::ZeroRotator, false);
		}

		p->Activate(true);
		
	}

	auto energyToPut = amount *  GameDefinitions::RainHitpointToEnergy;
	float actuallyPutted = 0;
	if (ElectricityComponent->PutAmount(energyToPut, actuallyPutted))	// first we try to fill internal buffer
	{
		energyToPut = FMath::Max(0.0f, energyToPut - actuallyPutted);
		ElectricityComponent->EnergyProduced += actuallyPutted;
	}

	// if we have left something, we to find appropriate storage randomly
	if (energyToPut > 0 && ElectricityComponent->ComponentNetworkState == EElectricNetworkState::Valid)
	{
		auto n = ElectricityComponent->Network;
		auto ns = n->ElectricityStorages.Num();
		if (ns > 0)
		{
			auto random = FMath::RandHelper(ns);
			ensure(n->ElectricityStorages.IsValidIndex(random));

			auto stor = n->ElectricityStorages[random];

			if (stor->ComponentNetworkState == EElectricNetworkState::Valid)
			{
				float storageActuallyPutted = 0;
				if (stor->PutAmount(energyToPut, storageActuallyPutted))
				{
					energyToPut = FMath::Max(0.0f, energyToPut - storageActuallyPutted);
					ElectricityComponent->EnergyProduced += storageActuallyPutted;
				}
			}
		}
	}

	energyToPut *= GameDefinitions::EnergyToRainHitpoint;

	ensure(energyToPut >= 0);

	if (!FMath::IsNearlyZero(energyToPut))
		Super::WasHitByStorm(blockHitLocation, energyToPut, hitWorldLocation);
}