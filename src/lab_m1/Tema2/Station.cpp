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

        // 60 de secunde initial pentru o comanda
        interval = 60.0f;
       
        // momentan nu e utilizat pentru statia centrala
        inStationInterval = 2.0f;
    }
    else {
        // la 5 secunde este disponibil simbolul din nou si stationeaza 1 secunda
        interval = 5.0f;
        inStationInterval = 1.0f;
    }
}

Station::~Station()
{

}


void Station::UpdateStation(float time)
{
    // daca e statie centrala nu creeaza resurse
    if (centralStation) {
        HandleOrders(time);
        return;
    }

    timer += time;

    if (timer >= interval) {
        timer -= interval;
        // are disponibil simbolul ei
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
    // timpul se reseteaza la generarea unei noi comenzi
    timer = 0;
    for (int i = 0; i < 5; i++) {
        order[i] = rand() % 4 + 1;
    }
}


void Station::HandleTrainStop(trainMap::Train* train, float time)
{
    // logica pentru gara producatoare
    if (!centralStation) {

        // se opreste doar un cadru practic in cazul asta (resursa nu e disponibila)
        if (counter < 1) {
            // trenul poate sa porneasca la drum din nou
            train->stop = false;
            train->action = -1;
            return;
        }

        int cargoCnt = std::count(train->cargo.begin(), train->cargo.end(), cargoType);
        int orderCnt = std::count(order.begin(), order.end(), cargoType);

        // nu lasam trenul sa preia o resursa de care nu are nevoie gara centrala
        if (cargoCnt + 1 > orderCnt) {
            // trenul poate sa porneasca la drum din nou
            train->stop = false;
            train->action = -1;
            return;
        }

        inStationtimer += time;

        // finalizarea stationarii => primim resursa, adaugam noul vagon
        if (inStationtimer > inStationInterval) {
            counter = 0;
            timer = 0.0f;
            inStationtimer -= inStationInterval;
            // trenul poate sa porneasca la drum din nou
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

            // pe baza unghiului stabilim pozitia de inceput (miscarea e gestionata de metoda moveTrain())
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
        // momentan nu opreste in gara centrala (un timp vizibil)
        train->stop = false;
        train->action = -1;

        // gestionam eliminarea simbolurilor din comanda care au fost aduse
        for (int i = 0; i < train->cargo.size(); ) {

            bool removed = false;

            for (int j = 0; j < order.size(); j++) {

                if (train->cargo[i] == order[j]) {
                    // un element lipsa este inlocuit cu -1
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

        // daca comanda a fost completata actualizam comenzile completate si timpul pentru comanda scade cu 1 secunda
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
