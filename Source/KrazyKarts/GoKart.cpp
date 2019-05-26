// Fill out your copyright notice in the Description page of Project Settings.

#include "GoKart.h"

#include "WheeledVehicle.h"
#include "WheeledVehicleMovementComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "SimpleWheeledVehicleMovementComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Root = CreateDefaultSubobject<USceneComponent>("Root");
	//SetRootComponent(Root);

	//Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VehicleMesh"));
	//Mesh->SetupAttachment(Root);

	//// Create a spring arm component
	//SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	//SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	//SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	//SpringArm->SetupAttachment(Root);
	//SpringArm->TargetArmLength = 600.0f;
	//SpringArm->bEnableCameraRotationLag = true;
	//SpringArm->CameraRotationLagSpeed = 7.f;
	//SpringArm->bInheritPitch = false;
	//SpringArm->bInheritRoll = false;

	//// Create camera component 
	//Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	//Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	//Camera->bUsePawnControlRotation = false;
	//Camera->FieldOfView = 90.f;
}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAirResistance();
	ApplyTurningForce();
	AddForces();
	UpdateLocation();
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);
}

void AGoKart::AddForces()
{
	FVector Acceleration = ForwardForce / Mass;
	Velocity += Acceleration;
	Velocity += AirResistance;
}

void AGoKart::ApplyTurningForce()
{
	AddActorWorldRotation(TurnForce, true);
	Velocity = TurnForce.RotateVector(Velocity);
}

void AGoKart::UpdateLocation()
{

	FHitResult HitResult = FHitResult();
	AddActorWorldOffset(Velocity, true, &HitResult);

	if (HitResult.bBlockingHit)
	{
		Velocity = FVector(0, 0, 0);
	}
}

void AGoKart::UpdateAirResistance()
{
	AirResistance = FMath::Pow(Velocity.Size(), 2) * (Velocity.GetSafeNormal() * -1) * AirResistanceCoeficient;
}

void AGoKart::MoveForward(float Val)
{
	ForwardForce = GetActorForwardVector().GetSafeNormal() * ForwardForceMagnitud * Val;
}

void AGoKart::MoveRight(float Val)
{
	TurnForce = FQuat(GetActorUpVector(), FMath::DegreesToRadians(RightForceMagnitud) * Val);
}

