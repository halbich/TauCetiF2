// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Module1Actor.generated.h"

UCLASS()
class BLOCKS_API AModule1Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModule1Actor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private:
	
	
	//TEMPLATE Load Obj From Path
	template <typename ObjClass>
	static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
	{
		if (Path == NAME_None) return NULL;
		//~

		return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
	}


	// Load Static Mesh From Path 
	static FORCEINLINE ABlock* LoadBlockFromPath(const FName& Path)
	{
		if (Path == NAME_None) return NULL;
		//~

		return LoadObjFromPath<ABlock>(Path);
	}

	void printAllModels();
};
