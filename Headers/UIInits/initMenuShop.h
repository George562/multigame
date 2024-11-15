#pragma once
#include "../UI/bar.h"
#include "../UI/Frame.h"
#include "../UI/panel.h"
#include "../UI/PolygonButton.h"
#include "../UI/Slot.h"
#include "../Entities/player.h"
#include "../Systems/inventory.h"
#include "../Systems/shop.h"

namespace MenuShop {
    bool isDrawShop = false;
    Shop shop;

    std::vector<sf::Drawable*> UIElements;
    std::vector<ShopSlot> slotsElements; // Analogous to the inventory itemSlotsElements + price
    std::vector<ShopSlot> playerSlotsElements; // Analogous to the inventory itemSlotsElements + price

    Frame BG("shop_BG", { 0, 0, scw, sch });
    Frame NPCTextFrame("shop_NPCTextFrame", UI::BL, UI::TL, { scw - 100, 200 });
    Frame itemsFrame("shop_ItemsFrame", UI::BL, UI::TL, { 0.6f * scw - 100, 0.35f * sch - 50 });
    Frame playerInvFrame("shop_PlInvFrame", UI::BR, UI::TR, { 0.4f * scw - 50, 0.35f * sch - 50 });
    Frame itemStatsFrame("shop_ItemStatsFrame", UI::BL, UI::TL, { 0.6f * scw - 100, 0.35f * sch - 50 });
    Frame itemSprite("shop_ItemSprite", UI::center, UI::center, { 0, 0 });

    RectButton backButton("shop_backBtn", UI::TL, UI::TL, { 250, 50 });
    RectButton buyButton("shop_buyBtn", UI::B, UI::T, { 400, 150 }, "Buy", []() { shop.buyFunction(); });

    ShopSlot itemSlot;
    Item* selectedItem = nullptr;

    Frame NPCSprite("shop_NPCSprite", UI::L, UI::L, { 150, 150 });
    Animation itemCoinsSprite("shop_ItemCoinSprite", UI::R, UI::L);
    Animation playerCoinsSprite("shop_PlCoinsSprite", UI::R, UI::L);

    float itemsViewSizeX, itemsViewSizeY;
    sf::Transform stockTransform;
    float playerInvViewSizeX, playerInvViewSizeY;
    sf::Transform playerInvTransform;

    PlacedText NPCName("shop_NPCName", UI::R, UI::L);
    PlacedText NPCText("shop_NPCText", UI::center, UI::center);
    PlacedText itemStatsText("shop_ItemStats", UI::R, UI::L);
    PlacedText playerCoinsText("shop_PlCoinsText", UI::TL, UI::B);
}

void initShop(Player* player) {
    {
        using namespace MenuShop;
        playerSlotsElements.resize(ItemID::ItemCount);
        slotsElements.resize(ItemID::ItemCount);
        shop.setShop(new std::vector<Item*>{ new Item(ItemID::regenDrug, 10) },
                             std::vector<int>{20});

        if (player->inventory.find(ItemID::dasher) == -1) {
            shop.addItem(new Item(ItemID::dasher, 1), 10);
        }

        BG.setTexture(Textures::GridBG);

        backButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed);
        backButton.setWord(FontString("Back", 36));
        backButton.setFunction([]() {
            isDrawShop = false;
            selectedItem = nullptr;
            removeUI(&MenuShop::BG, MenuShop::UIElements);
        });
        backButton.parentTo(&BG, true);

        NPCTextFrame.setTexture(Textures::GradientFrameAlpha, UI::element);
        NPCTextFrame.setSpriteColor(sf::Color(0x10, 0xBB, 0xFF));
        NPCTextFrame.parentTo(&backButton, true, { 0, 25 });

        NPCSprite.setTexture(Textures::DistortedScientist, UI::element);
        NPCSprite.parentTo(&NPCTextFrame, true, { 100, 0 });

        NPCName.setFontString(FontString(textWrap("Shop keeper", 20), 32));
        NPCName.parentTo(&NPCSprite, true, { 50, 0 });

        NPCText.setCharacterSize(32);
        NPCText.parentTo(&NPCTextFrame, true);

        itemsFrame.setTexture(Textures::GradientFrameAlpha);
        itemsFrame.setSpriteColor(sf::Color(0xCC, 0xAA, 0x11));
        itemsFrame.parentTo(&NPCTextFrame, true, { 0, 100 });

        itemsViewSizeX = (0.6 * scw - 100) / scw;
        itemsViewSizeY = (0.35 * sch - 50) / sch;
        ShopStockView.setViewport(sf::FloatRect((itemsFrame.getPosition().x + 50) / scw,
                                                (itemsFrame.getPosition().y + 50 / 3) / sch,
                                                itemsViewSizeX, itemsViewSizeY));
        stockTransform.scale(1, 1 / itemsViewSizeY);

        playerInvFrame.setTexture(Textures::GradientFrameAlpha);
        playerInvFrame.setSpriteColor(sf::Color(0xBB, 0x40, 0x40));
        playerInvFrame.parentTo(&NPCTextFrame, true, { 0, 100 });

        playerInvViewSizeX = (0.6 * scw - 100) / scw;
        playerInvViewSizeY = (0.35 * sch - 50) / sch;
        ShopPlayerInvView.setViewport(sf::FloatRect((playerInvFrame.getPosition().x + 50) / scw,
                                                    (playerInvFrame.getPosition().y + 50) / sch,
                                                    playerInvViewSizeX, playerInvViewSizeY));
        playerInvTransform.scale(1, 1 / playerInvViewSizeY);

        itemStatsFrame.setTexture(Textures::GradientFrameAlpha);
        itemStatsFrame.setSpriteColor(sf::Color(0xCC, 0xAA, 0x11));
        itemStatsFrame.parentTo(&itemsFrame, true, { 0, 100 });

        itemSlot.init("mShop_ChosenItemSlot", UI::TL, UI::TL,
                      sf::Vector2f(itemStatsFrame.getSize().y / 2,
                                   itemStatsFrame.getSize().y / 2));
        itemSlot.setTexture(Textures::ItemPanel, UI::element);
        itemSlot.background->setSpriteColor(sf::Color(0xAA, 0x88, 0x00));
        itemSlot.priceText->setCharacterSize(40);
        itemSlot.parentTo(&itemStatsFrame, true, { 50, 50 });
        itemSlot.priceText->setCenter(itemSlot.getPosition().x, itemSlot.priceText->getCenter().y);

        itemCoinsSprite.setAnimation(*itemTexture[ItemID::coin], itemTextureFrameAmount[ItemID::coin],
                                     1, itemTextureDuration[ItemID::coin]);
        itemCoinsSprite.setSize({ 50, 50 });
        itemCoinsSprite.parentTo(itemSlot.priceText, true, { 20, -15 });
        itemCoinsSprite.parentTo(&itemSlot);
        itemCoinsSprite.play();

        itemSprite.setTexture(Textures::INVISIBLE, UI::texture);
        itemSprite.parentTo(&itemSlot, true);

        itemStatsText.setCharacterSize(30);
        itemStatsText.parentTo(&itemSlot, true, { 50, 0 });

        buyButton.setTexture(Textures::YellowPanel, Textures::YellowPanelPushed);
        buyButton.setCharacterSize(70);
        buyButton.parentTo(&playerInvFrame, true, { 0, 300 });

        playerCoinsText.setCharacterSize(40);
        playerCoinsText.parentTo(&buyButton, true, { 0, -100 });

        playerCoinsSprite.setAnimation(*itemTexture[ItemID::coin], itemTextureFrameAmount[ItemID::coin],
                                       1, itemTextureDuration[ItemID::coin]);
        playerCoinsSprite.setSize({ 75, 75 });
        playerCoinsSprite.parentTo(&playerCoinsText, true);
        playerCoinsSprite.play();
    }
}