#include "lab_m1/Tema1/Chicken.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/Tema1/Grid.h"

using namespace std;
using namespace mesh;

Chicken::Chicken(glm::vec2 position, float radius, float speed, float period, std::string type)
		: centerPosition(position), hitBoxRadius(radius), eggSpeed(speed), eggType("egg"), eggPeriod(period), chickType("chickenBody"), headType("chickenHead"), wingsType("chickenWing"), legType("chickenLeg")
{
    eggTimer = static_cast<float>(rand()) / RAND_MAX * eggPeriod;
    // Spawning a chicken with a certain probability.
    // This chicken will lay eggs at a longer interval, but the eggs will be faster
    // and have a certain property.

    if (rand() % 30 == 0) {
        chickType = "chickenBodyRed";
        eggType = "eggGold";
        headType = "chickenHeadGold";
        wingsType = "chickenWingGold";
        legType = "chickenLegGold";
    }
}

Chicken::~Chicken()
{

}

void Chicken::LayEgg(float deltaTime, std::vector<InteractObj>& eggs)
{
    eggTimer += deltaTime;
    if (eggType == "egg" && eggTimer > eggPeriod) {
        eggs.push_back({centerPosition, glm::vec2(10, 16), glm::vec2(0), eggType, 0});
        eggTimer = 0;
    }

    // golden eggs spawn less often because they offer a benefit
    if (eggType == "eggGold" && eggTimer > eggPeriod * 1.2) {
        eggs.push_back({ centerPosition, glm::vec2(10, 16), glm::vec2(0), eggType, 0 });
        eggTimer = 0;
    }

}

void Chicken::UpdatePosX(float dist)
{ 
    centerPosition.x += dist;
    
}

void Chicken::UpdatePosY(float dist)
{
    centerPosition.y += dist;

}

void Chicken::UpdatePos(glm::vec2 trans)
{
    centerPosition += trans;
}
