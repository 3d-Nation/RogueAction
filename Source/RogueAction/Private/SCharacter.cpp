// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"  //from comments to first video in lecture 3
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}




// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FOLLOWING is from notes under first video in Lecture 3
	// ********
	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
	// ********
	//END of code added from notes under first video of lecture 3
}


// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

	//initial jump programming.  Instructor used the ACharacter jump call though (and only on pressed, not on released).
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::BeginJump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCharacter::EndJump);

	//Second Assignment
	// $$$$$$
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &ASCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("TeleportAttack", IE_Pressed, this, &ASCharacter::TeleportAttack);
	//$$$$$$
	//End Second Assignment

	//Instructor used this instead...
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// Unreal axis
	// X = forward (red)
	// Y = right (green)
	// Z = up (blue)


	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{

	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f );

	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);  //would use this to clear the timer if needed (Player dies for example)


}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	if(ensure(ProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FTransform SpawnTM; // = FTransform(GetControlRotation(), HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		//GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams );  //moved down for updates to projectile aim stuff

		// *************************************
		//Aim improvement
		//STUFF BELOW HERE

		FColor LineColor = FColor::Green;

		
		FVector CamLocation = CameraComp->GetComponentLocation();
		FRotator CamRotation = CameraComp->GetComponentRotation();

		//changed from 1000 to 500 and back
		FVector End = CamLocation + (CamRotation.Vector() * 1000);

		FHitResult HitPoint;

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);


		bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(HitPoint, CamLocation, End, ObjectQueryParams);

		FVector NewEnd = bBlockingHit ? HitPoint.ImpactPoint : HitPoint.TraceEnd;  //get a green squiggly underline about a slicer conversion ...  

		FRotator ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation,NewEnd);

		//DrawDebugLine(GetWorld(), HandLocation, NewEnd, FColor::Blue, true, 2.0f, 0, 2.0f);
		//DrawDebugSphere(GetWorld(), NewEnd, 12.0f, 12, FColor::Blue, true, 2.0f);

		SpawnTM = FTransform(ProjectileRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams );


		// ******************************* 
		//END STUFF FOR AIMING

	}


}

void ASCharacter::PrimaryInteract()
{
	if(InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
	
}

/*  //using character class jump instead...
void ASCharacter::BeginJump()
{
	bPressedJump = true;
}

void ASCharacter::EndJump()
{
	bPressedJump = false;
}
 */

void ASCharacter::SecondaryAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::SecondaryAttack_TimeElapsed, 0.2f );

	
}

void ASCharacter::SecondaryAttack_TimeElapsed()
{
	ProjectileAttack(BlackholeProjectile); // SecondaryProjectile
}

void ASCharacter::TeleportAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::TeleportAttack_TimeElapsed, 0.2f );
}

void ASCharacter::TeleportAttack_TimeElapsed()
{
	ProjectileAttack(TeleportProjectile); 
}

void ASCharacter::ProjectileAttack(TSubclassOf<AActor> ProjClass)
{
	if(ensure(ProjClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		FTransform SpawnTM; // = FTransform(GetControlRotation(), HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		//GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams );  //moved down for updates to projectile aim stuff

		// *************************************
		//Aim improvement
		//STUFF BELOW HERE

		FColor LineColor = FColor::Green;

		
		FVector CamLocation = CameraComp->GetComponentLocation();
		FRotator CamRotation = CameraComp->GetComponentRotation();

		//changed from 1000 to 500 and back
		FVector End = CamLocation + (CamRotation.Vector() * 1000);

		FHitResult HitPoint;

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);


		bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(HitPoint, CamLocation, End, ObjectQueryParams);

		FVector NewEnd = bBlockingHit ? HitPoint.ImpactPoint : HitPoint.TraceEnd;  //get a green squiggly underline about a slicer conversion ...  

		FRotator ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation,NewEnd);

		//DrawDebugLine(GetWorld(), HandLocation, NewEnd, FColor::Blue, true, 2.0f, 0, 2.0f);
		//DrawDebugSphere(GetWorld(), NewEnd, 12.0f, 12, FColor::Blue, true, 2.0f);

		SpawnTM = FTransform(ProjectileRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjClass, SpawnTM, SpawnParams );


		// ******************************* 
		//END STUFF FOR AIMING

	}

	//else
	//{
	//	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	//	FString textString = FString::Printf(TEXT("Stuff")) ;
	//	DrawDebugString(GetWorld(), HandLocation, textString, nullptr, FColor::Green, 3.0f, true);

		//DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
	//}

	
}



