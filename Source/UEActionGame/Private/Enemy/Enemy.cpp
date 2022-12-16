// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AIController.h"
#include "Items/Weapons/Weapon.h"

#include "UEActionGame/DebugMacros.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMeshComponent* EnemyMesh = this->GetMesh();
	EnemyMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	EnemyMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	// Prevent camera from zooming in when collided with enemy
	EnemyMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	EnemyMesh->SetGenerateOverlapEvents(true);

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(CurrentPatrolTarget);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AEnemy::Die()
{
	PlayDeathMontage();
	GetWorldTimerManager().ClearTimer(AttackTimer);
	EnemyState = EEnemyState::EES_Dead;
	DisableCapsule();
}

bool AEnemy::IsInTargetRange(AActor* Target, double AcceptanceRadius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - this->GetActorLocation()).Size();
	DRAW_SPHERE_SingleFrame(this->GetActorLocation());
	DRAW_SPHERE_SingleFrame(Target->GetActorLocation());
	return DistanceToTarget <= AcceptanceRadius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(50.f);

	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (auto Target : PatrolTargets)
	{
		if (Target != CurrentPatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const auto RandomTargetIndex = FMath::RandRange(0, ValidTargets.Num() - 1);
	return ValidTargets[RandomTargetIndex];
}

void AEnemy::Attack()
{
	Super::Attack();
	PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
	return IsInTargetRange(CombatTarget, AttackRadius) &&
		EnemyState != EEnemyState::EES_Attacking &&
		EnemyState != EEnemyState::EES_Dead;
}

int32 AEnemy::PlayDeathMontage()
{
	const int32 Index = Super::PlayDeathMontage();
	TEnumAsByte<EDeathPose> Pose(Index);
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Index;
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChase =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking&&
		SeenPawn->ActorHasTag(FName("PlayerCharacter"));

	if (bShouldChase)
	{
		CombatTarget = SeenPawn;
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		ChaseTarget();
	}
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(CurrentPatrolTarget);
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	MoveToTarget(CurrentPatrolTarget);
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	MoveToTarget(CombatTarget);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackDelay = FMath::RandRange(AttacDelaykMin, AttackDelayMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackDelay);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState == EEnemyState::EES_Dead) return;
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (IsInTargetRange(CurrentPatrolTarget, PatrolRadius))
	{
		CurrentPatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(5.f, 10.f);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

void AEnemy::CheckCombatTarget()
{
	if (!IsInTargetRange(CombatTarget, CombatRadius))
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		CombatTarget = nullptr;
		if(EnemyState != EEnemyState::EES_Engaged)
			StartPatrolling();
	}
	else if (!IsInTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		ChaseTarget();
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	if (IsAlive())
		DirectionalHitReact(ImpactPoint);
	else
		Die();

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, 
	AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	ChaseTarget();
	return DamageAmount;
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
}

