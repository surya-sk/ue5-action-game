// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Items/Weapons/Weapon.h"
#include "Characters/MainCharacter.h"
#include "NavigationSystem.h"


// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMeshComponent* EnemyMesh = this->GetMesh();
	EnemyMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	EnemyMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	// Prevent camera from zooming in when collided with enemy
	EnemyMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	EnemyMesh->SetGenerateOverlapEvents(true);

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	TorchPawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("TorchPawnSensing"));

	AssassinationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AssassinationBox"));
	AssassinationBox->SetupAttachment(EnemyMesh);

	NearbyEnemyTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("NearbyEnemyTrigger"));
	NearbyEnemyTrigger->SetupAttachment(EnemyMesh);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

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
// Called when the game starts or when spawned
void AEnemy::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit(ImpactPoint, Hitter);
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	GetWorldTimerManager().ClearTimer(AttackTimer);
	SetWeaponCollision(ECollisionEnabled::NoCollision);
	StopAttackMontage();
}

void AEnemy::PlayAssassinationMontage()
{
	EnemyState = EEnemyState::EES_Dead;
	DisableCapsule();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AssassinationMontage)
	{
		AnimInstance->Montage_Play(AssassinationMontage);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, 
	AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	if (IsInTargetRange(CombatTarget->GetActorLocation(), AttackRadius))
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (!IsInTargetRange(CombatTarget->GetActorLocation(), AttackRadius))
	{
		ChaseTarget();
	}
	ChaseTarget();
	return DamageAmount;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AssassinationBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnBoxOverlap);
	AssassinationBox->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnBoxEndOverlap);

	NearbyEnemyTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnSphereOverlap);
	NearbyEnemyTrigger->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnSphereEndOverlap);
	
	EnemyController = Cast<AAIController>(GetController());
	CurrentPatrolTarget = ChoosePatrolTarget();
	MoveToTarget(CurrentPatrolTarget);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	if (TorchPawnSensing)
	{
		TorchPawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::TorchSeen);
	}

	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
	Tags.Add(FName(TEXT("Enemy")));
}

void AEnemy::Die()
{
	Super::Die();
	GetWorldTimerManager().ClearTimer(AttackTimer);
	EnemyState = EEnemyState::EES_Dead;
	DisableCapsule();
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponCollision(ECollisionEnabled::NoCollision);
	if (OnEnemyKilled.IsBound())
	{
		OnEnemyKilled.Broadcast();
	}
}

void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
	return IsInTargetRange(CombatTarget->GetActorLocation(), AttackRadius) &&
		EnemyState != EEnemyState::EES_Attacking &&
		EnemyState != EEnemyState::EES_Engaged &&
		EnemyState != EEnemyState::EES_Dead;
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		Character->SetEnemyToAssassinate(this);
	}
}

void AEnemy::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMainCharacter* Character = Cast<AMainCharacter>(OtherActor);
	if (Character)
	{
		Character->SetEnemyToAssassinate(nullptr);
	}
}

void AEnemy::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* NearbyEnemy = Cast<AEnemy>(OtherActor);
	if (NearbyEnemy)
	{
		NearbyEnemies.AddUnique(NearbyEnemy);
	}
}

void AEnemy::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEnemy* NearbyEnemy = Cast<AEnemy>(OtherActor);
	if (NearbyEnemy)
	{
		NearbyEnemies.Remove(NearbyEnemy);
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
	if (CombatTarget == nullptr) return;
	if (!IsInTargetRange(CombatTarget->GetActorLocation(), CombatRadius))
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		CombatTarget = nullptr;
		if (EnemyState != EEnemyState::EES_Engaged)
			StartPatrolling();
	}
	else if (!IsInTargetRange(CombatTarget->GetActorLocation(), AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		GetWorldTimerManager().ClearTimer(AttackTimer);
		ChaseTarget();
	}
	else if (CanAttack())
	{
		StartAttackTimer();
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
	MoveToTarget(CombatTarget->GetActorLocation());
}

bool AEnemy::IsInTargetRange(FVector Target, double AcceptanceRadius)
{
	if (Target == FVector::ZeroVector) return false;
	const double DistanceToTarget = (Target - this->GetActorLocation()).Size();
	return DistanceToTarget <= AcceptanceRadius;
}

void AEnemy::MoveToTarget(FVector Target)
{
	if (EnemyController == nullptr) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Target);
	MoveRequest.SetAcceptanceRadius(50.f);

	EnemyController->MoveTo(MoveRequest);
}

FVector AEnemy::ChoosePatrolTarget()
{
	if (PatrolTargets.Num() > 0)
	{
		TArray<AActor*> ValidTargets;
		for (auto Target : PatrolTargets)
		{
			if (Target->GetActorLocation() != CurrentPatrolTarget)
			{
				ValidTargets.AddUnique(Target);
			}
		}

		const auto RandomTargetIndex = FMath::RandRange(0, ValidTargets.Num() - 1);
		return ValidTargets[RandomTargetIndex]->GetActorLocation();
	}
	else
	{
		return UNavigationSystemV1::GetRandomReachablePointInRadius(GetWorld(), GetActorLocation(), ReachablePatrolRadius);
	}
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChase =
		EnemyState != EEnemyState::EES_Dead &&
		//EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking&&
		SeenPawn->ActorHasTag(FName("PlayerCharacter"));

	if (bShouldChase)
	{
		CombatTarget = SeenPawn;
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		ChaseTarget();
		AlertNearbyEnemies();
	}
}

void AEnemy::TorchSeen(APawn* SeenPawn)
{
	const bool bShouldChase = EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking&&
		SeenPawn->ActorHasTag(FName("Torch")) && CombatTarget == nullptr;
	if (bShouldChase)
	{
		UE_LOG(LogTemp, Warning, TEXT("Start chase torch"));
		CombatTarget = SeenPawn;
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		ChaseTarget();
	}
}

void AEnemy::AlertNearbyEnemies()
{
	if (NearbyEnemies.Num() == 0) return;

	for (AEnemy* Enemy : NearbyEnemies)
	{
		Enemy->CombatTarget = CombatTarget;
		Enemy->ChaseTarget();
	}
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackDelay = FMath::RandRange(AttacDelayMin, AttackDelayMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackDelay);
}




