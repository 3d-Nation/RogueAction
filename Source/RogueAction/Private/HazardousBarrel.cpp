// Fill out your copyright notice in the Description page of Project Settings.


#include "HazardousBarrel.h"

#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
AHazardousBarrel::AHazardousBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");

	//Original....but not in instructors solution...
	//MeshComp->SetCollisionObjectType(ECC_PhysicsBody);


	MeshComp->SetSimulatePhysics(true);  //Originally commented out but in there (not used)
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

	//Additions from instructor solution
	//
	//Leaving this on applies small constant force via component 'tick'
	ForceComp->SetAutoActivate(false);

	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 2500.0f;  //alternative:  200000.0f if bImpulseVelChange = false
	//optional, ignores mass of other objects (if false, the impulse strength will be much higher to push most objects depending on mass)
	ForceComp->bImpulseVelChange = true;

	//optional, default constructor of component already adds 4 object types to affect, excluding WorldDynamic
	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
	//
	//End of additions from instructor solution
}

// Called when the game starts or when spawned
void AHazardousBarrel::BeginPlay()
{
	Super::BeginPlay();

	//Instructor does this in PostInitializeComponents() function instead...
	//MeshComp->OnComponentHit.AddDynamic(this, &AHazardousBarrel::HitBarrel);
}

void AHazardousBarrel::PostInitializeComponents()
{
	//Don't forget to call parent function
	Super::PostInitializeComponents();

	MeshComp->OnComponentHit.AddDynamic(this, &AHazardousBarrel::HitBarrel);
}

// Called every frame
void AHazardousBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AHazardousBarrel::HitBarrel( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp->FireImpulse();

	UE_LOG(LogTemp, Warning, TEXT("HitBarrel in Explosive Barrel"));

	// for the TEXT macro when adding variables to the string...
	// %f = float
	// %s = string (typically (always??) needs * in front of variable (assuming we are de-referencing...
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	
	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);

}
