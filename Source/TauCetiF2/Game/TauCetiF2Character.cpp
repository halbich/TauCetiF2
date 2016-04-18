// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TauCetiF2.h"
#include "TauCetiF2Character.h"

//////////////////////////////////////////////////////////////////////////
// ATauCetiF2Character

ATauCetiF2Character::ATauCetiF2Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	Selector = CreateDefaultSubobject<USelectorComponent>(TEXT("Selector"));
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	Builder = CreateDefaultSubobject<UBuilderComponent>(TEXT("Builder"));
	Builder->selector = Selector;

	lastPitchWasZero = false;
	lastRollWasZero = false;
	lastYawWasZero = false;
}


//////////////////////////////////////////////////////////////////////////
// Input

void ATauCetiF2Character::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &ATauCetiF2Character::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATauCetiF2Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ATauCetiF2Character::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ATauCetiF2Character::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &ATauCetiF2Character::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ATauCetiF2Character::TouchStopped);

	InputComponent->BindAction("LmbClick", IE_Pressed, this, &ATauCetiF2Character::BuilderDoAction);

	InputComponent->BindAxis("RotPitch", this, &ATauCetiF2Character::BuilderRotatePitch);
	InputComponent->BindAxis("RotRoll", this, &ATauCetiF2Character::BuilderRotateRoll);
	InputComponent->BindAxis("RotYaw", this, &ATauCetiF2Character::BuilderRotateYaw);
}


void ATauCetiF2Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void ATauCetiF2Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void ATauCetiF2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATauCetiF2Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATauCetiF2Character::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATauCetiF2Character::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATauCetiF2Character::BuilderDoAction()
{
	Builder->DoAction();
}

void ATauCetiF2Character::BuilderRotatePitch(float Value)
{
	if (Value != 0.0f && lastPitchWasZero)
		Builder->RotatePitch(Value);

	lastPitchWasZero = Value == 0.0f;
}
void ATauCetiF2Character::BuilderRotateRoll(float Value)
{
	if (Value != 0.0f && lastRollWasZero)
		Builder->RotateRoll(Value);

	lastRollWasZero = Value == 0.0f;
}
void ATauCetiF2Character::BuilderRotateYaw(float Value)
{
	if (Value != 0.0f && lastYawWasZero)
		Builder->RotateYaw(Value);

	lastYawWasZero = Value == 0.0f;
}