#pragma once
#include "../UIInits/initInventory.h"

void updateUpgradeInterfaceUI();
void updateInventoryUI();
void drawUpgradeInterface(sf::RenderWindow& window);

void drawInventory(sf::RenderWindow& window, Player* player) {
    window.setView(InventoryView);

    updateInventoryUI();
    {
        using namespace inventoryInterface;

        for (sf::Drawable*& elem : commonElements)
            window.draw(*elem);
        switch (activePage) {
            case inventoryPage::Items:
                for (sf::Drawable*& elem : pageElements[activePage])
                    window.draw(*elem);

                for (Item*& item : player->inventory.items) {
                    if (itemSlotsElements[item->id].isInitialized) {
                        window.draw(*itemSlotsElements[item->id].background);

                        if (item->amount >= 1) window.draw(*itemSlotsElements[item->id].amountText);
                    }
                    window.draw(*item);
                }

                if (isItemDescDrawn) {
                    itemDescText.setPosition(sf::Mouse::getPosition(window).x + 100, sf::Mouse::getPosition(window).y);
                    window.draw(itemDescText);
                }
                break;

            case inventoryPage::Weapons:
                for (sf::Drawable*& elem : pageElements[activePage])
                    window.draw(*elem);
                drawUpgradeInterface(window);
                break;

            case inventoryPage::Stats:
                for (sf::Drawable*& elem : pageElements[inventoryPage::Stats])
                    window.draw(*elem);
                break;

            default:
                break;
        }
    }
    window.setView(HUDView);
}

void drawUpgradeInterface(sf::RenderWindow& window) {
    window.setView(inventoryInterface::isDrawInventory ? InventoryView : InterfaceView);

    updateUpgradeInterfaceUI();
    {
        using namespace upgradeInterface;

        if (!inventoryInterface::isDrawInventory)
            for (sf::Drawable*& elem : UIElements)
                window.draw(*elem);

        float listBGVal = 128 * std::pow(std::sin(GameClock->getElapsedTime().asSeconds()), 2);
        if (isChoosingComponent) {
            switch (compType) {
                case 0:
                    choiceComp.setSpriteColor(sf::Color(0, 0, listBGVal));
                    break;
                case 1:
                    choiceComp.setSpriteColor(sf::Color(listBGVal, listBGVal, 0));
                    break;
                case 2:
                    choiceComp.setSpriteColor(sf::Color(0, listBGVal, 0));
                    break;
                case 3:
                    choiceComp.setSpriteColor(sf::Color(listBGVal, 0, 0));
                    break;
            }
            for (sf::Drawable* elem : choiceUIElements)
                window.draw(*elem);

            for (sf::Drawable* elem : compUpgCosts[compType])
                window.draw(*elem);
            for (sf::Drawable* elem : compUpgStats[compType])
                window.draw(*elem);
            for (sf::Drawable* elem : compUpgCount[compType])
                window.draw(*elem);
        }
    }

    window.setView(inventoryInterface::isDrawInventory ? InventoryView : HUDView);
}