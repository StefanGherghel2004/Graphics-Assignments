#include "LateralWave.h"
#include <glm/glm.hpp>
#include <cmath>
#include <iostream>

using namespace std;

LateralWave::LateralWave(float formationTime)
    : ChickenWave(formationTime, false)
{

    movingDir = 1;
    radians = 0;
    FormationInitPos();
}

void LateralWave::WaveForming(float deltaTime)
{
    if (formationEnd) return;

    if (formationTimer == 0) {
        radians = 0;
    }

    formationTimer += deltaTime;

    float horizontalSpeed = 216;
    float amplitude = 70.0f;
    float waveTime = radians;

    for (auto& chicken : left)
    {
        float offsetY = sinf(waveTime + phase[chicken]) * amplitude;

        chicken->setY(baseY[chicken] + offsetY);
        chicken->UpdatePosX(horizontalSpeed * deltaTime);
    }

    for (auto& chicken : right)
    {
        float offsetY = sinf(waveTime + phase[chicken]) * amplitude;

        chicken->setY(baseY[chicken] + offsetY);
        chicken->UpdatePosX(-horizontalSpeed * deltaTime);

    }

    //radians += 4 * deltaTime;

    radians += 0.1;

    if (formationTimer >= formationTime)
    {
        formationEnd = true;
        formationTimer = 0;
        radians = 0;
    }

}

void LateralWave::FormationMove(float deltaTime)
{
    if (!formationEnd) return;

    if (formationTimer == 0) {
        radians = glm::pi<float>() / 2.0f;
    }

    if (formationTimer < 0.8) {
        formationTimer += deltaTime;
        return;
    }


    for (auto& chicken : chickens) {
        chicken->UpdatePos(glm::vec2(4 * sinf(radians), -movingDir * chickenSpeed * deltaTime));
    }


    radians += 0.06;
}

void LateralWave::FormationInitPos(float logicWidth, float logicHeigth)
{

    float waveAmplitude = 70.0f;

    float gap = 1.57 / 2.0f;


    // in functie de numRows / 3 + 1 + numFullRows vom avea un numar in crestere de fiecare data de valuri in forma de ADN
    // 10 gaini din stanga si 8 din dreapta

    for (int j = 0; j < numRows / 3 + 1 + numfullRows; j++) {

        for (int i = 0; i < 9; i++) {
            float phaseOffset = i * gap;
            float x = -880 + 110 * i;
            float y = 550 + j * 400 + sinf(phaseOffset) * waveAmplitude;
            Chicken* chicken = new Chicken(glm::vec2(x, y), 25, eggSpeed, eggPeriod, "egg");
            chickens.push_back(chicken);
            left.push_back(chicken);

            // informatii necesare pentru a urma sinusoida creata in waveForming()
            phase[chicken] = phaseOffset;
            baseY[chicken] = 550 + j * 400;


        }
    }

    for (int j = 0; j < numRows / 3 + 1 + numfullRows; j++) {
        for (int i = 0; i < 9; i++) {
            float phaseOffset = i * gap + 2.7;
            float x = 1280 + 110 * i;
            float y = 550 + j * 400 + sinf(i * gap) * waveAmplitude;
            if (i != 2 && i != 6) {
                Chicken* chicken = new Chicken(glm::vec2(x, y), 25, eggSpeed, eggPeriod, "egg");
                chickens.push_back(chicken);
                right.push_back(chicken);

                // informatii necesare pentru a urma sinusoida creata in waveForming()
                phase[chicken] = phaseOffset;
                baseY[chicken] = 550 + j * 400;
            }
        }
    }
}

LateralWave::~LateralWave()
{

}
