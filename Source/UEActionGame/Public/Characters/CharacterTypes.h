#pragma once

UENUM(BlueprintType)
enum class ECharacterWeaponState : uint8
{
	ECWS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECWS_Equipped UMETA(DisplayName = "Equipped")
};
