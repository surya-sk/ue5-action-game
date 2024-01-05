// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Ghost.h"
#include "Components/SplineComponent.h"
#include "Environment/SplinePath.h"

AGhost::AGhost()
{
}

void AGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMoveAlongSpline)
	{
		MoveAlongSpline(bPatrol);
	}
}

float AGhost::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AGhost::GetHit(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit(ImpactPoint, Hitter);
}

void AGhost::BeginPlay()
{
	Super::BeginPlay();

	if (Action == EGhostAction::EGA_AppearAndDisappear)
	{
		GetWorldTimerManager().SetTimer(DestroyTimer, this, &AGhost::Disappear, DisappearIn, false);
	}
	else if (Action == EGhostAction::EGA_WalkBy)
	{
		bMoveAlongSpline = true;
	}
	else if(Action == EGhostAction::EGA_Patrol)
	{
		bMoveAlongSpline = true;
		bPatrol = true;
	}
}

void AGhost::Die()
{
	Super::Die();
}

void AGhost::Attack()
{
}

bool AGhost::CanAttack()
{
	return Super::CanAttack();
}

void AGhost::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
}

void AGhost::AttackEnd()
{
}

void AGhost::Disappear()
{
	Destroy();
}

void AGhost::MoveAlongSpline(bool bLoop)
{
    static bool bForward = true;

    if (SplinePath)
    {
        USplineComponent* Spline = SplinePath->GetSpline();
        if (Spline)
        {
            float SplineLength = Spline->GetSplineLength();
            CurrentDistance += (bForward ? 1 : -1) * Speed * GetWorld()->GetDeltaSeconds();

            if (bLoop)
            {
                if (CurrentDistance > SplineLength)
                {
                    CurrentDistance = SplineLength;
                    bForward = false;
                }
                else if (CurrentDistance < 0.0f)
                {
                    CurrentDistance = 0.0f;
                    bForward = true;
                }
            }
            else
            {
                CurrentDistance = FMath::Clamp(CurrentDistance, 0.0f, SplineLength);
            }

            FVector SplineLocation = Spline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
            FVector SplineDirection = Spline->GetDirectionAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);

            if (!bForward)
            {
                SplineDirection *= -1;
            }

            FRotator SplineRotation = SplineDirection.Rotation();

            SetActorLocationAndRotation(SplineLocation, SplineRotation);
        }
    }
}
