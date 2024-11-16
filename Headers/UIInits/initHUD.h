#pragma once
#include "../UI/bar.h"
#include "../UI/Frame.h"
#include "../UI/panel.h"
#include "../UI/PolygonButton.h"
#include "../UI/Slot.h"
#include "../UI/tempText.h"
#include "../Systems/effect.h"
#include "../Entities/player.h"

namespace HUD {
    bool EscapeMenuActivated = false, IsDrawHUD = true;
    std::vector<sf::Drawable*> InterfaceStuff;
    std::vector<TempText*> TempTextsOnScreen, MessageText;
    std::vector<Frame*> effectIcons(Effects::EffectCount);
    std::vector<TempText*> effectIconsTimers(Effects::EffectCount);

//////////////////////////////////////////////////////////// FPS Stuff
    sf::Clock ClockFPS; int FPSCounter;
    PlacedText TextFPS;

//////////////////////////////////////////////////////////// Panels
    Panel ListOfPlayers("listOfPlayers");


//////////////////////////////////////////////////////////// Buttons
    RectButton EscapeButton("exitBtn", UI::none, UI::none, { 0, 0 }, "Exit");

//////////////////////////////////////////////////////////// HUDStuff
    Frame HUDFrame("HUD", { 0, 0, scw, sch });
    Bar<float> HPBar("HPBar", UI::TR, UI::TR, { 360, 50 }), MPBar("MPBar", UI::BL, UI::TL, { 240, 50 });
    std::vector<Bar<float>*> AmmoBars;
    std::vector<PlacedText*> WeaponNameTexts;
    PlacedText ReloadWeaponText;
    Frame XButtonSprite("xBtn", UI::L, UI::BL, { 0, 0 });
    Frame InfoLogoSprite("infoBtn", UI::L, UI::TL, { 0, 0 });

    std::map<DescriptionID::Type, std::string> interactibleDiscriptions;
    PlacedText DescriptionText("descTxt", UI::center, UI::center, FontString("", 32));
    bool showDiscriptions;
}

void initHUD(Player* player, std::vector<Weapon*>* Weapons) {
    {
        using namespace HUD;
        
        ListOfPlayers.setTexture(Textures::GradientFrameAlpha);

        EscapeButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed, UI::texture);
        EscapeButton.setHitboxPoints({ EscapeButton.getLeftTop(), EscapeButton.getRightTop(),
                                       EscapeButton.getRightBottom(), EscapeButton.getLeftBottom() });

        EscapeButton.setCharacterSize(110);
        ListOfPlayers.text.setCharacterSize(60);

        EscapeButton.setCenter(scw / 2, sch * 3 / 4);

        ListOfPlayers.setCenter(scw / 2, sch / 4);

        HPBar.setValue(player->Health);
        HPBar.setFontString(FontString("", 36));
        HPBar.setColors(CommonColors::barWall, sf::Color(192, 0, 0, 160), CommonColors::barBG);
        HPBar.parentTo(&HUDFrame, true, { -30, 20 });

        MPBar.setValue(player->Mana);
        MPBar.setFontString(FontString("", 32));
        MPBar.setColors(CommonColors::barWall, sf::Color(0, 0, 192, 160), CommonColors::barBG);
        MPBar.parentTo(&HPBar, true);

        for (int i = 0; i < Weapons->size(); i++) {
            AmmoBars.push_back(new Bar<float>("AmmoBar_" + (*Weapons)[i]->Name,
                                            { 20, sch - 20 - (Weapons->size() - i) * 60, 160, 50 }));
            AmmoBars[i]->setColors(CommonColors::barWall, sf::Color(128, 128, 128, 160), CommonColors::barBG);
            AmmoBars[i]->parentTo(&HUDFrame);

            WeaponNameTexts.push_back(new PlacedText("weapName" + i + 1, UI::R, UI::L, FontString((*Weapons)[i]->Name, 36)));
            WeaponNameTexts[i]->setFillColor(sf::Color(25, 192, 25, 160));
            WeaponNameTexts[i]->setOutlineColor(sf::Color::Black);
            WeaponNameTexts[i]->parentTo(AmmoBars[i], true, { 35, 0 });
        }

        ReloadWeaponText.setFillColor(sf::Color(255, 20, 20));
        ReloadWeaponText.setCharacterSize(100);

        InfoLogoSprite.setTexture(Textures::InfoLogo, UI::texture);
        InfoLogoSprite.setScale(2.f, 2.f);
        InfoLogoSprite.parentTo(&HUDFrame, true, { 0, 20 });

        XButtonSprite.setTexture(Textures::XButton, UI::texture);
        XButtonSprite.setScale(2.f, 2.f);
        XButtonSprite.parentTo(&HUDFrame, true, { 0, -20 });

        DescriptionText.parentTo(&HUDFrame, true);

        for (int i = 0; i < Effects::EffectCount; i++) {
            effectIconsTimers[i] = new TempText(sf::Time::Zero);
            effectIcons[i] = new Frame("effect_" + i, UI::none, UI::none, (sf::Vector2f)Textures::Eff_HPRegen.getSize() / 2.f);
        }
        effectIcons[2]->setTexture(Textures::Eff_HPRegen, UI::element);
        effectIcons[3]->setTexture(Textures::Eff_Burn, UI::element);
    }
}

void loadDescriptions() {
    {
        using namespace HUD;
        std::ifstream descFile("sources/texts/descriptions.json");
        try {
            nlohmann::json j = nlohmann::json::parse(descFile);
            interactibleDiscriptions[DescriptionID::portal]        = j["portal"]        .template get<std::string>();
            interactibleDiscriptions[DescriptionID::box]           = j["box"]           .template get<std::string>();
            interactibleDiscriptions[DescriptionID::shopSector]    = j["shopSector"]    .template get<std::string>();
            interactibleDiscriptions[DescriptionID::upgradeSector] = j["upgradeSector"] .template get<std::string>();
            interactibleDiscriptions[DescriptionID::artifact]      = j["artifact"]      .template get<std::string>();
            interactibleDiscriptions[DescriptionID::fire]          = j["fire"]          .template get<std::string>();
        } catch(const std::exception& e) {
            for (DescriptionID::Type i = DescriptionID::portal; i < DescriptionID::DescriptionCount; i++) { 
                interactibleDiscriptions[i] = "Error loading description";
            }
        }
        descFile.close();
    }
}