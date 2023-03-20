// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunningManCharacter.generated.h"

UCLASS()
class RUNNINGMANCPP_API ARunningManCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARunningManCharacter();

	/* Объявление переменной, ответственной за штатив камеры персонажа */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* SpringArm;

	/* Объявление переменной, ответственной за камеру персонажа */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* ThirdPersonCamera;

	/* Объявление метода передвижения по оси x */
	void MoveForwardBack(float Axis);

	/* Объявление метода передвижения по оси y */
	void MoveLeftRight(float Axis);

	/* Объявление метода начала Спринта */
	void MoveStartSprint();

	/* Объявление метода начала Спринта */
	void MoveStopSprint();

	/* Объявление переменной, ответственной за проверку на Спринт */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsRunning;

	/* Объявление переменной, ответственной за кол-во выносливости */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fStamina;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
