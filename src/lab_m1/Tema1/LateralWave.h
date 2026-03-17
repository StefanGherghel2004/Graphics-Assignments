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

	// vectors used to control the chickens on each side
    std::vector<Chicken*> left;
    std::vector<Chicken*> right;

	// keeping track of the position and phase of each chicken in the formation
    std::unordered_map<Chicken*, float> baseY;
    std::unordered_map<Chicken*, float> phase;


};