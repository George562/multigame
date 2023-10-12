#pragma once
#include <map>
#include <string>

enum ItemID
{
    none,
    gunParts,
    repairKit,
    keyCard,
    protectiveSuit,
    regenDrug
};

enum PickupItemID
{
    nonePickup,
    genericPickup,
    medkitPickup,
    regenDrugPickup
};

std::map<ItemID, std::string> itemTextureName{
    {none, ""},
    {gunParts, "gunPartsItem"},
    {repairKit, "repairKitItem"},
    {keyCard, "keyCardItem"},
    {protectiveSuit, "protectiveSuitItem"},
    {regenDrug, "regenDrugItem"}
};

std::map<PickupItemID, std::string> pickupItemTextureName{
    {nonePickup, ""},
    {genericPickup, "genericPickup"},
    {medkitPickup, "medkitPickup"},
    {regenDrugPickup, "regenDrugPickup"}
};