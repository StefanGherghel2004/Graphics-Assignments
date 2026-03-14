#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <iostream>
#include "lab_m1/Tema1/meshObj.h"

using namespace mesh;
using namespace std;

class Chicken
{
public:
    Chicken(glm::vec2 position, float radius, float eggSpeed, float eggPeriod, std::string eggType);

    ~Chicken();

    void LayEgg(float deltaTime, std::vector<InteractObj>& eggs);

    void UpdatePosX(float dist);

    void UpdatePosY(float dist);

    void UpdatePos(glm::vec2 trans);

    float getX() { return centerPosition.x; };
    float getY() { return centerPosition.y; };
    glm::vec2 getPos() { return centerPosition; };
    float getRadius() { return hitBoxRadius; };
    float getEggSpeed() { return eggSpeed; };
    void setY(float y) { centerPosition.y = y; };


    std::string getMeshBody() { return chickType; };
    std::string getMeshHead() { return headType; };
    std::string getMeshWing() { return wingsType; };
    std::string getMeshLeg() { return legType; };

private:

protected:
    glm::vec2 centerPosition;

    float hitBoxRadius;
    float eggSpeed;
    float eggTimer;
    float eggPeriod;

    std::string eggType; // meshName
    std::string chickType; //meshName
    std::string headType; //meshName
    std::string wingsType; //meshName
    std::string legType;  //meshName

};
