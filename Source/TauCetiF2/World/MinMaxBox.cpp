

#include "TauCetiF2.h"
#include "MinMaxBox.h"

FMinMaxBox::FMinMaxBox(FVector min, FVector max, int8 dividingIndex) : B1(nullptr), B2(nullptr), Min(min), Max(max), containingObject(nullptr), DividingIndex(dividingIndex), SingleChild(nullptr)
{
	name = TEXT("");
}

FMinMaxBox::~FMinMaxBox()
{
	if (B1)
	{
		delete B1;
		B1 = nullptr;
	}

	if (B2)
	{
		delete B2;
		B2 = nullptr;
	}
}

void FMinMaxBox::AddToTree(FMinMaxBox* box, bool forceInsert) {

	/*if (DividingIndex >= )
	{
		UE_LOG(LogTemp, Log, TEXT("Dive EXCEEDED!"));
		return;
	}
*/
	ensure(box != nullptr);

	/*UE_LOG(LogTemp, Log, TEXT("Adding box to %s ( (%s) , (%s))"), *name, *Min.ToString(), *Max.ToString());

	if (box->containingObject && box->containingObject->IsValidLowLevel())
		UE_LOG(LogTemp, Log, TEXT("Inner item name is %s"), *(box->containingObject->GetName()));*/

	if (!B1 && !B2 && !SingleChild && GtMin(box->Min) && LtMax(box->Max))
	{
		//UE_LOG(LogTemp, Log, TEXT("Box %s has no child, adding Single"), *name);
		SingleChild = box;
		return;
	}


	if (SingleChild && !forceInsert)
	{
		//UE_LOG(LogTemp, Log, TEXT(" ---- Box %s has single child, force add"), *name);
		AddToTree(SingleChild, true); // forcing to insert
		SingleChild = nullptr;
		//UE_LOG(LogTemp, Log, TEXT(" ---- Box %s had single child, forcing done"), *name);
	}

	//UE_LOG(LogTemp, Log, TEXT("Searching where can be box (%s) stored in %s"), box->containingObject ? *(box->containingObject->GetName()) : TEXT("nothing"), *name);


	float dividingCoord = 0;
	switch (DividingIndex % 3) {
	case 0:
		dividingCoord = ((Min.X + Max.X)* 0.5);

		if (((int32)FMath::Abs(Max.X - Min.X) / 20) % 2 == 1)
		{
			dividingCoord += 10.0f;
		}
		addToTreeByX(box, dividingCoord);
		break;
	case 1:
		dividingCoord = (Min.Y + Max.Y)* 0.5;
		if (((int32)FMath::Abs(Max.Y - Min.Y) / 20) % 2 == 1)
		{
			dividingCoord += 10.0f;
		}
		addToTreeByY(box, dividingCoord);

		break;
	case 2:
		dividingCoord = (Min.Z + Max.Z)* 0.5;
		if (((int32)FMath::Abs(Max.Z - Min.Z) / 20) % 2 == 1)
		{
			dividingCoord += 10.0f;
		}
		addToTreeByZ(box, dividingCoord);

		break;
	}
}


void FMinMaxBox::addToTreeByX(FMinMaxBox* box, float& dividingCoord) {
	//UE_LOG(LogTemp, Log, TEXT("Box %s has dividingCoord X  %f"), *name, dividingCoord);

	if (box->Max.X <= dividingCoord)		// whole object is in left plane
	{
		//UE_LOG(LogTemp, Log, TEXT("Box %s is all in left plane of %s"), *(box->name), *name);
		if (!B1)
		{
			//UE_LOG(LogTemp, Log, TEXT("Creating B1 for %s"), *name);
			B1 = new FMinMaxBox(Min, FVector(dividingCoord, Max.Y, Max.Z), DividingIndex + 1);
			B1->name = FString::Printf(TEXT("%s > B1"), *name);
		}
		B1->AddToTree(box);
		return;
	}

	if (box->Min.X >= dividingCoord)		// whole object is in right plane
	{
		//UE_LOG(LogTemp, Log, TEXT("Box %s is all in right plane of %s"), *(box->name), *name);
		if (!B2)
		{
			//UE_LOG(LogTemp, Log, TEXT("Creating B2 for %s"), *name);
			B2 = new FMinMaxBox(FVector(dividingCoord, Min.Y, Min.Z), Max, DividingIndex + 1);
			B2->name = FString::Printf(TEXT("%s > B2"), *name);
		}

		B2->AddToTree(box);
		return;
	}

	// object is in between. We need to split and then add object to both branches

	//UE_LOG(LogTemp, Log, TEXT("----   - ----Object %s needs to be divided (in %s)"), box->containingObject ? *(box->containingObject->GetName()) : TEXT("nothing"), *name);

	FMinMaxBox* newB1 = new FMinMaxBox(box->Min, box->Max, box->DividingIndex);
	newB1->containingObject = box->containingObject;


	box->Max.X = dividingCoord;
	newB1->Min.X = dividingCoord;
	addToTreeByX(box, dividingCoord);
	addToTreeByX(newB1, dividingCoord);



}


void FMinMaxBox::addToTreeByY(FMinMaxBox* box, float dividingCoord) {
	//UE_LOG(LogTemp, Log, TEXT("Box %s has dividingCoord Y %f"), *name, dividingCoord);

	if (box->Max.Y <= dividingCoord)		// whole object is in left plane
	{
		//UE_LOG(LogTemp, Log, TEXT("Box %s is all in left plane of %s"), *(box->name), *name);
		if (!B1)
		{
			//UE_LOG(LogTemp, Log, TEXT("Creating B1 for %s"), *name);
			B1 = new FMinMaxBox(Min, FVector(Max.X, dividingCoord, Max.Z), DividingIndex + 1);
			B1->name = FString::Printf(TEXT("%s > B1"), *name);
		}
		B1->AddToTree(box);
		return;
	}

	if (box->Min.Y >= dividingCoord)		// whole object is in right plane
	{
		//UE_LOG(LogTemp, Log, TEXT("Box %s is all in right plane of %s"), *(box->name), *name);
		if (!B2)
		{
			//UE_LOG(LogTemp, Log, TEXT("Creating B2 for %s"), *name);
			B2 = new FMinMaxBox(FVector(Min.X, dividingCoord, Min.Z), Max, DividingIndex + 1);
			B2->name = FString::Printf(TEXT("%s > B2"), *name);
		}
		B2->AddToTree(box);
		return;
	}

	// object is in between. We need to split and then add object to both branches

	//UE_LOG(LogTemp, Log, TEXT("Object %s needs to be divided (in %s)"), box->containingObject ? *(box->containingObject->GetName()) : TEXT("nothing"), *name);

	FMinMaxBox* newB1 = new FMinMaxBox(box->Min, box->Max, box->DividingIndex);
	newB1->containingObject = box->containingObject;

	box->Max.Y = dividingCoord;
	newB1->Min.Y = dividingCoord;
	addToTreeByY(box, dividingCoord);
	addToTreeByY(newB1, dividingCoord);
}

void FMinMaxBox::addToTreeByZ(FMinMaxBox* box, float dividingCoord) {
	//UE_LOG(LogTemp, Log, TEXT("Box %s has dividingCoord Z %f"), *name, dividingCoord);

	if (box->Max.Z <= dividingCoord)		// whole object is in left plane
	{
		//UE_LOG(LogTemp, Log, TEXT("Box %s is all in left plane of %s"), *(box->name), *name);
		if (!B1)
		{
			//UE_LOG(LogTemp, Log, TEXT("Creating B1 for %s"), *name);
			B1 = new FMinMaxBox(Min, FVector(Max.X, Max.Y, dividingCoord), DividingIndex + 1);
			B1->name = FString::Printf(TEXT("%s > B1"), *name);
		}
		B1->AddToTree(box);
		return;
	}

	if (box->Min.Z >= dividingCoord)		// whole object is in right plane
	{
		//UE_LOG(LogTemp, Log, TEXT("Box %s is all in right plane of %s"), *(box->name), *name);
		if (!B2)
		{
			//UE_LOG(LogTemp, Log, TEXT("Creating B2 for %s"), *name);
			B2 = new FMinMaxBox(FVector(Min.X, Min.Y, dividingCoord), Max, DividingIndex + 1);
			B2->name = FString::Printf(TEXT("%s > B1"), *name);
		}
		B2->AddToTree(box);
		return;
	}

	// object is in between. We need to split and then add object to both branches

	//UE_LOG(LogTemp, Log, TEXT("Object %s needs to be divided (in %s)"), box->containingObject ? *(box->containingObject->GetName()) : TEXT("nothing"), *name);

	FMinMaxBox* newB1 = new FMinMaxBox(box->Min, box->Max, box->DividingIndex);
	newB1->containingObject = box->containingObject;

	box->Max.Z = dividingCoord;
	newB1->Min.Z = dividingCoord;
	addToTreeByZ(box, dividingCoord);
	addToTreeByZ(newB1, dividingCoord);
}


void FMinMaxBox::DEBUGDrawContainingBox(UWorld* world)
{
	if (!world)
		return;


	auto center = (Max + Min) * 0.5;
	auto extend = Max - center;



	auto di = DividingIndex % 3;

	DrawDebugBox(world, center, extend, di == 0 ? FColor::Red : (di == 1 ? FColor::Green : FColor::Blue), true);

	if (SingleChild)
	{
		SingleChild->DEBUGDrawContainingBox(world);
		return;
	}
	if (B1)
		B1->DEBUGDrawContainingBox(world);

	if (B2)
		B2->DEBUGDrawContainingBox(world);


}

