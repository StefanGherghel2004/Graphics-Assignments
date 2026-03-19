#include "Station.h"
using namespace trainMap;
#include "Map.h"

std::vector<int> Station::order;

Station::Station(glm::vec2 position, bool central, string mesh, float angle)
		: centerPosition(position), centralStation(central), stationMesh(mesh), angle(angle)
{
    int idx = stationMesh.back() - '0';
    symbolMesh = "symbol" + std::to_string(idx);
    cargoType = idx;

    if (centralStation) {

        order.resize(5, -1);
        GenerateOrder();
        // 60 seconds for an order
        interval = 60.0f;
       
        // not used for central station
        inStationInterval = 2.0f;
    }
    else {
        // 5 seconds for station recharge and 1 second for a stop 
        interval = 5.0f;
        inStationInterval = 1.0f;
    }
}

Station::~Station()
{

}


void Station::UpdateStation(float time)
{
    // central station doesn' t produce symbols
    if (centralStation) {
        HandleOrders(time);
        return;
    }

    timer += time;

    if (timer >= interval) {
        timer -= interval;
        // symbol available
        counter = 1;
    }
}


void Station::HandleOrders(float time)
{
    timer += time;

    if (timer > interval) {
        endGame = true;
        return;
    }

    if (CheckOrderDone()) {
        GenerateOrder();
    }

}

float Station::getAlertFactor()
{
    return timer / interval;
}


void Station::GenerateOrder()
{
    // reset timer for a new order
    timer = 0;
    for (int i = 0; i < 5; i++) {
        order[i] = rand() % 4 + 1;
    }
}


void Station::HandleTrainStop(trainMap::Train* train, float time)
{

    // producer station logic
    if (!centralStation) {

        // stopping for a frame (the symbol is not available)
        if (counter < 1) {
            // the train can move 
            train->stop = false;
            train->action = -1;
            return;
        }

        int cargoCnt = std::count(train->cargo.begin(), train->cargo.end(), cargoType);
        int orderCnt = std::count(order.begin(), order.end(), cargoType);

        // do not allow the train to pick up a symbol that the central station doesn't need
        if (cargoCnt + 1 > orderCnt) {
            // the train can move 
            train->stop = false;
            train->action = -1;
            return;
        }

        inStationtimer += time;

        // stop time reached => the train has the symbol, adding the new wagon
        if (inStationtimer > inStationInterval) {
            counter = 0;
            timer = 0.0f;
            inStationtimer -= inStationInterval;
            // the train can move
            train->stop = false;
            train->action = -1;
            train->cargo.push_back(cargoType);

            Wagon newWagon;

            newWagon.rail = train->railHist.back() ;
            int numWagons = train->wagons.size();
            int histSize = train->railHist.size();

            int idx = histSize - 2 - numWagons;

            if (idx < 0)
                idx = 0;

            // choosing the start position based on angle (the movement is handled by moveTrain())
            newWagon.rail = train->railHist[idx];
            if (newWagon.rail->angle == 0 || newWagon.rail->angle == 90) {
                newWagon.position = newWagon.rail->start;
            }
            else {
                newWagon.position = newWagon.rail->end;
            }

            train->wagons.push_back(newWagon);

        }
    }
    else {
       
        // the train can leave instantly
        train->stop = false;
        train->action = -1;

        // handling removal of symbols from the order that have already been delivered
        for (int i = 0; i < train->cargo.size(); ) {

            bool removed = false;

            for (int j = 0; j < order.size(); j++) {

                if (train->cargo[i] == order[j]) {
                    // delivered symbol => -1  (empty rendering)
                    order[j] = -1;
                    train->cargo.erase(train->cargo.begin() + i);
                    train->wagons.erase(train->wagons.begin() + i);
                    removed = true;
                    break;
                }
            }

            if (!removed) {
                i++;
            }
        }

        // completed order => update the completed orders and reduce the order time by 1 second.
        if (CheckOrderDone()) {
            train->completed++;
            interval -= 1.0f;
        }
    }
}

bool Station::EndGame()
{
    return endGame;
}

string Station::getMesh(bool miniMap)
{ 

    if (miniMap) {
        return "mini_" + stationMesh;
    }

    return stationMesh;
};


bool Station::CheckOrderDone()
{
    bool allNegative = true;
    for (int i = 0; i < order.size(); i++) {
        if (order[i] >= 0) {
            allNegative = false;
            break;
        }
    }

    return allNegative;
}
