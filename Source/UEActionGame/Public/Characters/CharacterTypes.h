#pragma once

UENUM(BlueprintType)
enum class ECharacterWeaponState : uint8
{
	ECWS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECWS_Equipped UMETA(DisplayName = "Equipped"),
	ECWS_Torch UMETA(DisplayName = "Torch")
};

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	ECAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECAS_Swimming UMETA(DisplayName = "Swimming"),
	ECAS_Crouching UMETA(DisplayName = "Crouching"),
	ECAS_HitReaction UMETA(DisplayName = "Hit Reaction"),
	ECAS_Attacking UMETA(DisplayName = "Attacking"),
	ECAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"),
	ECAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum EDeathPose
{
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Death5 UMETA(DisplayName = "Death5"),

	EDP_MAX UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "No State"),

	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")
};

