#include <map>
#include <string>

enum PickupItemType
{
    none,
    medkit,
    pistolAmmo
};

static std::map<PickupItemType, std::string> pickupItemTextureName
{
    {none, ""},
    {medkit, "medkitPickup"},
    {pistolAmmo, "pistolAmmoPickup"}
};