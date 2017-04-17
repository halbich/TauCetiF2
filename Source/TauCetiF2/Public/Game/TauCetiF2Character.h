// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Components/SelectorComponent.h"
#include "Inventory/Public/InventoryComponent.h"
#include "Components/BuilderComponent.h"
#include "Game/TauCetiF2PlayerController.h"
#include "Blocks/Public/Definitions/OxygenComponentDefinition.h"
#include "Blocks/Public/Definitions/ElectricityComponentDefinition.h"
#include "Blocks/Public/Components/OxygenComponent.h"
#include "Blocks/Public/Components/ElectricityComponent.h"
#include "GameSave/Public/SaveGameCarrier.h"
#include "Blocks/Public/Helpers/BlockHelpers.h"
#include "TauCetiF2Character.generated.h"

UCLASS(config = Game)
class ATauCetiF2Character : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Selector, meta = (AllowPrivateAccess = "true"))
		class USelectorComponent* Selector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* Inventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
		class UBuilderComponent* Builder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
		class UOxygenComponent* OxygenComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
		class UElectricityComponent* ElectricityComponent;

public:
	ATauCetiF2Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	virtual void BecomeViewTarget(APlayerController* pc) override;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | OxygenComponent", meta = (ShowOnlyInnerProperties))
		FOxygenComponentDefinition OxygenDef;

	UPROPERTY(EditDefaultsOnly, Category = "TCF2 | ElectricityComponent", meta = (ShowOnlyInnerProperties))
		FElectricityComponentDefinition ElectricityDef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TCF2 | GameMode")
		bool IsInCreativeMode;

	void doCharacterHit(float intensity);

protected:

	void BeginPlay();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void BuilderDoAction();

	void BuilderRotatePitch(float Value);
	void BuilderRotateRoll(float Value);
	void BuilderRotateYaw(float Value);

	bool lastPitchWasZero;
	bool lastRollWasZero;
	bool lastYawWasZero;

	void OnEscapeKey();
	void OnEnterKey();

	void OnInventory();

	void OnNextInventoryBank();
	void OnPrevInventoryBank();
	void OnEmptyHand();

	void toogleCreative(bool isCreative);
	void OnToggleCreativeMode();

	UPROPERTY()
		ATauCetiF2PlayerController* PC;   // todo přesunout?

	void OnUse();

	UFUNCTION(BlueprintCallable, Category = Character)
		void LoadFromCarrier(USaveGameCarrier* carrier, TArray<FText>& validationErrors);

	UFUNCTION(BlueprintCallable, Category = Character)
		void SaveToCarrier(USaveGameCarrier* carrier);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* inputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class UInventoryComponent* GetInventory() const { return Inventory; }
};
