// Fill out your copyright notice in the Description page of Project Settings.


#include "HazardousBarrel.h"
#include  "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
AHazardousBarrel::AHazardousBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	//MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AHazardousBarrel::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->OnComponentHit.AddDynamic(this, &AHazardousBarrel::HitBarrel);
}

// Called every frame
void AHazardousBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AHazardousBarrel::HitBarrel( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hazardous Barrel HitBarrel"));

	ForceComp->FireImpulse();

	UE_LOG(LogTemp, Warning, TEXT("Hazardous Barrel anything??"));
}
