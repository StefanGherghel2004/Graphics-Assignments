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

    // metoda apelata la fiecare cadru pentru actualizarea statiilor
    void UpdateStation(float time);

    // apelata de UpdateStation() daca statia este centrala
    void HandleOrders(float time);

    // factor folosit pentru modificarea culorii in shader a garii centrale cu cat ramane mai putin timp de facut comanda
    float getAlertFactor();

    // generarea unei comenzi cu simboluri 5 simboluri aleatorii din cele 4 disponibile
    void GenerateOrder();

    void HandleTrainStop(trainMap::Train* train, float time);

    int getCounter() { return counter; };
    glm::vec2 getPos() { return centerPosition; };
    string getSymbolMesh() { return symbolMesh; };
    float getAngle() { return angle; };
    std::vector<int>& getOrder() { return order; };
    bool isCentralStation() { return centralStation; };

    bool EndGame();

    // returneaza mesh-ul pentru randare obisnuita sau cel pentru minimap
    string getMesh(bool miniMap = false);

    // verifica daca am completat comanda
    bool CheckOrderDone();

    float getInterval() { return interval; };
    float getTimer() { return timer; };


private:

protected:
    glm::vec2 centerPosition;

    // actualizat la fiecare cadru pentru ambele tipuri de gara
    float timer = 0.0f;
    // folosit pentru regenerare de resurse la o gara normala si pentru limita de asteptare la o comanda pentru gara centrala
    float interval;

    // folosit doar de catre gara de materiale pentru a determina daca are sau nu disponibil simbolul (practic un bool)
    int counter = 0;

    bool centralStation;

    string stationMesh;
    string symbolMesh;

    float angle;

    // variabile folosite pentru gestionarea opririlor in statie
    float inStationtimer = 0.0f;
    float inStationInterval;

    // 1, 2,3, 4 corespunzator celor 4 simboluri
    float cargoType;

    // folosit doar de statia centrala
    bool endGame = false;

    // variabila statica de comanda folosita doar de statia centrala
    static std::vector<int> order;

};
