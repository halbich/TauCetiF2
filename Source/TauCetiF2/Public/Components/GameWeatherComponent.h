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

	UPROPERTY(Transient)
		ACharacter* playerCharacter;

	UPROPERTY(Transient)
		float startPointZ;

#pragma region StormMechanics

	UPROPERTY(EditDefaultsOnly, Category = Curve)
		UCurveFloat* IntensityCurve;

	// Target hits per sq. meter per second
	UPROPERTY(Transient)
		float CurrentHitIntensity;

	UPROPERTY(Transient)
		float hitpointsCounter;

	UPROPERTY(Transient)
		float playerHitpointCounter;

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

	friend void tryDoCharacterHit(AActor* actor, float intensity);

private:

	FDelegateHandle ListeningHandle;

	FORCEINLINE void doDamage(int32 currentHitPoints) {
		for (auto i = 0; i < currentHitPoints; i++)
		{
			auto targNum = Targets.Num();
			if (targNum == 0)
				return;

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
				hitStorm = FVector(mrsrc.Y - hitStorm.Y, hitStorm.X, 0);
				break;
			}
			default:
				checkNoEntry();
			}

			// TODO value
			bl->WasHitByStorm(hitStorm, 1);
		}
	}

	FORCEINLINE bool ensurePC()
	{
		if (playerCharacter && playerCharacter->IsValidLowLevel())
			return true;

		playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		return playerCharacter != NULL;
	}

	FORCEINLINE void doPlayerDamage(int32 currentHitPoints) {
		if (!ensurePC())
			return;

		auto w = GetWorld();

		FHitResult result;
		FCollisionObjectQueryParams params;

		params.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		params.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		params.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

		for (auto i = 0; i < currentHitPoints; i++)
		{
			auto origLoc = playerCharacter->GetActorLocation();
			FBox b(origLoc - FVector(40, 40, 5), origLoc + FVector(40, 40, 5));

			auto loc = FMath::RandPointInBox(b) / GameDefinitions::CubeMinSize;
			auto target = FVector(FMath::FloorToInt(loc.X), FMath::FloorToInt(loc.Y), FMath::FloorToInt(loc.Z)) * GameDefinitions::CubeMinSize;
			auto start = FVector(target);
			start.Z += startPointZ;

			if (w->LineTraceSingleByObjectType(result, start, target, params))
			{
				auto act = Cast<ABlock>(result.GetActor());
				if (act)
					continue;

				tryDoCharacterHit(result.GetActor(), 5);
			}
		}
	}

	static const float EasingBorderValue;
};
