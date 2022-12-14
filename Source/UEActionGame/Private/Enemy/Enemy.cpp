// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UEActionGame/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "AIController.h"


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

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

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
	if (EnemyController && CurrentPatrolTarget)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(CurrentPatrolTarget);
		MoveRequest.SetAcceptanceRadius(15.f);
		FNavPathSharedPtr NavPath;
		EnemyController->MoveTo(MoveRequest, &NavPath);
		TArray<FNavPathPoint>& PathPoints =  NavPath->GetPathPoints();
		for (auto& Point : PathPoints)
		{
			const FVector& Location = Point.Location;
			DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
		}
	}
}

void AEnemy::PlayHitReactMontage(const FName SectionName)
{
	UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = this->GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		
		const int32 RandomSelection = FMath::RandRange(0, 4);
		FName SectionName = FName();
		switch (RandomSelection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			SectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			SectionName = FName("Death5");
			DeathPose = EDeathPose::EDP_Death5;
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	if (Attributes && Attributes->IsDead())
	{
		Die();
	}
	else
	{
		DirectionalHitReact(ImpactPoint);
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this->GetWorld(), HitParticles, ImpactPoint);
	}
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
		//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f,
		//	FColor::Blue, 5.f);
	}

	FName SectionName("ReactBack"); // Because 135 > Theta < -135 means needs to react to the back

	if (Theta >= -45.f && Theta < 45.f)
		SectionName = FName("ReactFront");
	else if (Theta >= -135 && Theta < 45)
		SectionName = FName("ReactLeft");
	else if (Theta >= 45 && Theta < 135)
		SectionName = FName("ReactRight");

	this->PlayHitReactMontage(SectionName);

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	//}
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f,
	//	FColor::Red, 5.f);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60, 5.f,
	//	FColor::Green, 5.f);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, 
	AActor* DamageCauser)
{
	if (Attributes)
	{
		Attributes->ReceieveDamage(DamageAmount);
	}
	return DamageAmount;
}

