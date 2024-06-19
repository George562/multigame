#pragma once
#include "../../nlohjson/json.hpp"
using nlohmann::json;

class Serializable {
public:
    virtual void readJSON(json&) = 0;
    virtual json writeJSON() = 0;
};