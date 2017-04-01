#pragma once

#include "Components/ActorComponent.h"
#include "Game/Weather/WeatherSavingHelpers.h"
#include "Blocks/Public/Tree/WeatherTargetsKDTree.h"
#include "GameSave/Public/SaveGameCarrier.h"
#include "GameWeatherComponent.generated.h"



UENUM(BlueprintType)
enum class EStormState : uint8
{
	NoStorm	 	UMETA(DisplayName = "NoStorm"),
	EaseIn		UMETA(DisplayName = "EaseIn"),
	Running		UMETA(DisplayName = "Running"),
	EaseOut		UMETA(DisplayName = "EaseOut")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TAUCETIF2_API UGameWeatherComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGameWeatherComponent();

	UPROPERTY(BlueprintReadWrite, Transient, Category = "TCF2 | GameWeatherComponent")
		UCurrentWeatherState* currentWeatherState;

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameWeatherComponent")
		void LoadFromCarrier(USaveGameCarrier* carrier, TArray<FText>& validationErrors);

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameWeatherComponent")
		void SaveToCarrier(USaveGameCarrier* carrier);

	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | GameWeatherComponent")
		UWeatherTargetsKDTree* WeatherRootTree;


	UPROPERTY(Transient)
		bool debugBoxesShown;

#pragma region StormMechanics

	UPROPERTY(EditDefaultsOnly, Category = Curve)
		UCurveFloat* IntensityCurve;

	// Target hits per sq. meter per second
	UPROPERTY(Transient)
		float CurrentHitIntensity;

	UPROPERTY(Transient)
		float hitpointsCounter;

	UPROPERTY(Transient)
		float currentEasingTime;

	UPROPERTY(Transient)
		float currentEaseInTime;

	UPROPERTY(Transient)
		float currentEaseOutTime;

	UPROPERTY(Transient)
		EStormState StormState;


#pragma endregion

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameWeatherComponent")
		void DEBUGShowMinMaxBoxes();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameWeatherComponent")
		void DEBUGHideMinMaxBoxes();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameWeatherComponent")
		void OnStormBegin();

	UFUNCTION(BlueprintCallable, Category = "TCF2 | GameWeatherComponent")
		void OnStormEnd();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ObjectsChanged();


	UPROPERTY(BlueprintReadOnly, Transient, Category = "TCF2 | GameWeatherComponent")
		TArray<UWeatherTargetsKDTree*> Targets;

	UFUNCTION()
		void OnTargetElementsChanged(UWeatherTargetsKDTree* target, bool isAdding);


	void InitComp();

private:

	FDelegateHandle ListeningHandle;




#pragma optimize("", off)

	void doDamage(int32 currentHitPoints) {

		auto targNum = Targets.Num();

		if (targNum == 0)
			return;

		for (auto i = 0; i < currentHitPoints; i++)
		{
			auto target = Targets[FMath::RandHelper(targNum)];
			check(target && target->IsValidLowLevelFast() && !target->IsPendingKill() && target->ChildHeap.Num() > 0);

			auto targObj = target->ChildHeap.HeapTop();
			check(targObj && targObj->IsValidLowLevelFast() && !targObj->IsPendingKill());
			check(targObj->ContainingObject && targObj->ContainingObject->IsValidLowLevelFast() && !targObj->ContainingObject->IsPendingKill());

			auto bl = Cast<ABlock>(targObj->ContainingObject);
			check(bl && bl->IsValidLowLevelFast() && !bl->IsPendingKill());

			auto defBox = bl->WorldObjectComponent->DefiningBox;
			auto myBox = targObj;

			auto v = ((myBox->Min - defBox->Min) / GameDefinitions::CubeMinSize).GridSnap(1);

			auto norm = FMath::RoundToInt(bl->BlockInfo->Rotation.GetNormalized().Yaw + 360) % 360;

			auto hitStorm = FVector(v);
			auto rotSc = bl->BlockInfo->Rotation.RotateVector(bl->BlockInfo->Scale - FVector(1, 1, 1)).GridSnap(1);
			switch (norm)
			{
			case 0: break;
			case 90: {
				auto mrsrc = (-1) * rotSc;
				hitStorm = FVector(hitStorm.Y, mrsrc.X - hitStorm.X, 0);
				break;
			}
			case 180: {
				hitStorm = (-1) * rotSc - hitStorm; break;
			}
			case 270: {
				auto mrsrc = (-1) * rotSc;
				hitStorm = FVector( mrsrc.Y - hitStorm.Y ,hitStorm.X , 0);
				break;
			}
			default:
				checkNoEntry();
			}


			bl->WasHitByStorm(hitStorm);

			targObj->DEBUGDrawBorder(GetWorld(), FColor::Orange, 0.75f);
		}

	}


#pragma optimize("", on)

	static const float OneSixth;

};
