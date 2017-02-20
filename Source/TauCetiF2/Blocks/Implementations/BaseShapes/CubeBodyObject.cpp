#include "TauCetiF2.h"
#include "CubeBodyObject.h"
//#include "Engine/DestructibleMesh.h"

//ACubeBodyObject::ACubeBodyObject(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	//auto dc = GetDestructibleComponent();
//	//if (!dc)
//	//	return;
//
//	//ConstructorHelpers::FObjectFinder<UDestructibleMesh> destructible(TEXT("DestructibleMesh'/Game/BuildingObjects/DestructibleMeshes/box2_DM.box2_DM'"));
//
//	//if (destructible.Succeeded())
//	//	dc->SetDestructibleMesh(destructible.Object);
//
//	static ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/BuildingObjects/Meshes/box2.box2'"));
//
//	checkf(mesh.Succeeded(), TEXT("Failed to find mesh"));
//
//	constructorSetMeshes(mesh.Object);
//
//}