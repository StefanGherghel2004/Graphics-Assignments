#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <iostream>
#include "lab_m1/Tema1/meshObj.h"
#include "Chicken.h"

using namespace mesh;
using namespace std;

class ChickenWave
{
public:

    ChickenWave(float formationTimem, bool initPos);

    virtual void WaveForming(float deltaTime);

    vector<Chicken*>& getChickens() { return chickens; };
    bool getFormationEnd() { return formationEnd; };
    float getChickenSpeed() { return chickenSpeed; };

    ~ChickenWave();

    virtual void FormationMove(float deltaTime);

    virtual void FormationInitPos(float logicWidth = 1280.0f, float logicHeight = 720.0f);

    static void ResetBaseValues();

    void RemoveChicken(Chicken* c);

private:

protected:

    // vectorul de gaini
    vector<Chicken*> chickens;

    // valorile pe care le incrementeaza constructorul de valuri
    static float baseChickenSpeed;
    static float baseNumRows;
    static float baseEggSpeed;
    static float baseEggPeriod;
    static float baseNumfullRows;

    // parametrii ce preiau valorile statice de mai sus sa lucreze in instanta
    float chickenSpeed;
    int   numRows;
    float eggSpeed;
    float eggPeriod;
    int numfullRows;
    float movingDir;
    float radians;
    float formationTime;
    float formationTimer;
    bool formationEnd;




};
