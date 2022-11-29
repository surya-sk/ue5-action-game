#pragma once

UENUM(BlueprintType)
enum class ECharacterWeaponState : uint8
{
	ECWS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECWS_Equipped UMETA(DisplayName = "Equipped")
};

enum class ECharacterActionState : uint8
{
	ECAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECAS_Attacking UMETA(DisplayName = "Attacking")
};
