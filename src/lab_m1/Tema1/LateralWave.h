#pragma once

#include "ChickenWave.h"
#include <vector>
#include <iostream>

class LateralWave : public ChickenWave
{
public:
    LateralWave(float formationTime);

    void WaveForming(float deltaTime) override;
    void FormationMove(float deltaTime) override;
    void FormationInitPos(float logicWidth = 1280.0f, float logicHeight = 720.0f) override;

    ~LateralWave();

protected:


    // vectori folositi pentru a controla gaininile din fiecare parte
    std::vector<Chicken*> left;
    std::vector<Chicken*> right;

    // retinem informatii despre pozitia si faza fiecarei  gaini din formatie
    std::unordered_map<Chicken*, float> baseY;
    std::unordered_map<Chicken*, float> phase;


};