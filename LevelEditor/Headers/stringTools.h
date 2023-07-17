#pragma once
#include <vector>
#include <string>
#include "../../SFML-2.5.1/include/SFML/Graphics/Text.hpp"

std::vector<std::string> splitString(sf::Text text, char ch)
{
    std::vector<std::string> strings;
    int lastInd = 0;

    for(int i = 0; i < text.getString().getSize(); i++)
    {
        if(text.getString()[i] == ch)
        {
            strings.push_back(text.getString().substring(lastInd, i));
            lastInd = i + 1;
        }
        if(i == text.getString().getSize() - 1)
            strings.push_back(text.getString().substring(lastInd, i + 1));
    }

    return strings;
}