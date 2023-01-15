// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USInteractionComponent;
class UAnimMontage;

UCLASS()
class ROGUEACTION_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	//Assignment 2 additions

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> BlackholeProjectile; // SecondaryProjectile
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> TeleportProjectile;
	
	//End assignment 2 additions

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;



public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void PrimaryAttack();

	void PrimaryAttack_TimeElapsed();

	//Assignment 2 additions
	// %%%%%%%%%%%%
	void SecondaryAttack();

	void SecondaryAttack_TimeElapsed();

	void TeleportAttack();

	void TeleportAttack_TimeElapsed();

	void ProjectileAttack(TSubclassOf<AActor> ProjClass);
	// %%%%%%%%%%%%
	//End assignment 2 additions

	void PrimaryInteract();

	void BeginJump();
	void EndJump();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
