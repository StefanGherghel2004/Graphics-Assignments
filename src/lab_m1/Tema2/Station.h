#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <iostream>
#include "lab_m1/Tema1/meshObj.h"

using namespace mesh;
using namespace std;

namespace trainMap {
    struct Train;
}

class Station
{
public:

    Station(glm::vec2 position, bool central, string mesh, float angle);

    ~Station ();

    // called every frame to update station state
    void UpdateStation(float time);

    // called by UpdateStation() if central station
    void HandleOrders(float time);

    // factor used for rendering the central station in shader (less time remaining => redder)
    float getAlertFactor();

    // generate random order (5 symbols with 4 unique avalaible)
    void GenerateOrder();

    void HandleTrainStop(trainMap::Train* train, float time);

    int getCounter() { return counter; };
    glm::vec2 getPos() { return centerPosition; };
    string getSymbolMesh() { return symbolMesh; };
    float getAngle() { return angle; };
    std::vector<int>& getOrder() { return order; };
    bool isCentralStation() { return centralStation; };

    bool EndGame();

    string getMesh(bool miniMap = false);

    bool CheckOrderDone();

    float getInterval() { return interval; };
    float getTimer() { return timer; };


private:

protected:
    glm::vec2 centerPosition;

    // updated every frame
    float timer = 0.0f;

    // used for resource regeneration at a normal station and for the wait limit on an order at the central station
    float interval;

    // used by producer station to determine whether the symbol is available or not
    int counter = 0;

    bool centralStation;

    string stationMesh;
    string symbolMesh;

    float angle;

    // handling stop in station
    float inStationtimer = 0.0f;
    float inStationInterval;

    // the 4 avalaible symnbols
    float cargoType;

    // used by central station
    bool endGame = false;

    // used by central station
    static std::vector<int> order;

};
