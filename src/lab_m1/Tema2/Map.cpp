#include "lab_m1/Tema1/Grid.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/Tema2/Map.h"

using namespace std;
using namespace grid;
using namespace mesh;
using namespace trainMap;

GameMenu trainMap::Map::endGameMenu;

Map::Map()
{

}

Map::Map(int rows, int cols, float cellSize, glm::vec3 center, vector<string> meshNames)
    : N(rows), M(cols), gridMatrix(rows, vector<Tile>(cols)), stations(5)
{

    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(nullptr));
        seeded = true;
    }

    float startX = center.x - (cols * cellSize) / 2.0f;
    float startY = center.y - (rows * cellSize) / 2.0f;

	// initialize the map grid with grass
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {

            Tile& tile = gridMatrix[i][j];
            InteractObj& cell = tile.terrain;

            cell.position = glm::vec2(startX + j * cellSize, startY + i * cellSize);
            cell.size = glm::vec2(cellSize, cellSize);
            cell.center = cell.position + cell.size * 0.5f;

            cell.meshName = "grass";
            cell.depth = 0;
            cell.gridI = i;
            cell.gridJ = j;

            tile.rail = nullptr;
        }
    }

    // mountains
    int mountainPatches = 6 + rand() % 5;
    for (int k = 0; k < mountainPatches; k++) {
        int i = rand() % N;
        int j = rand() % M;
        GenerateTerrain(i, j, 4 + rand() % 7, "mountain");
    }

    // lakes
    int lakesPatches = 1 + rand() % 3;
    for (int k = 0; k < lakesPatches; k++) {
        int i = rand() % N;
        int j = rand() % M;
        GenerateTerrain(i, j, 6 + rand() % 7, "water");
    }

    PlaceTrainStation(23, 12, 90);
    PlaceTrainStation(40, 20, 0);
    PlaceTrainStation(5, 10, 0);
    PlaceTrainStation(32, 33, 0);
    PlaceTrainStation(24, 49, 90);

    GenerateRiver();

    CreateTrainPath();

    train = new Train();
    // the initial train position 
    train->rail = getTileRail(3, 5);
    train->position = train->rail->start;

}

void Map::GenerateRiver()
{
    int j = rand() % M;

    for (int i = 0; i < N; i++) {

        gridMatrix[i][j].terrain.meshName = "water";

        if (j + 1 < M) {
            gridMatrix[i][j + 1].terrain.meshName = "water";
        }

        if (j - 1 >= 0) {
            gridMatrix[i][j - 1].terrain.meshName = "water";
        }

		// left - right randomised deviation
        int dir = rand() % 3 - 1;
        j = glm::clamp(j + dir, 1, M - 2);
    }
}

void Map::GenerateTerrain(int centerI, int centerJ, int radius, string mesh)
{
    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {

            int ni = centerI + i;
            int nj = centerJ + j;

            if (ni < 0 || nj < 0 || ni >= N || nj >= M)
                continue;

            float dist = sqrtf(float(i * i + j * j));
            if (dist <= radius) {
                gridMatrix[ni][nj].terrain.meshName = mesh;
            }
        }
    }
}


void Map::CreateTrainPath()
{
   
    CreateTrackLine( glm::vec2(4, 29), 1, 14);
    CreateTrackLine(glm::vec2(19, 29), 1, 11);
    CreateTrackLine( glm::vec2(31, 29), 1, 12);
    CreateTrackLine(glm::vec2(4, 44), 1, 14);
    CreateTrackLine(glm::vec2(19, 44), 1, 11);
    CreateTrackLine(glm::vec2(31, 38), 1, 12);
    CreateTrackLine(glm::vec2(31, 53), 1, 12);
    CreateTrackLine(glm::vec2(19, 53), 1, 11);
    CreateTrackLine(glm::vec2(3, 30), 0, 14);
    CreateTrackLine(glm::vec2(18, 30), 0, 14);
    CreateTrackLine(glm::vec2(18, 45), 0, 8);
    CreateTrackLine(glm::vec2(30, 30), 0, 14);
    CreateTrackLine(glm::vec2(30, 45), 0, 8);
    CreateTrackLine(glm::vec2(43, 30), 0, 23);
    CreateTrackLine(glm::vec2(3, 3), 0, 18);
    CreateTrackLine(glm::vec2(12, 3), 0, 18);
    CreateTrackLine(glm::vec2(3, 22), 0, 7);
    CreateTrackLine(glm::vec2(18, 17), 0, 12);
    CreateTrackLine(glm::vec2(30, 17), 0, 12);
    CreateTrackLine(glm::vec2(43, 3), 0, 26);
    CreateTrackLine(glm::vec2(4, 2), 1, 8);
    CreateTrackLine(glm::vec2(13, 2), 1, 30);
    CreateTrackLine(glm::vec2(4, 21), 1, 26);
    CreateTrackLine(glm::vec2(37, 10), 1, 6);
    CreateTrackLine(glm::vec2(36, 3), 0, 26);
    CreateTrackLine(glm::vec2(26, 25), 1, 4);
    CreateTrackLine(glm::vec2(25, 26), 0, 3);
    CreateTrackLine(glm::vec2(19, 16), 1, 11);

	// types 0, 2, 3, 4 are corners
	// types 1, 5 are intersections

    CreateJunction(3, 2, 4);
    CreateJunction(3, 29, 1);
    CreateJunction(3, 44, 2);
    CreateJunction(12, 2, 5);
    CreateJunction(12, 21, 5);
    CreateJunction(18, 21, 1);
    CreateJunction(3, 21, 1);
    CreateJunction(36, 2, 5);
    CreateJunction(36, 10, 1);
    CreateJunction(36, 29, 5);
    CreateJunction(43, 10, 1);
    CreateJunction(30, 38, 1);
    CreateJunction(43, 38, 1);
    CreateJunction(18, 16, 4);
    CreateJunction(18, 29, 1);
    CreateJunction(18, 44,  1);
    CreateJunction(18, 53, 2);
    CreateJunction(30, 16, 3);
    CreateJunction(30, 29, 1);
    CreateJunction(30, 44, 1);
    CreateJunction(30, 53, 5);
    CreateJunction(30, 21, 1);
    CreateJunction(25, 25, 4);
    CreateJunction(25, 29, 5);
    CreateJunction(30, 25, 1);
    CreateJunction(43, 2, 3);
    CreateJunction(43, 29, 1);
    CreateJunction(43, 53, 0);

}


void Map::CreateTrackLine(glm::vec2 startIdx, int direction,  int length)
{
    int startI = startIdx.x;
    int startJ = startIdx.y;

    Rail* prev = nullptr;

    for (int k = 0; k < length; k++) {

        int i = startI + (direction == 1 ? k : 0);
        int j = startJ + (direction == 0 ? k : 0);

        Tile& tile = gridMatrix[i][j];
        InteractObj& cell = tile.terrain;

        tile.rail = new Rail();
        Rail* curr = tile.rail;

        if (direction == 0) { // orizontal
            curr->start = glm::vec3(cell.position.x, 0.0f, cell.center.y - cell.size.x / 2.0f);
            curr->end = glm::vec3(cell.position.x + cell.size.x, 0.0f, cell.center.y - cell.size.x / 2.0f);
            curr->rotated = true;
        }
        else { // vertical
            curr->start = glm::vec3(cell.center.x - cell.size.y / 2.0f, 0.0f, cell.position.y);
            curr->end = glm::vec3(cell.center.x - cell.size.y / 2.0f, 0.0f, cell.position.y + cell.size.y);
            curr->rotated = false;
        }

        curr->idxs = glm::vec2(i, j);

        curr->next = nullptr;
        curr->children.clear();

		// connecting with the previous for both ways movement
        if (prev) {
            prev->next = curr;
            curr->prev = prev;
        }

        prev = curr;
    }

}


void Map::CreateJunction(int i, int j, int type)
{
    Tile& tile = gridMatrix[i][j];
    if (!tile.rail) {
        tile.rail = new Rail();
    }

    tile.rail->start = glm::vec3(tile.terrain.position.x, 0, tile.terrain.center.y - tile.terrain.size.x / 2.0f);
    tile.rail->end = glm::vec3(tile.terrain.position.x, 0, tile.terrain.center.y - tile.terrain.size.x / 2.0f);
    tile.rail->junction = true;
    tile.rail->idxs = glm::vec2(i, j);

    if (type == 0) {
        tile.rail->next = getTileRail(i  - 1, j);
        tile.rail->prev = getTileRail(i, j - 1);
        if (getTileRail(i - 1, j)) {
            getTileRail(i - 1, j)->next = tile.rail;

        }

        if (getTileRail(i, j - 1)) {
            getTileRail(i, j - 1)->next = tile.rail;

        }
    }
    else if (type == 1) {
        tile.rail->next = getTileRail(i, j + 1);
        tile.rail->prev = getTileRail(i, j - 1);

        if (getTileRail(i - 1, j)) {
            getTileRail(i - 1, j)->next = tile.rail;
        }
        tile.rail->children.push_back(getTileRail(i - 1, j));

        if (getTileRail(i, j + 1)) {
            getTileRail(i, j + 1)->prev = tile.rail;
        }
        tile.rail->children.push_back(getTileRail(i, j + 1));


        if (getTileRail(i + 1, j)) {
            getTileRail(i + 1, j)->prev = tile.rail;
        }

        tile.rail->children.push_back(getTileRail(i + 1, j));


        if (getTileRail(i, j - 1)) {
            getTileRail(i, j - 1)->next = tile.rail;
        }

        tile.rail->children.push_back(getTileRail(i, j - 1));


    }
    else if (type == 2) {
        tile.rail->next = getTileRail(i + 1, j);
        tile.rail->prev = getTileRail(i, j - 1);
        if (getTileRail(i, j - 1)) {
            getTileRail(i, j - 1)->next = tile.rail;

        }

        if (getTileRail(i + 1 , j)) {
            getTileRail(i + 1, j)->prev = tile.rail;

        }
    }
    else if (type == 3) {
        tile.rail->next = getTileRail(i, j + 1);
        tile.rail->prev = getTileRail(i - 1, j);
        if (getTileRail(i, j + 1)) {
            getTileRail(i, j + 1)->prev = tile.rail;

        }

        if (getTileRail(i - 1, j)) {
            getTileRail(i - 1, j)->next = tile.rail;

        }
    }
    else if (type == 4) {
        tile.rail->next = getTileRail(i, j + 1);
        tile.rail->prev = getTileRail(i + 1, j);
        if (getTileRail(i, j + 1)) {
            getTileRail(i, j + 1)->prev = tile.rail;

        }

        if (getTileRail(i + 1, j)) {
            getTileRail(i + 1, j)->prev = tile.rail;

        }

    } 
    else if (type == 5) {
        tile.rail->next = getTileRail(i + 1, j);
        tile.rail->prev = getTileRail(i - 1, j);

        if (getTileRail(i - 1, j)) {
            getTileRail(i - 1, j)->next = tile.rail;
        }
        tile.rail->children.push_back(getTileRail(i - 1, j));

        if (getTileRail(i, j + 1)) {
            getTileRail(i, j + 1)->prev = tile.rail;
        }
        tile.rail->children.push_back(getTileRail(i, j + 1));


        if (getTileRail(i + 1, j)) {
            getTileRail(i + 1, j)->prev = tile.rail;
        }

        tile.rail->children.push_back(getTileRail(i + 1, j));


        if (getTileRail(i, j - 1)) {
            getTileRail(i, j - 1)->next = tile.rail;
        }

        tile.rail->children.push_back(getTileRail(i, j - 1));
    }


    tile.rail->type = type;

}



void Map::PlaceTrainStation(int i, int j, float angle)
{
    if (stations < 1) {
        return;
    }

    Tile& tile = gridMatrix[i][j];
    int index = 0;

    if (stations == 5) {
        index = 0;
    }
    else {
        int randPos = rand() % remainingIndices.size();
        index = remainingIndices[randPos];

        remainingIndices.erase(remainingIndices.begin() + randPos);        
    }

    tile.stationMesh = "station" + std::to_string(index);
    tile.station = true;

    Station* station = new Station(glm::vec2(tile.terrain.center), index == 0, tile.stationMesh, angle);
    stationsList.push_back(station);
    stations--;

    // grass under the station
    GenerateTerrain(i, j, 6, "grass");
}


glm::vec2 Map::getTrainPos()
{
    return glm::vec2(train->position.x, train->position.z);
}


Rail* Map::getTrainRail()
{
    return train->rail;
}


Train* Map::getTrain()
{
    return train;
}


void Map::MoveTrain(float deltaTime)
{

    train->time += deltaTime;

    if (!train || !train->rail)
        return;

    if (train->stop) {
        stationsList[stationStopIdx]->HandleTrainStop(train, deltaTime);
        return;
    }

    train->progress += deltaTime * train->speed;

	// used for wagons to move to the next rail
    bool next = false;

    if (train->progress >= 1.0f) {

        // needed so that the wagons follow the train
        next = true;
        train->rail->angle = train->angle;
        train->railHist.push_back(train->rail);
        train->railIndexMap[train->rail] = train->railHist.size() - 1;
        train->railDirMap[train->rail] = train->dir;
        train->progress = 0.0f;

        Rail* nextRail = nullptr;

		// initial next based on train direction (can be changed by junction)
        if (train->dir == FORWARD)
            nextRail = train->rail->next;
        else
            nextRail = train->rail->prev;

        if (nextRail) {

			// saving the last rail before junction for determining the orientation of the control buttons 
            // (to know what W A S D means for the train in every case)
            if (nextRail->junction) {
                lastRail = train->rail;
            }

            if (train->rail->junction) {

                Rail* realJunctionNext = nullptr;

                if (IsRealJunction(train->rail->type)) {
                    realJunctionNext = NextRailAction();
                    
                }

                if (realJunctionNext) {
                    nextRail = realJunctionNext;
                }

                train->action = -1;

                // the logic of turns for correctly changing direction in both ways(at corners)
                
                if (nextRail == lastRail) {
                    if (nextRail == train->rail->next) {
                        nextRail = train->rail->prev;
                    }
                    else if (nextRail == train->rail->prev) {
                        nextRail = train->rail->next;
                    }
                }

                glm::ivec2 curr = train->rail->idxs;
                glm::ivec2 next = nextRail->idxs;

                // determine the rotation of the train after the junction
                if (train->angle == 0.0f || train->angle == 180.0f) {

                    if (curr.y > next.y) {
                        train->angle = 270.0f;
                    }
                    else if (curr.y < next.y) {
                        train->angle = 90.0f;
                    }
                }

                else if (train->angle == 90.0f || train->angle == 270.0f) {

                    if (curr.x > next.x) {
                        train->angle = 180.0f;
                    }
                    else if (curr.x < next.x) {
                        train->angle = 0.0f;
                    }
                }
                
                // determine the direction (meaning advancing on next or prev) based on rotation angle
                if (train->angle == 0.0f || train->angle == 90.0f) {
                    train->dir = FORWARD;
                }
                else {
                    train->dir = BACKWARD;
                }

                

            }

            train->rail = nextRail;
            
        }
        
    }

	// interpolate position based on direction of movement
    if (train->dir == FORWARD)
        train->position = glm::mix(train->rail->start, train->rail->end, train->progress);
    else
        train->position = glm::mix(train->rail->end, train->rail->start, train->progress);


    for (int i = 0; i < train->wagons.size(); i++) {


        Wagon& w = train->wagons[i];
        w.dir = train->railDirMap[w.rail];

		// if the train has moved to the next rail section the wagons are moving to it as well
        if (next) {

            int idx = train->railIndexMap[w.rail];
            int nextIdx = idx + 1;

            if (nextIdx >= train->railHist.size())
                nextIdx = train->railHist.size() - 1;

            w.rail = train->railHist[nextIdx];
        }

        // interpolate position based on direction of movement
        if (w.dir == FORWARD)
            w.position = glm::mix(w.rail->start, w.rail->end, train->progress);
        else
            w.position = glm::mix(w.rail->end, w.rail->start, train->progress);
    }

    // checking if a stop was triggered in the proximity of a station
    CheckTrainStop();

}


Map::~Map()
{

}


void Map::UpdateKeyPressed(int key)
{
    train->action = key;
}

bool Map::IsTrainNearStation(Station* station)
{
    return glm::distance(glm::vec3(getTrainPos().x, 0, getTrainPos().y), glm::vec3(station->getPos().x, 0, station->getPos().y)) < stopRadius;
}


bool Map::EndGame()
{
    // calling EndGame() activates the endMenu if the game ended (based on the central station state)
    if (getCentralStation()->EndGame() && !endMenuTrigger) {
        endGameMenu.setActive();
        endMenuTrigger = true;
    }

    return endMenuTrigger;
}

int Map::getScore()
{
    return train->completed;
}


bool Map::EndMenuEnded()
{
    // calling this after trigger is true
    return !endGameMenu.IsActive();
}

float Map::getTime()
{
    return train->time;
}

float Map::getRemainingTime()
{
    return getCentralStation()->getInterval() - getCentralStation()->getTimer();
}

Rail* Map::getTileRail(int i, int j)
{
    if (i < 0 || j < 0 || i >= N || j >= M) {
        return nullptr;
    }

    return gridMatrix[i][j].rail;
};


Rail* Map::NextRailAction()
{

    Rail* next = nullptr;

    if (train->angle == 0.0f) {

        if (train->action == GLFW_KEY_LEFT || train->action == GLFW_KEY_A) {
            next = train->rail->children[1];
        }

        if (train->action == GLFW_KEY_RIGHT || train->action == GLFW_KEY_D) {
            next = train->rail->children[3];
        }

        if (train->action == GLFW_KEY_UP || train->action == GLFW_KEY_W) {
            next = train->rail->children[2];
        }
    }

    if (train->angle == 90.0f) {

        if (train->action == GLFW_KEY_LEFT || train->action == GLFW_KEY_A) {
            next = train->rail->children[0];
        }

        if (train->action == GLFW_KEY_RIGHT || train->action == GLFW_KEY_D) {
            next = train->rail->children[2];
        }

        if (train->action == GLFW_KEY_UP || train->action == GLFW_KEY_W) {
            next = train->rail->children[1];
        }
    }

    if (train->angle == 180.0f) {

        if (train->action == GLFW_KEY_LEFT || train->action == GLFW_KEY_A) {
            next = train->rail->children[3];
        }

        if (train->action == GLFW_KEY_RIGHT || train->action == GLFW_KEY_D) {
            next = train->rail->children[1];
        }

        if (train->action == GLFW_KEY_UP || train->action == GLFW_KEY_W) {
            next = train->rail->children[0];
        }
    }

    if (train->angle == 270.0f) {

        if (train->action == GLFW_KEY_LEFT || train->action == GLFW_KEY_A) {
            next = train->rail->children[2];
        }

        if (train->action == GLFW_KEY_RIGHT || train->action == GLFW_KEY_D) {
            next = train->rail->children[0];
        }

        if (train->action == GLFW_KEY_UP || train->action == GLFW_KEY_W) {
            next = train->rail->children[3];
        }
    }

    return next;
}


bool Map::IsRealJunction(int type) {
    return type == 1 || type == 5;
}

void Map::CheckTrainStop()
{
    int i = 0;
    for (Station* station : stationsList) {

        if (IsTrainNearStation(station) && (train->action == GLFW_KEY_DOWN || train->action == GLFW_KEY_S)) {
            // the flag will be set back to false when the stop is completed (by the station instance)
            train->stop = true;
            stationStopIdx = i;
            return;
        }

        i++;
    }
}