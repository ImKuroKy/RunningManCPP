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

	/* Установка размера капсулы */
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	/* Создание и настройка SpringArm */
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true;

	/* Создание и настройка ThirdPersonCamera */
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Third Person Camera"));
	ThirdPersonCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;

	/* Настройка компонента перемещения */
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	/* Инициализация переменной, ответственной за проверку на Спринт */
	bIsRunning = false;
	
	/* Инициализация переменной, ответственной за кол-во Выносливости */
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
	/* Проверка на стамину. Если хватает, скорость передвижения увеличивается */
	if ((bIsRunning == true) && (fStamina > 0.0f))
	{
		fStamina -= 2.0f;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else if (fStamina == 0.0f) // Если не хватает, скорость возвращается к исходной
	{
		bIsRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
	
	/* Проверка на восстановление стамины */
	if ((bIsRunning == false) && (fStamina < 100.0f))
	{
		fStamina += 2.0f;
	}
	
}

// Called to bind functionality to input
void ARunningManCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Настройка управления камеры */
	PlayerInputComponent->BindAxis("Move_CameraLeftRight", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Move_CameraUpDown", this, &APawn::AddControllerPitchInput);

	/* Настройка управления персонажем */
	PlayerInputComponent->BindAxis("Move_ForwardBack", this, &ARunningManCharacter::MoveForwardBack);
	PlayerInputComponent->BindAxis("Move_LeftRight", this, &ARunningManCharacter::MoveLeftRight);

	/* Напстройка Спринта */
	PlayerInputComponent->BindAction("Move_Sprint", IE_Pressed, this, &ARunningManCharacter::MoveStartSprint);
	PlayerInputComponent->BindAction("Move_Sprint", IE_Released, this, &ARunningManCharacter::MoveStopSprint);

}

/* Метод движения вперёд - назад */ 
void ARunningManCharacter::MoveForwardBack(float Axis)
{
	/* Проверку на наличие контроллера просят делать в документации */ 
	if (Controller != NULL)
	{
		/* Получение угла камеры по оси Yaw */
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		/* Рассчёт вектора направления движения */
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		/* Создание движения */
		AddMovementInput(Direction, Axis);
	}
}

/* Метод движения влево - вправо */
void ARunningManCharacter::MoveLeftRight(float Axis)
{
	/* Проверку на наличие контроллера просят делать в документации */
	if (Controller != NULL)
	{
		/* Получение угла камеры по оси Yaw */
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		/* Рассчёт вектора направления движения */
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		/* Создание движения */
		AddMovementInput(Direction, Axis);
	}
}

/* Метод изменения скорости персонажа при нажатии клавиши Спринта */
void ARunningManCharacter::MoveStartSprint()
{
	/* Проверку на наличие контроллера просят делать в документации */
	if ((Controller != NULL))
	{
		// Проверка на наличие стамины
		if (fStamina > 0.0f)
		{
			bIsRunning = true;	
		}
	}
}

/* Метод изменения скорости персонажа при отжатии клавиши Спринта */
void ARunningManCharacter::MoveStopSprint()
{
	/* Проверку на наличие контроллера просят делать в документации */
	if (Controller != NULL)
	{
		bIsRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}