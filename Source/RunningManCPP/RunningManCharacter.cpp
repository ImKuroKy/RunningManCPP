// Fill out your copyright notice in the Description page of Project Settings.


#include "RunningManCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ARunningManCharacter::ARunningManCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* ��������� ������� ������� */
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	/* �������� � ��������� SpringArm */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true;

	/* �������� � ��������� ThirdPersonCamera */
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Third Person Camera"));
	ThirdPersonCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;

	/* ��������� ���������� ����������� */
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	/* ������������� ����������, ������������� �� �������� �� ������ */
	bIsRunning = false;
	
	/* ������������� ����������, ������������� �� ���-�� ������������ */
	fStamina = 100.0f;

}

// Called when the game starts or when spawned
void ARunningManCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARunningManCharacter::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	/* �������� �� �������. ���� �������, �������� ������������ ������������� */
	if ((bIsRunning == true) && (fStamina > 0.0f))
	{
		fStamina -= 2.0f;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else if (fStamina == 0.0f) // ���� �� �������, �������� ������������ � ��������
	{
		bIsRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
	
	/* �������� �� �������������� ������� */
	if ((bIsRunning == false) && (fStamina < 100.0f))
	{
		fStamina += 2.0f;
	}
	
}

// Called to bind functionality to input
void ARunningManCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* ��������� ���������� ������ */
	PlayerInputComponent->BindAxis("Move_CameraLeftRight", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Move_CameraUpDown", this, &APawn::AddControllerPitchInput);

	/* ��������� ���������� ���������� */
	PlayerInputComponent->BindAxis("Move_ForwardBack", this, &ARunningManCharacter::MoveForwardBack);
	PlayerInputComponent->BindAxis("Move_LeftRight", this, &ARunningManCharacter::MoveLeftRight);

	/* ���������� ������� */
	PlayerInputComponent->BindAction("Move_Sprint", IE_Pressed, this, &ARunningManCharacter::MoveStartSprint);
	PlayerInputComponent->BindAction("Move_Sprint", IE_Released, this, &ARunningManCharacter::MoveStopSprint);

}

/* ����� �������� ����� - ����� */ 
void ARunningManCharacter::MoveForwardBack(float Axis)
{
	/* �������� �� ������� ����������� ������ ������ � ������������ */ 
	if (Controller != NULL)
	{
		/* ��������� ���� ������ �� ��� Yaw */
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		/* ������� ������� ����������� �������� */
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		/* �������� �������� */
		AddMovementInput(Direction, Axis);
	}
}

/* ����� �������� ����� - ������ */
void ARunningManCharacter::MoveLeftRight(float Axis)
{
	/* �������� �� ������� ����������� ������ ������ � ������������ */
	if (Controller != NULL)
	{
		/* ��������� ���� ������ �� ��� Yaw */
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		/* ������� ������� ����������� �������� */
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		/* �������� �������� */
		AddMovementInput(Direction, Axis);
	}
}

/* ����� ��������� �������� ��������� ��� ������� ������� ������� */
void ARunningManCharacter::MoveStartSprint()
{
	/* �������� �� ������� ����������� ������ ������ � ������������ */
	if ((Controller != NULL))
	{
		// �������� �� ������� �������
		if (fStamina > 0.0f)
		{
			bIsRunning = true;	
		}
	}
}

/* ����� ��������� �������� ��������� ��� ������� ������� ������� */
void ARunningManCharacter::MoveStopSprint()
{
	/* �������� �� ������� ����������� ������ ������ � ������������ */
	if (Controller != NULL)
	{
		bIsRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}