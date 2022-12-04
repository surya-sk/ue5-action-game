#pragma once

UENUM(BlueprintType)
enum class ECharacterWeaponState : uint8
{
	ECWS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECWS_Equipped UMETA(DisplayName = "Equipped")
};

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	ECAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECAS_Attacking UMETA(DisplayName = "Attacking"),
	ECAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon")
};