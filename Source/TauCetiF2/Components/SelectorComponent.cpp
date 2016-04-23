

#include "TauCetiF2.h"
#include "SelectorComponent.h"


// Sets default values for this component's properties
USelectorComponent::USelectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void USelectorComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner();


	cameraManager = UGameplayStatics::GetPlayerCameraManager(World, 0);
	playerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
}




// Called every frame
void USelectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	auto cameraLoc = cameraManager->GetCameraLocation();


	auto rot = playerPawn->GetControlRotation();
	auto hitEnd = FRotationMatrix(rot).GetScaledAxis(EAxis::X) * 1000 + cameraLoc;

	const FName TraceTag("MyTraceTag");

	World->DebugDrawTraceTag = TraceTag;

	FHitResult result;
	FCollisionObjectQueryParams params;

	params.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	params.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	FCollisionQueryParams CollisionParams;
	//CollisionParams.TraceTag = TraceTag;
	CollisionParams.AddIgnoredActor(traceIgnoreActor);

	if (!World->LineTraceSingleByObjectType(result, cameraLoc, hitEnd, params, CollisionParams))
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



	auto worldAct = Cast<AWorldObject>(act);
	if (!worldAct)
	{
		if (!act->ActorHasTag(TEXT("Selectable")))
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

	DrawDebugDirectionalArrow(World, ImpactPointWithSnap, ImpactPointWithSnap + ImpactNormal * GameDefinitions::CubeMinSize, 1, FColor::Red, false, 5);

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



	print(TEXT("showing"));

	if (false) {

		DrawDebugDirectionalArrow(GetWorld(), ImpactPoint, ImpactPoint + (ImpactNormal * 100), 300, FColor::Red, false, 10);

		auto rotator = ImpactNormal.Rotation() + FRotator(-90, 0, 0);

		FQuat rot(rotator);

		float angle(0);
		spawnedPlane->SetActorLocationAndRotation(ImpactPoint + ImpactNormal, rot); // UKismetMathLibrary::FindLookAtRotation(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation(), ImpactPoint));
		spawnedPlane->SetActorHiddenInGame(false);
		spawnedPlane->SetActorEnableCollision(true);

	}
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


void USelectorComponent::TrySelect() {

	if (!usableObjectTargeted)
		return;

	SelectedTarget->SelectTargetComponent->OnUse(owner);

}


void USelectorComponent::SetOutlining(bool enableOutlining, int32 stencilValue)
{
	StencilValue = stencilValue;
	outliningEnabled = enableOutlining;


	hideObjectOutline();
	showObjectOutline();

	hideUsableObjectOutline();
	showUsableObjectOutline();

}