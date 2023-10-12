#pragma once
#include <map>
#include <string>

enum ItemID
{
    gunParts,
    repairKit,
    keyCard,
    protectiveSuit,
    generic,
    medkit,
    regenDrug
};

std::map<ItemID, std::string> itemTextureName {
    {gunParts, "gunPartsItem"},
    {repairKit, "repairKitItem"},
    {keyCard, "keyCardItem"},
    {protectiveSuit, "protectiveSuitItem"},
    {regenDrug, "regenDrugItem"}
};

std::map<ItemID, std::string> pickupItemTextureName {
    {generic, "genericPickup"},
    {medkit, "medkitPickup"},
    {regenDrug, "regenDrugPickup"}
};