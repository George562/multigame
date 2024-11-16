#include "../UIInits/initHUD.h"

void drawEffects(sf::RenderWindow&, Player*);

void drawHUD(sf::RenderWindow& window, Player* player, std::vector<Weapon*>* Weapons) {
    {
        using namespace HUD;
        window.setView(HUDView);
        for (sf::Drawable*& d : InterfaceStuff) {
            window.draw(*d);
        }

        for (int i = 0; i < Weapons->size(); i++) {
            if ((*Weapons)[i] == player->CurWeapon) {
                WeaponNameTexts[i]->setFillColor(sf::Color::White);
                WeaponNameTexts[i]->setOutlineThickness(3);
            } else {
                WeaponNameTexts[i]->setFillColor(sf::Color(25, 192, 25, 160));
                WeaponNameTexts[i]->setOutlineThickness(1);
            }
            AmmoBars[i]->setValue((*Weapons)[i]->ManaStorage);

            if ((*Weapons)[i]->holstered) {
                float holsterPercent = std::min((*Weapons)[i]->HolsterTimer->getElapsedTime() /
                                                (*Weapons)[i]->TimeToHolster, 1.0f);
                sf::Color wallColor(255 - 155 * holsterPercent, 255 - 155 * holsterPercent, 255, 160);
                sf::Color foreColor(128 - 96 * holsterPercent, 128 - 96 * holsterPercent, 128, 160);
                sf::Color backColor(32 - 32 * holsterPercent, 32 - 32 * holsterPercent, 32 - 32 * holsterPercent, 160);
                AmmoBars[i]->setColors(wallColor, foreColor, backColor);
            } else {
                float dispatchPercent = std::min((*Weapons)[i]->DispatchTimer->getElapsedTime() /
                                                (*Weapons)[i]->TimeToDispatch, 1.0f);
                sf::Color wallColor(100 + 155 * dispatchPercent, 100 + 155 * dispatchPercent, 255, 160);
                sf::Color foreColor(32 + 96 * dispatchPercent, 32 + 96 * dispatchPercent, 128, 160);
                sf::Color backColor(32 * dispatchPercent, 32 * dispatchPercent, 32 * dispatchPercent, 160);
                AmmoBars[i]->setColors(wallColor, foreColor, backColor);
            }

            window.draw(*WeaponNameTexts[i]);
        }

        if (EscapeMenuActivated) {
            window.draw(ListOfPlayers);
            window.draw(EscapeButton);
        }

        FPSCounter++;
        if (ClockFPS.getElapsedTime() >= sf::seconds(1)) {
            TextFPS.setString(std::to_string(FPSCounter) + " FPS");
            FPSCounter = 0;
            ClockFPS.restart();
        }
        window.draw(TextFPS);

        for (size_t i = 0; i < TempTextsOnScreen.size(); i++) {
            if (TempTextsOnScreen[i]->localClock->getElapsedTime() < TempTextsOnScreen[i]->howLongToExist) {
                window.draw(*TempTextsOnScreen[i]);
            } else {
                DeletePointerFromVector(TempTextsOnScreen, i--);
            }
        }

        for (size_t i = 0; i < MessageText.size(); i++) {
            if (MessageText[i]->localClock->getElapsedTime() < MessageText[i]->howLongToExist) {
                Shaders::FloatingUp.setUniform("uTime", MessageText[i]->localClock->getElapsedTime().asSeconds());
                window.draw(*MessageText[i], &Shaders::FloatingUp);
            } else {
                DeletePointerFromVector(MessageText, i--);
            }
        }

        drawEffects(window, player);

        if (showDiscriptions) {
            window.draw(DescriptionText);
        }

        window.setView(GameView);
    }
}

void drawEffects(sf::RenderWindow& window, Player* player) {
    {
        using namespace HUD;
        int count = 0;
        int xOffset = 175, yOffset = 175;
        std::vector<int> seenEffects(Effects::EffectCount, 0);
        std::vector<sf::Time> effectTimersTimes(Effects::EffectCount, sf::Time::Zero);
        for (Effect* eff : player->effects) {
            if (eff->type != Effects::Heal && eff->type != Effects::Damage && eff->active) {
                if (seenEffects[eff->type] == 0) {
                    effectIcons[eff->type]->setPosition(MPBar.getPosition().x - 300 + xOffset * (count % 3),
                                                        MPBar.getPosition().y + HUD::MPBar.getSize().y + 20 + yOffset * (count / 3));

                    effectIconsTimers[eff->type]->setPosition(effectIcons[eff->type]->getPosition() + sf::Vector2f(0, yOffset * 2 / 3));
                    effectIconsTimers[eff->type]->setCharacterSize(28);
                    window.draw(*effectIcons[eff->type]);

                    count++;
                }
                seenEffects[eff->type] += 1;
                effectTimersTimes[eff->type] = std::max(effectTimersTimes[eff->type], eff->howLongToExist);
            }
        }
        for (int i = 0; i < effectTimersTimes.size(); i++) {
            if (seenEffects[i] > 0) {
                TempText* txt = effectIconsTimers[i];
                sf::Time timeDif = txt->howLongToExist - txt->localClock->getElapsedTime();
                if (txt->localClock->getElapsedTime() > txt->howLongToExist || effectTimersTimes[i] > timeDif) {
                    txt->howLongToExist = effectTimersTimes[i];
                    txt->localClock->restart();
                }
                txt->setString("x" + std::to_string(seenEffects[i]) + "\t\t\t\t" +
                            floatToString(timeDif.asSeconds()));
                window.draw(*txt);
            }
        }
    }
}

void displayDescription(DescriptionID::Type id) {
    {
        using namespace HUD;
        if (interactibleDiscriptions[id] != DescriptionText.getText()) {
            showDiscriptions = true;
            DescriptionText.setString(interactibleDiscriptions[id]);
            DescriptionText.parentTo(&HUD::HUDFrame, true, { 0, -200 });
        } else {
            showDiscriptions ^= true;
        }
    }
}