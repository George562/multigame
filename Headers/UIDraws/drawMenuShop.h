#pragma once
#include "../UIInits/initMenuShop.h"

void updateShopUI();

void drawShop(sf::RenderWindow& window) {
    window.setView(InterfaceView);
    {
        using namespace MenuShop;
        updateShopUI();

        for (sf::Drawable*& elem : UIElements)
            window.draw(*elem);

        window.setView(ShopStockView);
        for (Item*& item : shop.soldItems.items) {
            if (slotsElements[item->id].isInitialized) {
                window.draw(*slotsElements[item->id].background, stockTransform);
                if (item->amount >= 1) window.draw(*slotsElements[item->id].amountText, stockTransform);
                window.draw(*slotsElements[item->id].priceText, stockTransform);
            }
            window.draw(*item, stockTransform);
        }

        window.setView(ShopPlayerInvView);
        for (Item*& item : MenuShop::player->inventory.items) {
            if (playerSlotsElements[item->id].isInitialized) {
                window.draw(*playerSlotsElements[item->id].background, playerInvTransform);
                if (item->amount >= 1) window.draw(*playerSlotsElements[item->id].amountText, playerInvTransform);
                window.draw(*playerSlotsElements[item->id].priceText, playerInvTransform);
            }
            window.draw(*item, playerInvTransform);
        }

    }
    window.setView(HUDView);
}