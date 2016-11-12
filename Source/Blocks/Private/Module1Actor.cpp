// Fill out your copyright notice in the Description page of Project Settings.

#include "Blocks.h"
#include "IPlatformFilePak.h"
#include "Block.h"
#include "Module1Actor.h"


// Sets default values
AModule1Actor::AModule1Actor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AModule1Actor::BeginPlay()
{
	Super::BeginPlay();
	printAllModels();
}

// Called every frame
void AModule1Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


#pragma optimize("",off)

void AModule1Actor::printAllModels()
{
	auto autobl = LoadObjFromPath<UBlueprint>(*(FPaths::GameContentDir() + TEXT("/Block1_Blueprint.Block1_Blueprint")));
	if (autobl)
	{
		auto topMost = autobl->ParentClass;

		if (topMost)
		{
			auto par = topMost->GetDefaultObject();

			if (par) {

				auto b = Cast<ABlock>(par);

				if (b)
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, *b->GetFullName());
				else

					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "nee");

			}

		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "neemám nic");
	}


	auto autobl1 = LoadObjFromPath<UBlueprint>(TEXT("/Game/Block1_Blueprint_Child_Child_Child.Block1_Blueprint_Child_Child_Child"));
	if (autobl1)
	{
		auto topMost1 = autobl1->ParentClass;

		if (topMost1)
		{
			auto par1 = topMost1->GetDefaultObject();

			if (par1) {

				auto b1 = Cast<ABlock>(par1);

				if (b1)
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, *b1->GetFullName());
				else

					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "nee");

			}

		}
	}



}
#pragma optimize("",on)
