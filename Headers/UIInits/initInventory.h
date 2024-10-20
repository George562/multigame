#pragma once
#include "../UI/bar.h"
#include "../UI/Frame.h"
#include "../UI/panel.h"
#include "../UI/PolygonButton.h"
#include "../UI/Slot.h"
#include "../UI/tempText.h"
#include "../UI/text.h"
#include "../Entities/player.h"
#include "../Systems/inventory.h"
#include "../Systems/weapon.h"
#include "../Utility/tools.h"
#include "../Utility/vectorTools.h"

namespace inventoryInterface {
    bool isDrawInventory = false;

    std::vector<bool> doInventoryUpdate(inventoryPage::PageCount);
    inventoryPage::Type activePage = inventoryPage::Items;
    std::vector<sf::Drawable*> commonElements; // These elements appear on every page
    std::vector<std::vector<sf::Drawable*>> pageElements; // These elements only appear on certain pages

    Frame inventoryFrame("InventoryFrame", { 0, 0, scw, sch });
    Frame itemsFrame("PageFrame_items", { 0, 0, scw, sch });
    Frame weaponsFrame("PageFrame_weapons", { 0, 0, scw, sch });
    Frame statsFrame("PageFrame_stats", { 0, 0, scw, sch });

    RectButton itemsButton("inv_itemsBtn", UI::BL, UI::BL, { 0.333f * scw, 0.1f * sch });
    RectButton weaponsButton("inv_weaponsBtn", UI::R, UI::L, { 0.333f * scw, 0.1f * sch });
    RectButton statsButton("inv_statsBtn", UI::R, UI::L, { 0.333f * scw, 0.1f * sch });

    ButtonsHolder frameButtonsHolder({&itemsButton, &weaponsButton, &statsButton});

    RectButton backButton("inv_backBtn", UI::TL, UI::TL, { 300, 150 });

    // ITEM INVENTORY ELEMENTS
    int prevItemTypeCount = 0;
    bool isItemDescDrawn = false;

    Frame itemListBG("itemListBG", UI::center, UI::center, { 0.8f * scw, 0.5f * sch });

    Animation coinSprite("inv_coinAnim", { 0, 0 });
    ItemSlot coinSlot;        // Special slot, so that it can be conveniently used other than in the inventory

    std::vector<ItemSlot> itemSlotsElements; // Elements that comprise an inventory slot - the background texture and the amount text

    Frame statsPlayerImage("statsPlImg", UI::center, UI::L, { 0, 0 });
    Bar<float> statsHPBar("statsHPBar", UI::R, UI::L, { 400, 60 });
    Bar<float> statsMPBar("statsMPBar", UI::R, UI::L, { 400, 60 });

    PlacedText statsHPText("statsHPText", UI::TL, UI::TL),
        statsMPText("statsMPText", UI::TL, UI::TL),
        statsArmorText("statsArmorText", UI::TL, UI::TL),
        statsHPRegenText("statsHPRegenText", UI::TL, UI::TL),
        statsMPRegenText("statsMPRegenText", UI::TL, UI::TL),
        statsCompletedLevelsText("statsComplLvlText", UI::TL, UI::TL),
        statsCurLevelsText("statsCurLvlText", UI::TL, UI::TL);

    PlacedText itemDescText;
    ItemID::Type prevItemDescID;
}

void setUpgradeFunctions();
void updateUpgradeShopStats();

///////////////////// CHECK THE CRASH AFTER CLICKING ON WEAPONS PAGE YOU ABSOLUTE BUFOON CRETIN IDIOT

namespace upgradeInterface {
    bool isDrawUpgradeInterface = false;
    std::vector<sf::Drawable*> UIElements;

    Animation coinSprite("upg_coinAnim", UI::R, UI::L);
    Frame BG("upg_BG", { 0, 0, scw, sch });
    Frame weaponImg("upg_weapImg", UI::center, UI::center, { 0, 0 });
    Panel weaponDescPanel("upg_weapDescPanel", UI::T, UI::T, { 0.5f * scw - 50.f, 475 });

    RectButton backButton("upg_backBtn", UI::TL, UI::TL, { 250, 50 });
    PolygonButton switchGunLBtn("upg_lGunBtn", UI::L, UI::R, { 0, 0 });
    PolygonButton switchGunRBtn("upg_rGunBtn", UI::R, UI::L, { 0, 0 });

    PolygonButton generatorBtn("upg_genBtn", { 0.100f * scw, 0.50f * sch + 100.f, 0, 0 }),
        formFactorBtn("upg_ffaBtn", { 0.333f * scw, 0.70f * sch + 50.0f, 0, 0 }),
        converterBtn("upg_conBtn", { 0.600f * scw, 0.65f * sch + 50.0f, 0, 0 }),
        targetingBtn("upg_tarBtn", { 0.825f * scw, 0.50f * sch + 100.f, 0, 0 });

    // UPGRADE SHOP COMPONENT CHOICE ELEMENTS
    std::vector<sf::Drawable*> choiceUIElements;
    Frame choiceBG("choiceUI_BG", { 0, 0, scw, sch });
    Frame choiceComp("choiceUI_comp", UI::center, UI::center, { 0.75f * scw, 0.5f * sch });
    Panel choiceMessage("choiceUI_msg", UI::T, UI::B, { 0.5f * scw, 0.125f * sch });
    Frame choiceCompImg("choiceUI_img", UI::TR, UI::TR, { 0.25f * scw, 0.25f * sch });
    std::vector<UIElement*> compUpgPages;
    std::vector<std::vector<RectButton*>> compUpgBtns;
    std::vector<std::vector<PlacedText*>> compUpgCosts;
    std::vector<std::vector<PlacedText*>> compUpgStats;
    std::vector<std::vector<PlacedText*>> compUpgCount;
    PlacedText playerCoinAmount("plCoin_amount", UI::BR, UI::BR);

    bool isChoosingComponent = false;
    int compType = 0;

    void openComponentUpgrade(int type) {
        isChoosingComponent = true;
        removeUI(&playerCoinAmount, choiceUIElements);
        removeUI(compUpgPages[compType], choiceUIElements);
        compType = type;
        addUI(&choiceBG, choiceUIElements);
        addUI(compUpgPages[compType], choiceUIElements);
        std::string compName;
        sf::Texture* compTexture = nullptr;
        switch (type) {
            case 0:
                compName = "Coch Generator";
                compTexture = &Textures::PH_CochGen;
                break;
            case 1:
                compName = "Form Factor";
                compTexture = &Textures::PH_FormFactor;
                break;
            case 2:
                compName = "Converter";
                compTexture = &Textures::PH_Converter;
                break;
            case 3:
                compName = "Targeting";
                compTexture = &Textures::PH_Targeting;
                break;
        }
        if (inventoryInterface::isDrawInventory) {
            choiceMessage.setFontString(FontString("You can only upgrade at a special upgrade shop.",
                                                   50, CommonColors::warning));
            removeUI(&playerCoinAmount, choiceUIElements);
        } else {
            choiceMessage.setFontString(FontString("Choose a " + compName + " upgrade   (press RMB to close)",
                                                   50, CommonColors::text));
            addUI(&playerCoinAmount, choiceUIElements);
        }
        choiceCompImg.setTexture(*compTexture, UI::size, { 500, 500 });
    }
}

void openUpgradeShop();

void initInventory(Player* player) {
    {
        using namespace inventoryInterface;

        pageElements.resize(inventoryPage::PageCount);
        itemSlotsElements.resize(ItemID::ItemCount, ItemSlot());

        inventoryFrame.setTexture(Textures::GridBG);

        backButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed);
        backButton.setWord(FontString("Back", 52));
        backButton.setFunction([]() { isDrawInventory = false; });
        backButton.parentTo(&inventoryFrame, true);

        itemsButton.setTexture(Textures::YellowButton, Textures::YellowButtonPushed);
        itemsButton.setWord(FontString("Items", 32));
        itemsButton.setFunction([]() {
            activePage = inventoryPage::Items;
            addUI(&itemsFrame, pageElements[activePage]);
        });
        itemsButton.parentTo(&inventoryFrame, true);

        weaponsButton.setTexture(Textures::RedButton, Textures::RedButtonPushed);
        weaponsButton.setWord(FontString("Weapons", 32));
        weaponsButton.setFunction([]() {
            activePage = inventoryPage::Weapons;
            addUI(&weaponsFrame, pageElements[activePage]);
            addUI(&upgradeInterface::BG, pageElements[activePage]);
            removeUI(&upgradeInterface::BG, pageElements[activePage], false);
            removeUI(&upgradeInterface::backButton, pageElements[activePage], false);
            openUpgradeShop();
        });
        weaponsButton.parentTo(&itemsButton, true);

        statsButton.setTexture(Textures::GreyButton, Textures::GreyButtonPushed);
        statsButton.setWord(FontString("Stats", 32));
        statsButton.setFunction([]() {
            activePage = inventoryPage::Stats;
            addUI(&statsFrame, pageElements[activePage]);
        });
        statsButton.parentTo(&weaponsButton, true);

        itemListBG.setTexture(Textures::GradientFrameAlpha);
        itemListBG.parentTo(&itemsFrame, true);

        statsPlayerImage.setTexture(Textures::Player, UI::texture);
        statsPlayerImage.parentTo(&statsFrame, true, { 200, 0 });

        int fontSize = 40;
        float yOffset = sch / 10;
        statsHPText.setFontString(FontString("Health", fontSize));
        statsHPText.parentTo(&statsFrame, true, { 200, 200 });
        statsHPBar.setValue(player->Health);
        statsHPBar.setColors(CommonColors::barWall, sf::Color(192, 0, 0, 255), CommonColors::barBG);
        statsHPBar.parentTo(&statsHPText, true, { 300, 0 });

        statsMPText.setFontString(FontString("Mana", fontSize));
        statsMPText.parentTo(&statsHPText, true, { 0, yOffset });
        statsMPBar.setValue(player->Mana);
        statsMPBar.setColors(CommonColors::barWall, sf::Color(0, 0, 192, 255), CommonColors::barBG);
        statsMPBar.parentTo(&statsMPText);
        statsMPBar.setPosition(statsHPBar.getPosition() + sf::Vector2f{ 0, yOffset });

        statsHPRegenText.setFontString(FontString("Health regen: " + floatToString(player->HealthRecovery), fontSize));
        statsHPRegenText.parentTo(&statsMPText, true, { 0, yOffset });

        statsMPRegenText.setFontString(FontString("Mana regen: " + floatToString(player->ManaRecovery), fontSize));
        statsMPRegenText.parentTo(&statsHPRegenText, true, { 0, yOffset });

        statsArmorText.setFontString(FontString("Armor: " + floatToString(player->Armor.cur), fontSize));
        statsArmorText.parentTo(&statsMPRegenText, true, { 0, yOffset });

        statsCompletedLevelsText.setFontString(FontString("Completed Levels: " + std::to_string(completedLevels), fontSize));
        statsCompletedLevelsText.parentTo(&statsArmorText, true, { 0, yOffset });

        statsCurLevelsText.setFontString(FontString("Current Levels: " + std::to_string(curLevel), fontSize));
        statsCurLevelsText.parentTo(&statsCompletedLevelsText, true, { 0, yOffset });

        coinSprite.parentTo(&itemsFrame);
        coinSprite.setPosition(backButton.getRightTop() + sf::Vector2f(200, 25));
        coinSprite.setAnimation(*itemTexture[ItemID::coin], itemTextureFrameAmount[ItemID::coin],
                                1, itemTextureDuration[ItemID::coin]);
        coinSprite.play();

        coinSlot.init("coinSlot");
        coinSlot.amountText->setAnchors(UI::R, UI::L);
        coinSlot.amountText->setFontString(FontString("0", 50));
        coinSlot.amountText->parentTo(&coinSprite, true, { 50, 0 });
        doInventoryUpdate[inventoryPage::Stats] = true;
        doInventoryUpdate[inventoryPage::Weapons] = true;

        addUI(&inventoryFrame, commonElements);

        itemDescText.setOutlineThickness(2);
        itemDescText.setOutlineColor(sf::Color::Black);
    }
}

void initUpgradeUI();

void initUpgradeShop() {
    {
        using namespace upgradeInterface;
        BG.setTexture(Textures::GridBG);

        backButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed);
        backButton.setWord(FontString("Back", 36));
        backButton.setFunction([]() { isDrawUpgradeInterface = false; });
        backButton.parentTo(&BG, true);

        weaponDescPanel.setTexture(Textures::GradientFrameAlpha);
        weaponDescPanel.setSpriteColor(sf::Color::Black);
        weaponDescPanel.setCharacterSize(30);
        weaponDescPanel.parentTo(&BG, true, { 0, 75 });

        weaponImg.setTexture(Textures::PH_Pistol, UI::texture);
        weaponImg.parentTo(&BG, true, { 0, 150 });

        switchGunLBtn.setHitboxPoints(std::vector<sf::Vector2f>{{50, 100}, { 50, 0 }, { 0, 50 }});
        switchGunLBtn.setShape(sf::Color::Black, sf::Color::White, 2);
        switchGunLBtn.enableShape(true);
        switchGunLBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        switchGunLBtn.parentTo(&weaponImg, true, { -50, 0 });

        switchGunRBtn.setHitboxPoints(std::vector<sf::Vector2f>{{0, 100}, { 0, 0 }, { 50, 50 }});
        switchGunRBtn.setShape(sf::Color::Black, sf::Color::White, 2);
        switchGunRBtn.enableShape(true);
        switchGunRBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        switchGunRBtn.parentTo(&weaponImg, true, { 50, 0 });

        choiceComp.setTexture(Textures::GradientFrameAlpha);
        choiceComp.parentTo(&choiceBG, true);

        sf::Texture* fadeTexture = new sf::Texture();
        sf::Image fadeTexturePixels = sf::Image();
        fadeTexturePixels.create(scw, sch, sf::Color(255, 255, 255, 128));
        for (int i = 0; i < scw; i++)
            for (int j = 0; j < sch; j++)
                if (i >= choiceComp.getLeft() && i <= choiceComp.getRight() &&
                    j >= choiceComp.getTop() && j <= choiceComp.getBottom()) {
                    float gradVal = 25 * (((int)(i - choiceComp.getLeft() + j - choiceComp.getTop()) / (255)) % 25);
                    fadeTexturePixels.setPixel(i, j, sf::Color(gradVal, gradVal, gradVal, 255));
                }
        fadeTexture->create(scw, sch);
        fadeTexture->update(fadeTexturePixels);
        choiceBG.setTexture(*fadeTexture);

        choiceMessage.setTexture(Textures::GradientFrame);
        choiceMessage.setFontString(FontString("Choose a component upgrade", 40));
        choiceMessage.parentTo(&choiceComp, true, { 0, -100 });

        choiceCompImg.setTexture(Textures::PH_FormFactor);
        choiceCompImg.parentTo(&choiceComp, true, { -50, 50 });

        generatorBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        generatorBtn.setHitboxPoints(CommonShapes::starShape);
        generatorBtn.enableShape(true);
        generatorBtn.setFunction([]() { openComponentUpgrade(0); });
        generatorBtn.setShape(sf::Color::Transparent, sf::Color(192, 192, 255, 255), 3);
        generatorBtn.parentTo(&BG);

        formFactorBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        formFactorBtn.setHitboxPoints(CommonShapes::rectShape);
        formFactorBtn.enableShape(true);
        formFactorBtn.setFunction([]() { openComponentUpgrade(1); });
        formFactorBtn.setShape(sf::Color::Transparent, sf::Color(255, 255, 192, 255), 3);
        formFactorBtn.parentTo(&BG);

        converterBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        converterBtn.setHitboxPoints(RotateOn(-30, CommonShapes::triangleShape));
        converterBtn.enableShape(true);
        converterBtn.setFunction([]() { openComponentUpgrade(2); });
        converterBtn.setShape(sf::Color::Transparent, sf::Color(192, 255, 192, 255), 3);
        converterBtn.parentTo(&BG);

        targetingBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        targetingBtn.setHitboxPoints(RotateOn(60, CommonShapes::frustumShape));
        targetingBtn.enableShape(true);
        targetingBtn.setFunction([]() { openComponentUpgrade(3); });
        targetingBtn.setShape(sf::Color::Transparent, sf::Color(255, 192, 192, 255), 3);
        targetingBtn.parentTo(&BG);

        playerCoinAmount.setFontString(FontString("You have:", 50, sf::Color(200, 20, 200)));
        playerCoinAmount.parentTo(&choiceComp, true, { -100, -50 });

        coinSprite.setScale({ 0.5, 0.5 });
        coinSprite.setPosition({ playerCoinAmount.getRight() + 20, playerCoinAmount.getTop() - 50 });
        coinSprite.setAnimation(*itemTexture[ItemID::coin], itemTextureFrameAmount[ItemID::coin],
                                1, itemTextureDuration[ItemID::coin]);
        coinSprite.play();
        coinSprite.parentTo(&playerCoinAmount, true);

        initUpgradeUI();
    }
}

void initUpgradeUI() {
    {
        using namespace upgradeInterface;

        int btnTextSize = 24;
        sf::Vector2f costOffset = sf::Vector2f(300, -25);
        sf::Vector2f countOffset = sf::Vector2f(0, -24);
        sf::Vector2f statOffset = sf::Vector2f(0, 0);

        compUpgBtns.resize(4);
        compUpgCosts.resize(4);
        compUpgStats.resize(4);
        compUpgCount.resize(4);

        compUpgPages.push_back(new Frame("upgPage_CGen", choiceComp.getRect()));
        compUpgPages.push_back(new Frame("upgPage_FFac", choiceComp.getRect()));
        compUpgPages.push_back(new Frame("upgPage_Conv", choiceComp.getRect()));
        compUpgPages.push_back(new Frame("upgPage_Targ", choiceComp.getRect()));

        compUpgBtns[0].push_back(new RectButton("upg_MMSBtn",
                                                UI::TL, UI::TL, { 250, 100 },
                                                FontString("Mana storage", btnTextSize))
        );
        compUpgBtns[0][0]->setTexture(Textures::BlueButton, Textures::BlueButtonPushed);
        compUpgBtns[0][0]->parentTo(compUpgPages[0], true, { 50, 50 });

        compUpgCosts[0].push_back(new PlacedText());
        compUpgStats[0].push_back(new PlacedText());
        compUpgCount[0].push_back(new PlacedText());
        compUpgCosts[0][0]->setPosition(compUpgBtns[0][0]->getCenter() + costOffset);
        compUpgStats[0][0]->setPosition(compUpgBtns[0][0]->getRightTop() + statOffset);
        compUpgCount[0][0]->setPosition(compUpgBtns[0][0]->getRightBottom() + countOffset);


        compUpgBtns[0].push_back(new RectButton("upg_RSBtn",
                                                UI::BL, UI::TL, { 250, 100 },
                                                FontString("Reload Speed", btnTextSize))
        );
        compUpgBtns[0][1]->setTexture(Textures::GreenButton, Textures::GreenButtonPushed);
        compUpgBtns[0][1]->parentTo(compUpgBtns[0][0], true, { 0, 50 });

        compUpgCosts[0].push_back(new PlacedText());
        compUpgStats[0].push_back(new PlacedText());
        compUpgCount[0].push_back(new PlacedText());
        compUpgCosts[0][1]->setPosition(compUpgBtns[0][1]->getCenter() + costOffset);
        compUpgStats[0][1]->setPosition(compUpgBtns[0][1]->getRightTop() + statOffset);
        compUpgCount[0][1]->setPosition(compUpgBtns[0][1]->getRightBottom() + countOffset);


        compUpgBtns[1].push_back(new RectButton("upg_TTHBtn",
                                                UI::TL, UI::TL, { 250, 100 },
                                                FontString("Time To\nHolster", btnTextSize))
        );
        compUpgBtns[1][0]->setTexture(Textures::BlueButton, Textures::BlueButtonPushed);
        compUpgBtns[1][0]->parentTo(compUpgPages[1], true, { 50, 50 });

        compUpgCosts[1].push_back(new PlacedText());
        compUpgStats[1].push_back(new PlacedText());
        compUpgCount[1].push_back(new PlacedText());
        compUpgCosts[1][0]->setPosition(compUpgBtns[1][0]->getCenter() + costOffset);
        compUpgStats[1][0]->setPosition(compUpgBtns[1][0]->getRightTop() + statOffset);
        compUpgCount[1][0]->setPosition(compUpgBtns[1][0]->getRightBottom() + countOffset);


        compUpgBtns[1].push_back(new RectButton("upg_TTDBtn",
                                                UI::BL, UI::TL, { 250, 100 },
                                                FontString("Time To\nDispatch", btnTextSize))
        );
        compUpgBtns[1][1]->setTexture(Textures::GreenButton, Textures::GreenButtonPushed);
        compUpgBtns[1][1]->parentTo(compUpgBtns[1][0], true, { 0, 50 });

        compUpgCosts[1].push_back(new PlacedText());
        compUpgStats[1].push_back(new PlacedText());
        compUpgCount[1].push_back(new PlacedText());
        compUpgCosts[1][1]->setPosition(compUpgBtns[1][1]->getCenter() + costOffset);
        compUpgStats[1][1]->setPosition(compUpgBtns[1][1]->getRightTop() + statOffset);
        compUpgCount[1][1]->setPosition(compUpgBtns[1][1]->getRightBottom() + countOffset);


        compUpgBtns[2].push_back(new RectButton("upg_FRBtn",
                                                UI::TL, UI::TL, { 250, 100 },
                                                FontString("Fire Rate", btnTextSize))
        );
        compUpgBtns[2][0]->setTexture(Textures::BlueButton, Textures::BlueButtonPushed);
        compUpgBtns[2][0]->parentTo(compUpgPages[2], true, { 50, 50 });

        compUpgCosts[2].push_back(new PlacedText());
        compUpgStats[2].push_back(new PlacedText());
        compUpgCount[2].push_back(new PlacedText());
        compUpgCosts[2][0]->setPosition(compUpgBtns[2][0]->getCenter() + costOffset);
        compUpgStats[2][0]->setPosition(compUpgBtns[2][0]->getRightTop() + statOffset);
        compUpgCount[2][0]->setPosition(compUpgBtns[2][0]->getRightBottom() + countOffset);


        compUpgBtns[2].push_back(new RectButton("upg_MCBtn",
                                                UI::BL, UI::TL, { 250, 100 },
                                                FontString("Mana Cost", btnTextSize))
        );
        compUpgBtns[2][1]->setTexture(Textures::GreenButton, Textures::GreenButtonPushed);
        compUpgBtns[2][1]->parentTo(compUpgBtns[2][0], true, { 0, 50 });

        compUpgCosts[2].push_back(new PlacedText());
        compUpgStats[2].push_back(new PlacedText());
        compUpgCount[2].push_back(new PlacedText());
        compUpgCosts[2][1]->setPosition(compUpgBtns[2][1]->getCenter() + costOffset);
        compUpgStats[2][1]->setPosition(compUpgBtns[2][1]->getRightTop() + statOffset);
        compUpgCount[2][1]->setPosition(compUpgBtns[2][1]->getRightBottom() + countOffset);


        compUpgBtns[2].push_back(new RectButton("upg_MSBtn",
                                                UI::BL, UI::TL, { 250, 100 },
                                                FontString("Multishot", btnTextSize))
        );
        compUpgBtns[2][2]->setTexture(Textures::RedButton, Textures::RedButtonPushed);
        compUpgBtns[2][2]->parentTo(compUpgBtns[2][1], true, { 0, 50 });

        compUpgCosts[2].push_back(new PlacedText());
        compUpgStats[2].push_back(new PlacedText());
        compUpgCount[2].push_back(new PlacedText());
        compUpgCosts[2][2]->setPosition(compUpgBtns[2][2]->getCenter() + costOffset);
        compUpgStats[2][2]->setPosition(compUpgBtns[2][2]->getRightTop() + statOffset);
        compUpgCount[2][2]->setPosition(compUpgBtns[2][2]->getRightBottom() + countOffset);

        compUpgBtns[3].push_back(new RectButton("upg_BVBtn",
                                                UI::TL, UI::TL, { 250, 100 },
                                                FontString("Bullet Velocity", btnTextSize))
        );
        compUpgBtns[3][0]->setTexture(Textures::BlueButton, Textures::BlueButtonPushed);
        compUpgBtns[3][0]->parentTo(compUpgPages[3], true, { 50, 50 });

        compUpgCosts[3].push_back(new PlacedText());
        compUpgStats[3].push_back(new PlacedText());
        compUpgCount[3].push_back(new PlacedText());
        compUpgCosts[3][0]->setPosition(compUpgBtns[3][0]->getCenter() + costOffset);
        compUpgStats[3][0]->setPosition(compUpgBtns[3][0]->getRightTop() + statOffset);
        compUpgCount[3][0]->setPosition(compUpgBtns[3][0]->getRightBottom() + countOffset);

        compUpgBtns[3].push_back(new RectButton("upg_SCBtn",
                                                UI::BL, UI::TL, { 250, 100 },
                                                FontString("Scatter", btnTextSize))
        );
        compUpgBtns[3][1]->setTexture(Textures::GreenButton, Textures::GreenButtonPushed);
        compUpgBtns[3][1]->parentTo(compUpgBtns[3][0], true, { 0, 50 });

        compUpgCosts[3].push_back(new PlacedText());
        compUpgStats[3].push_back(new PlacedText());
        compUpgCount[3].push_back(new PlacedText());
        compUpgCosts[3][1]->setPosition(compUpgBtns[3][1]->getCenter() + costOffset);
        compUpgStats[3][1]->setPosition(compUpgBtns[3][1]->getRightTop() + statOffset);
        compUpgCount[3][1]->setPosition(compUpgBtns[3][1]->getRightBottom() + countOffset);
    }
}