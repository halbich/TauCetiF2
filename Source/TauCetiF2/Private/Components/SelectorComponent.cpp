#include "TauCetiF2.h"
#include "SelectorComponent.h"

// Sets default values for this component's properties
USelectorComponent::USelectorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USelectorComponent::OwnerBecomeViewTarget()
{
	owner = GetOwner();

	auto world = GetWorld();
	cameraManager = UGameplayStatics::GetPlayerCameraManager(world, 0);
	playerPawn = UGameplayStatics::GetPlayerPawn(world, 0);
}

// Called every frame
void USelectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!playerPawn)
		return;

	auto cameraLoc = cameraManager->GetCameraLocation();

	auto rot = playerPawn->GetControlRotation();
	auto hitEnd = FRotationMatrix(rot).GetScaledAxis(EAxis::X) * 1000 + cameraLoc;

	const FName TraceTag("MyTraceTag");

	GetWorld()->DebugDrawTraceTag = TraceTag;

	FHitResult result;
	FCollisionObjectQueryParams params;

	params.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	params.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	FCollisionQueryParams CollisionParams;
	//CollisionParams.TraceTag = TraceTag;
	CollisionParams.AddIgnoredActor(traceIgnoreActor);

	if (!GetWorld()->LineTraceSingleByObjectType(result, cameraLoc, hitEnd, params, CollisionParams))
	{
		deselectComponent();
		return;
	}

	auto act = result.GetActor();
	if (!act)
	{
		deselectComponent();
		return;
	}

	auto worldAct = Cast<ABlock>(act);
	if (!worldAct)
	{
		auto slComp = act->GetComponentByClass(USelectTargetComponent::StaticClass());

		if (!act->ActorHasTag(TEXT("Selectable")) && !slComp)
		{
			deselectComponent();
			return;
		}
	}

	int32 cubeSize = GameDefinitions::CubeMinSize;

	ImpactPoint = result.ImpactPoint;
	ImpactNormal = result.ImpactNormal.GridSnap(1);
	ImpactPointWithSnap = FVector(
		FMath::RoundToInt((ImpactPoint.X + ImpactNormal.X) / cubeSize) * cubeSize,
		FMath::RoundToInt((ImpactPoint.Y + ImpactNormal.Y) / cubeSize) * cubeSize,
		FMath::RoundToInt((ImpactPoint.Z + ImpactNormal.Z) / cubeSize) * cubeSize);

	//DrawDebugDirectionalArrow(World, ImpactPointWithSnap, ImpactPointWithSnap + ImpactNormal * GameDefinitions::CubeMinSize, 1, FColor::Red, false, 5);

	IsValidBuildingLocation = true;
	selectComponent(act, worldAct);
}

void USelectorComponent::ShowPlane()
{
	if (!spawnedPlane)
	{
		if (!LockingPlane)
			return;

		spawnedPlane = GetWorld()->SpawnActor(LockingPlane->GeneratedClass);
	}

	if (!spawnedPlane)
	{
		return;
	}

	DrawDebugDirectionalArrow(GetWorld(), ImpactPointWithSnap, ImpactPointWithSnap + (ImpactNormal * 100), 300, FColor::Red, false, 10);

	auto rotator = ImpactNormal.Rotation() + FRotator(-90, 0, 0);

	FQuat rot(rotator);

	spawnedPlane->SetActorLocationAndRotation(ImpactPointWithSnap + ImpactNormal, rot);
	spawnedPlane->SetActorHiddenInGame(false);
	spawnedPlane->SetActorEnableCollision(true);
}

void USelectorComponent::HidePlane()
{
	if (!spawnedPlane)
	{
		return;
	}

	spawnedPlane->SetActorHiddenInGame(true);
	spawnedPlane->SetActorEnableCollision(false);
}

void USelectorComponent::TryUse(bool isSpecialUse) {
	if (!usableObjectTargeted)
		return;

	SelectedBlock->SelectTargetComponent->OnUse(owner, isSpecialUse);
}

void USelectorComponent::SetOutlining(bool enableOutlining, int32 outlineStencilValue)
{
	StencilValue = outlineStencilValue;
	outliningEnabled = enableOutlining;

	hideObjectOutline();
	hideUsableObjectOutline();

	if (!showUsableObjectOutline())
		showObjectOutline();
}