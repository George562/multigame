#include <itemHelper.h>
#include <item.h>
#include <unordered_map>

class Inventory
{
public:
    std::unordered_map<ItemType, Item> items;
    std::unordered_map<ItemType, Item> keyItems;
    std::unordered_map<ItemType, Item> equipItems;
};