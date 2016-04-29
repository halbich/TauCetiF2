

#include "TauCetiF2.h"
#include "WindowObject.h"


AWindowObject::AWindowObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	static ConstructorHelpers::FObjectFinder<UStaticMesh> corner(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/windowFrameCorner.windowFrameCorner'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> frame(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/windowFrame.windowFrame'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> windowPolycarbon(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/windowPolycarbon.windowPolycarbon'"));

	checkf(corner.Succeeded(), TEXT("Failed to find corner mesh"));
	checkf(frame.Succeeded(), TEXT("Failed to find frame mesh"));
	checkf(windowPolycarbon.Succeeded(), TEXT("Failed to find windowPolycarbon mesh"));

	LeftBottomCorner = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftBottomCorner"));
	LeftBottomCorner->SetStaticMesh(corner.Object);
	LeftBottomCorner->AttachTo(GetStaticMeshComponent());

	RightBottomCorner = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightBottomCorner"));
	RightBottomCorner->SetStaticMesh(corner.Object);
	RightBottomCorner->AttachTo(GetStaticMeshComponent());

	LeftTopCorner = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftTopCorner"));
	LeftTopCorner->SetStaticMesh(corner.Object);
	LeftTopCorner->AttachTo(GetStaticMeshComponent());

	RightTopCorner = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightTopCorner"));
	RightTopCorner->SetStaticMesh(corner.Object);
	RightTopCorner->AttachTo(GetStaticMeshComponent());

	Bottom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottom"));
	Bottom->SetStaticMesh(frame.Object);
	Bottom->AttachTo(GetStaticMeshComponent());

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetStaticMesh(frame.Object);
	Left->AttachTo(GetStaticMeshComponent());

	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	Top->SetStaticMesh(frame.Object);
	Top->AttachTo(GetStaticMeshComponent());

	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetStaticMesh(frame.Object);
	Right->AttachTo(GetStaticMeshComponent());


	Center = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Center"));
	Center->SetStaticMesh(windowPolycarbon.Object);
	Center->AttachTo(GetStaticMeshComponent());

}


void  AWindowObject::OnConstruction(const FTransform& Transform) {

	Super::OnConstruction(Transform);

	/*SelectTargetComponent->RegisterTargetObjectPrimitiveComponent(DoorFrameMesh);
	SelectTargetComponent->EnableUse(200, DoorMesh);*/

	auto s = Transform.GetScale3D();

	RightBottomCorner->ResetRelativeTransform();
	LeftBottomCorner->ResetRelativeTransform();
	RightTopCorner->ResetRelativeTransform();
	LeftTopCorner->ResetRelativeTransform();

	Bottom->ResetRelativeTransform();
	Right->ResetRelativeTransform();
	Top->ResetRelativeTransform();
	Left->ResetRelativeTransform();

	auto rbc = RightBottomCorner->GetRelativeTransform();
	auto lbc = LeftBottomCorner->GetRelativeTransform();
	auto rtc = RightTopCorner->GetRelativeTransform();
	auto ltc = LeftTopCorner->GetRelativeTransform();

	auto b = Bottom->GetRelativeTransform();
	auto r = Right->GetRelativeTransform();
	auto t = Top->GetRelativeTransform();
	auto l = Left->GetRelativeTransform();


	rbc.SetLocation(FVector(1 * GameDefinitions::CubeMinSizeHalf, 0, -1 * GameDefinitions::CubeMinSizeHalf));
	lbc.SetLocation(FVector(-1 * GameDefinitions::CubeMinSizeHalf, 0, -1 * GameDefinitions::CubeMinSizeHalf));
	rtc.SetLocation(FVector(1 * GameDefinitions::CubeMinSizeHalf, 0, 1 * GameDefinitions::CubeMinSizeHalf));
	ltc.SetLocation(FVector(-1 * GameDefinitions::CubeMinSizeHalf, 0, 1 * GameDefinitions::CubeMinSizeHalf));

	b.SetLocation(FVector(0, 0, -1 / s.Z * GameDefinitions::CubeMinSize));
	r.SetLocation(FVector(0.5f * GameDefinitions::CubeMinSize, 0, 0));
	t.SetLocation(FVector(0, 0, 0.5f * GameDefinitions::CubeMinSize));
	l.SetLocation(FVector(-0.5f * GameDefinitions::CubeMinSize, 0, 0));


	rbc.SetRotation(FQuat(FRotator(0, 0, 0)));
	lbc.SetRotation(FQuat(FRotator(-90, 0, 0)));
	rtc.SetRotation(FQuat(FRotator(90, 0, 0)));
	ltc.SetRotation(FQuat(FRotator(180, 0, 0)));

	b.SetRotation(FQuat(FRotator(0, 0, 0)));
	r.SetRotation(FQuat(FRotator(90, 0, 0)));
	t.SetRotation(FQuat(FRotator(180, 0, 0)));
	l.SetRotation(FQuat(FRotator(-90, 0, 0)));

	rbc.SetScale3D(FVector(1 / s.X, 1, 1 / s.Z));
	lbc.SetScale3D(FVector(1 / s.X, 1, 1 / s.Z));
	rtc.SetScale3D(FVector(1 / s.X, 1, 1 / s.Z));
	ltc.SetScale3D(FVector(1 / s.X, 1, 1 / s.Z));

	b.SetScale3D(FVector(1 / s.X, 1, 1 / s.Z));
	r.SetScale3D(FVector(1 / s.X, 1, 1 / s.Z));
	t.SetScale3D(FVector(1 / s.X, 1, 1 / s.Z));
	l.SetScale3D(FVector(1 / s.X, 1, 1 / s.Z));


	

	RightBottomCorner->SetRelativeTransform(rbc);
	LeftBottomCorner->SetRelativeTransform(lbc);
	RightTopCorner->SetRelativeTransform(rtc);
	LeftTopCorner->SetRelativeTransform(ltc);

	Bottom->SetRelativeTransform(b);
	Right->SetRelativeTransform(r);
	Top->SetRelativeTransform(t);
	Left->SetRelativeTransform(l);

}



void AWindowObject::SetBlockInfo(UBlockInfo* info, FBlockDefinition* definition)
{
	AWorldObject::SetBlockInfo(info, definition);


}

UStaticMeshComponent* AWindowObject::GetPrimitiveComponentByName(const FName& name)
{
	return AWorldObject::GetPrimitiveComponentByName(name);
}
