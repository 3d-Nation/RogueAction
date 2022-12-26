// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"

#include "Components/CapsuleComponent.h"
#include  "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComp");
	CapsuleComp->SetCollisionObjectType(ECC_PhysicsBody);

	//Trial Start
	//**should this be in beginplay rather than contstructor???
	//CapsuleComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::HitBarrel);
	//Trial End
	RootComponent = CapsuleComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(CapsuleComp);

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(CapsuleComp);

}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	//moved from constructor to see if it will work here...Register hit event...
	CapsuleComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::HitBarrel);
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveBarrel::HitBarrel( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	UE_LOG(LogTemp, Warning, TEXT("ExplosiveBarrel Capsule Hit"));

	ForceComp->FireImpulse();
}