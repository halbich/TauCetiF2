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

	UPROPERTY(Transient)
		float CurrentHitIntensity;

	UPROPERTY(Transient)
		float hitpointsCounter;

	UPROPERTY(Transient)
		float currentEasingTime;

	UPROPERTY(EditDefaultsOnly, Category = Curve)
		float EntryEasingTime;

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

	void doDamage(int targets) {
		print(TEXT("Doing damage"));
	}

};
