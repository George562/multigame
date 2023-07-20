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

std::vector<std::string> splitString(std::string text, char ch)
{
    std::vector<std::string> strings;
    int lastInd = 0;

    for(int i = 0; i < text.size(); i++)
    {
        if(text[i] == ch)
        {
            strings.push_back(text.substr(lastInd, (i - lastInd)));
            lastInd = i + 1;
        }
        if(i == text.size() - 1)
            strings.push_back(text.substr(lastInd, i + 1));
    }

    return strings;
}

std::string fitText(std::string text, int characterSize, float width, float height)
{
    if(text.empty())
        return "";

    std::vector<std::string> lines = splitString(text, ' ');
    std::string transformText = "";
    int cumulativeLength = 0;

    for(std::string s : lines)
    {
        if((cumulativeLength + s.size() + 1) * characterSize >= width)
        {
            if(cumulativeLength == 0)
            {
                transformText.append(s.substr(0, width / characterSize) + "\n" + s.substr(width / characterSize));
                continue;
            }
            cumulativeLength = 0;
            transformText.append("\n" + s + " ");
            continue;
        }
        cumulativeLength += s.size() + 1;
        transformText.append(s + " ");
    }

    return transformText;
}