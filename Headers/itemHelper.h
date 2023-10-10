#include <map>
#include <string>

enum ItemType
{
    none,
    // DEBUG
    gunParts,
    repairKit,
    keyCard,
    protectiveSuit
};

enum PickupItemType
{
    none,
    medkit,
    stimPack
};

static std::map<PickupItemType, std::string> pickupItemTextureName
{
    {none, ""},
    {medkit, "medkitPickup"},
    {stimPack, "stimPack"}
};