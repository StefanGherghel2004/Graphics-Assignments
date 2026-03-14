#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <iostream>
#include "lab_m1/Tema1/meshObj.h"
#include "Station.h"
#include "GameMenu.h"
#include <deque>

using namespace mesh;
using namespace std;

namespace trainMap
{

    // enum folosit pentru miscarea trenului (FORWARD -> se misca pe next, BACKWARD -> se misca pe prev)
    enum TrainDir {
        FORWARD,
        BACKWARD
    };


    struct Rail
    {
        glm::vec3 start;
        glm::vec3 end;
        bool rotated = false;
        bool junction = false;
        int type = -1;

        //  in cazul intersectiilor contine cele 4 directii posibile
        std::vector<Rail*> children;
        // indexul din grid-ul hartii (redundant pentru ca se afla si in Tile->terrain dar pentru acces din rail direct e folositor)
        glm::vec2 idxs;

        Rail* next = nullptr;
        Rail* prev = nullptr;

        // folosit atunci cand trenul adauga sine in istoricul pentru vagoane (vagonul sa preia rotatia trenului cand a trecut pe acolo)
        float angle = 0.0f;
    };

    struct Wagon {
        glm::vec3 position;
        float angle = 0.0f;
        Rail* rail;
        TrainDir dir = FORWARD;
    };

    struct Train
    {
        Rail* rail;
        float progress = 0.0f;
        float speed = 10.0f;
        float angle = 270.0f;
        glm::vec3 position;

        TrainDir dir = BACKWARD;
        // folosit pentru a modela oprirea in statie (trenul nu se misca cat este true)
        // setat true de catre instanta de Map si false la finalulo opririi de catre statie
        bool stop = false;

        // folosit pentru afisarea timpului total de joc
        float time = 0.0f;

        // comenzi livrate complet
        int completed = 0;

        // se pune tasta apasata
        int action = -1;

        // vectorul ce se compara cu comanda din gara centrala
        std::vector<int> cargo;
        // vagoanele efective ce contin informatii pentru randare
        std::vector<Wagon> wagons;

        // folosite pentru gestionarea miscariiv vagoanelor
        std::vector<Rail*> railHist;
        std::unordered_map<Rail*, int> railIndexMap;
        std::unordered_map<Rail*, TrainDir> railDirMap;

    };

    // o patratica de pe harta
    struct Tile
    {
        InteractObj terrain;
        string stationMesh;
        bool station = false;
        Rail* rail = nullptr;
    };

    class Map
    {
    public:
        Map();
        Map(int rows, int cols, float size, glm::vec3 center, vector<string> meshName);

        // metode utilizate pentru generarea hartii
        void GenerateRiver();
        void GenerateTerrain(int centerI, int centerJ, int radius, string mesh);
        void CreateTrainPath();
        void CreateTrackLine(glm::vec2 startIdx, int direction, int length);
        void CreateJunction(int i, int j, int initialDir);
        void PlaceTrainStation(int i, int j, float angle);

        glm::vec2 getTrainPos();
        Rail* getTrainRail();
        Train* getTrain();
        Rail* getTileRail(int i, int j);

        // determina urmatorul segment de sina pe baza tastei apasate si a rotatiei trenului
        Rail* NextRailAction();

        // intersectii in care poti schimba directia (nu colturi) 
        bool IsRealJunction(int type);

        // verifica daca trenul s-a oprit in proximitatea unei statii si actualizeaza id-ul pentru statie
        void CheckTrainStop();

        // metoda apelata la fiecare frame pentru miscarea automata a trenului
        void MoveTrain(float deltaTime);

        ~Map();

        void UpdateKeyPressed(int key);
        bool IsTrainNearStation(Station* station);

        // daca comanda a expirat => endgame
        bool EndGame();

        // numarul de comenzi completate in runda curenta
        int getScore();

        bool EndMenuEnded();

        // timpul total in runda de joc curenta
        float getTime();

        // timpul ramas pana cand expira comanda actuala
        float getRemainingTime();

        int N;
        int M;

        InteractObj& getTileInteract(int i, int j) { return  gridMatrix[i][j].terrain; };
        string getStationMesh(int i, int j) { return  gridMatrix[i][j].stationMesh; };
        vector<Station*>& getStations() { return stationsList; };
        Station* getCentralStation() { return stationsList[0]; };
        GameMenu* getMenu() { return &endGameMenu; };

    private:


    protected:
        vector<vector<Tile>> gridMatrix;
        vector<Station*> stationsList;
            
        Rail* trainPos = nullptr;
        Train* train;
        Rail* lastRail = nullptr;

        // meniul de final unic al jocului
        static GameMenu endGameMenu;

        bool endMenuTrigger = false;

        int stations;

        // folosit pentru generarea de statii de produse in mod aleator in cele 4 pozitii prestabilite
        std::vector<int> remainingIndices = { 1, 2, 3, 4 };

        // index pentru selectarea statiei la care s-a facut oprirea pentru apelarea metodei ce se ocupa de aceasta
        int stationStopIdx = -1;

        // raza in jurul statiei pentru oprire
        float stopRadius = 15.0f;

    };

}  // namespace trainMap