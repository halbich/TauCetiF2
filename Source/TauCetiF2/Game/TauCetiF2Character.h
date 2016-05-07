// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Components/SelectorComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/BuilderComponent.h"
#include "Game/TauCetiF2PlayerController.h"
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

public:
	ATauCetiF2Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	virtual void BecomeViewTarget(APlayerController* PC) override;

protected:

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

	UPROPERTY()
		ATauCetiF2PlayerController* PC;   // todo pøesunout?

	void OnUse();

	UFUNCTION(BlueprintCallable, Category = Character)
	void LoadFromCarrier(USaveGameCarrier* carrier);

	UFUNCTION(BlueprintCallable, Category = Character)
		void SaveToCarrier(USaveGameCarrier* carrier);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

