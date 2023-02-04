// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

//class USAttributeComponent;  //if error, could use keyword class in front of USAttributeComponent in our Declare Dynamic statement below...

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor,  USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUEACTION_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USAttributeComponent();

protected:

	// EditAnywhere - edit in BP editor and per-instance in level.
	// VisibleAnywhere - 'read only' in editor and level.  (use for components)
	// EditDefaultsOnly - hide variable per-instance, edit in BP editor only
	// VisibleDefaultsOnly - 'read only' access for variable, only in BP Editor (uncommon)
	// EditInstanceOnly - allow only editing of instance (eg. when placed in level)
	// --
	// BlueprintReadOnly - read-only in the Blueprint scripting (does not affect 'details' panel)
	// BlueprintReadWrite - read-write access in blueprints
	// --
	// Category = "" - display only for detail panels and blueprint context menu.



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

	//  Other attributes we might use in the future include...
	//  HealthMax, Stamina, Strength



public:	

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Delta);

		
};
