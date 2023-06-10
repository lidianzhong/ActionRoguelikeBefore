// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//�������
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");


	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(float value)
{
	// Direction in world space to apply input
	// GetActorForwardVector() means get the forward (X) vector (length 1.0) from this Actor, in world space
	// AddMovementInput(Fvector WorldDirection, float ScaleValue);
	// AddMovementInput(GetActorForwardVector(), value);

	// not to use GetActorForwardVector,to use Control's vector
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), value);

}

void ASCharacter::MoveRight(float value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, value);
}




// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//���ð���
	//PlayerInputComponent->BindAxis(��������, Ӧ�ö���, ӳ�亯��); ʹ��BindAxis����Ϊ������
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	//������ת���ӳ�亯��ʱ��ֱ��ʹ��AddControllerYawInput�������
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//����Magic����
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	
	// ���������ӵĻ���
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

	// Jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

}


void ASCharacter::PrimaryAttack()
{
	// play animmontage about attack
	PlayAnimMontage(AttackAnim);

	// generate a timer
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

	// clear a timer
	//GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);

}



void ASCharacter::PrimaryAttack_TimeElapsed()
{

	if (ensureAlways(ProjectileClass)) {
		// get generate actor location
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		// GetControlRotation() : Get the rotation of the Controller, often the 'view' ratation of this Pawn
		FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		// SpawnActor : generate actor in the world
		// First parameter : ָ����Ҫ���ɵ�Actor��
		// Second parameter : �任(λ��,��ת,����)
		// Third parameter : ���ԣ�
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);

		//DrawDebugSphere(GetWorld(), HandLocation, 10.0f, 16, FColor::Red, false, 2.0f);
	}


}


void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}
