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

    // enum used for train movement (next or prev)
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

        // in case of junctions containing all 4 directions
        std::vector<Rail*> children;
        // index in the map grid
        glm::vec2 idxs;

        Rail* next = nullptr;
        Rail* prev = nullptr;

        // used when adding rails in the history (so that the wagon has the same rotation as the train when passing)
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
        // used for handling the train stop (the train does not move when true)
        // set true by Map instance and false (after some time) by the station
        bool stop = false;

        // total game time
        float time = 0.0f;

        // completed orders (score)
        int completed = 0;

        // last key pressed for an action (cleared after every junction automatically)
        int action = -1;

        // compared with the order from the central station (for delivering the order)
        std::vector<int> cargo;
        // wagons that contain information for rendering
        std::vector<Wagon> wagons;

        // handling wagon movement
        std::vector<Rail*> railHist;
        std::unordered_map<Rail*, int> railIndexMap;
        std::unordered_map<Rail*, TrainDir> railDirMap;

    };

    // tile on the grid map
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

        // used for generating the map
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

        // determine next rail segment based on key pressed and train rotation angle
        Rail* NextRailAction();

        // junction that accept an action to change direction (not corners)
        bool IsRealJunction(int type);

        // checking if the train is in the proximity of a station 
        void CheckTrainStop();

        // called every frame for automatic movement of train
        void MoveTrain(float deltaTime);

        ~Map();

        void UpdateKeyPressed(int key);
        bool IsTrainNearStation(Station* station);

        // if the order time expired => endgame
        bool EndGame();

        // number of completed orders in current round/game
        int getScore();

        bool EndMenuEnded();

        // total time
        float getTime();

        // time until order deadline
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

        static GameMenu endGameMenu;

        bool endMenuTrigger = false;

        int stations;

        //sed to randomly generate product stations in the four predefined positions
        std::vector<int> remainingIndices = { 1, 2, 3, 4 };

        // index used to determine the station where the stop occurred,
        // in order to call the method for that station
        int stationStopIdx = -1;

        // radius around the station for stopping
        float stopRadius = 15.0f;

    };

}  // namespace trainMap