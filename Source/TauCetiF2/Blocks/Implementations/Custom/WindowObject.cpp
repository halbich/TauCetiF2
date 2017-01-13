

#include "TauCetiF2.h"
#include "WindowObject.h"


//AWindowObject::AWindowObject(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//
//	static ConstructorHelpers::FObjectFinder<UStaticMesh> corner(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/windowFrameCorner.windowFrameCorner'"));
//	static ConstructorHelpers::FObjectFinder<UStaticMesh> frame(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/windowFrame.windowFrame'"));
//	static ConstructorHelpers::FObjectFinder<UStaticMesh> windowPolycarbon(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/windowPolycarbon.windowPolycarbon'"));
//
//	checkf(corner.Succeeded(), TEXT("Failed to find corner mesh"));
//	checkf(frame.Succeeded(), TEXT("Failed to find frame mesh"));
//	checkf(windowPolycarbon.Succeeded(), TEXT("Failed to find windowPolycarbon mesh"));
//
//	LeftBottomCorner = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftBottomCorner"));
//	LeftBottomCorner->SetStaticMesh(corner.Object);
//	LeftBottomCorner->SetupAttachment(GetStaticMeshComponent());
//
//	RightBottomCorner = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightBottomCorner"));
//	RightBottomCorner->SetStaticMesh(corner.Object);
//	RightBottomCorner->SetupAttachment(GetStaticMeshComponent());
//
//	LeftTopCorner = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftTopCorner"));
//	LeftTopCorner->SetStaticMesh(corner.Object);
//	LeftTopCorner->SetupAttachment(GetStaticMeshComponent());
//
//	RightTopCorner = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightTopCorner"));
//	RightTopCorner->SetStaticMesh(corner.Object);
//	RightTopCorner->SetupAttachment(GetStaticMeshComponent());
//
//	Bottom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottom"));
//	Bottom->SetStaticMesh(frame.Object);
//	Bottom->SetupAttachment(GetStaticMeshComponent());
//
//	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
//	Left->SetStaticMesh(frame.Object);
//	Left->SetupAttachment(GetStaticMeshComponent());
//
//	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
//	Top->SetStaticMesh(frame.Object);
//	Top->SetupAttachment(GetStaticMeshComponent());
//
//	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
//	Right->SetStaticMesh(frame.Object);
//	Right->SetupAttachment(GetStaticMeshComponent());
//
//
//	Center = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Center"));
//	Center->SetStaticMesh(windowPolycarbon.Object);
//	Center->SetupAttachment(GetStaticMeshComponent());
//
//}
//
//
//void  AWindowObject::OnConstruction(const FTransform& Transform) {
//
//	Super::OnConstruction(Transform);
//
//	/*SelectTargetComponent->RegisterTargetObjectPrimitiveComponent(DoorFrameMesh);
//	SelectTargetComponent->EnableUse(200, DoorMesh);*/
//
//	auto s = Transform.GetScale3D();
//
//	RightBottomCorner->ResetRelativeTransform();
//	LeftBottomCorner->ResetRelativeTransform();
//	RightTopCorner->ResetRelativeTransform();
//	LeftTopCorner->ResetRelativeTransform();
//
//	Bottom->ResetRelativeTransform();
//	Right->ResetRelativeTransform();
//	Top->ResetRelativeTransform();
//	Left->ResetRelativeTransform();
//
//	Center->ResetRelativeTransform();
//
//	auto rbc = RightBottomCorner->GetRelativeTransform();
//	auto lbc = LeftBottomCorner->GetRelativeTransform();
//	auto rtc = RightTopCorner->GetRelativeTransform();
//	auto ltc = LeftTopCorner->GetRelativeTransform();
//
//	auto b = Bottom->GetRelativeTransform();
//	auto r = Right->GetRelativeTransform();
//	auto t = Top->GetRelativeTransform();
//	auto l = Left->GetRelativeTransform();
//
//	auto c = Center->GetRelativeTransform();
//
//	rbc.SetLocation(FVector(GameDefinitions::CubeMinSizeHalf * (1 - 1 / s.X), 0, -GameDefinitions::CubeMinSizeHalf * (1 - 1 / s.Z)));
//	lbc.SetLocation(FVector(-GameDefinitions::CubeMinSizeHalf * (1 - 1 / s.X), 0, -GameDefinitions::CubeMinSizeHalf * (1 - 1 / s.Z)));
//	rtc.SetLocation(FVector(GameDefinitions::CubeMinSizeHalf * (1 - 1 / s.X), 0, GameDefinitions::CubeMinSizeHalf * (1 - 1 / s.Z)));
//	ltc.SetLocation(FVector(-GameDefinitions::CubeMinSizeHalf * (1 - 1 / s.X), 0, GameDefinitions::CubeMinSizeHalf * (1 - 1 / s.Z)));
//
//	b.SetLocation(FVector(0, 0, -GameDefinitions::CubeMinSizeHalf * (1 - 0.5 / s.Z)));
//	r.SetLocation(FVector(GameDefinitions::CubeMinSizeHalf * (1 - 0.5 / s.X), 0, 0));
//	t.SetLocation(FVector(0, 0, GameDefinitions::CubeMinSizeHalf * (1 - 0.5 / s.Z)));
//	l.SetLocation(FVector(-GameDefinitions::CubeMinSizeHalf * (1 - 0.5 / s.X), 0, 0));
//
//
//	rbc.SetRotation(FQuat(FRotator(0, 0, 0)));
//	lbc.SetRotation(FQuat(FRotator(-90, 0, 0)));
//	rtc.SetRotation(FQuat(FRotator(90, 0, 0)));
//	ltc.SetRotation(FQuat(FRotator(180, 0, 0)));
//
//	b.SetRotation(FQuat(FRotator(0, 0, 0)));
//	r.SetRotation(FQuat(FRotator(90, 0, 0)));
//	t.SetRotation(FQuat(FRotator(180, 0, 0)));
//	l.SetRotation(FQuat(FRotator(-90, 0, 0)));
//
//	rbc.SetScale3D(FVector(1 / s.X, 1, 1 / s.Z));
//	lbc.SetScale3D(FVector(1 / s.X, 1, 1 / s.Z));
//	rtc.SetScale3D(FVector(1 / s.X, 1, 1 / s.Z));
//	ltc.SetScale3D(FVector(1 / s.X, 1, 1 / s.Z));
//
//	b.SetScale3D(FVector(1 - 2 / s.X, 1, 1 / s.Z));
//	r.SetScale3D(FVector((s.Z -2) /s.X , 1, 1 / s.Z));
//	t.SetScale3D(FVector(1 - 2 / s.X, 1, 1 / s.Z));
//	l.SetScale3D(FVector((s.Z - 2) / s.X, 1, 1 / s.Z));
//
//	c.SetScale3D(FVector((1 - (0.5 / s.X)) / 0.75, 1, (1 - (0.5 / s.Z)) / 0.75));
//
//
//	RightBottomCorner->SetRelativeTransform(rbc);
//	LeftBottomCorner->SetRelativeTransform(lbc);
//	RightTopCorner->SetRelativeTransform(rtc);
//	LeftTopCorner->SetRelativeTransform(ltc);
//
//	Bottom->SetRelativeTransform(b);
//	Right->SetRelativeTransform(r);
//	Top->SetRelativeTransform(t);
//	Left->SetRelativeTransform(l);
//
//	Center->SetRelativeTransform(c);
//}
//
//
//
//void AWindowObject::SetBlockInfo(UBlockInfo* info, UBlockDefinition* definition)
//{
//	AWorldObject::SetBlockInfo(info, definition);
//
//
//}
//
//UStaticMeshComponent* AWindowObject::GetPrimitiveComponentByName(const FName& name)
//{
//	return AWorldObject::GetPrimitiveComponentByName(name);
//}
