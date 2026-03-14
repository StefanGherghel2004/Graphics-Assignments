#include "lab_m1/Tema1/Grid.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/Tema1/Grid.h"

using namespace std;
using namespace grid;
using namespace mesh;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Grid::Grid()
{

}

Grid::Grid(int rows, int cols, float cellSize, glm::vec2 corner, std::string meshName)
    : N(rows), M(cols), gridMatrix(rows, std::vector<std::vector<InteractObj>>(cols)), hoveredBlockCoords(-1, -1), baseMeshName(meshName)
{
    drawnBlocks = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            glm::vec2 pos = glm::vec2(corner.x + j * cellSize, corner.y + i * cellSize);
            glm::vec2 size = glm::vec2(0.75f * cellSize, 0.75f * cellSize);

            InteractObj cellObj = {pos,size, glm::vec2(0), baseMeshName};

            gridMatrix[i][j].push_back(cellObj);
        }
    }
}

Grid::~Grid()
{

}

void grid::Grid::ClearDrawing()
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            while (gridMatrix[i][j].size() > 1) {
                RemoveObject(i, j);
            }
        }
    }
}

// punerea unui block nou in grid
void Grid::InsertObject(int i, int j, InteractObj object)
{
    gridMatrix[i][j].push_back(object);
    drawnBlocks++;
}

// scoatem din grid ultimul block adaugat pe aceea pozitie
void Grid::RemoveObject(int i, int j)
{
    gridMatrix[i][j].pop_back();
    drawnBlocks--;
}

bool Grid::CheckRestrictions()
{

    int cannonCount = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            for (auto& obj : gridMatrix[i][j]) {

                if (obj.meshName == "cannon") {
                    // numararea numarului de tunuri pentru ca trebuie sa avem macar un tun ca sa aiba sens jocul
                    cannonCount++;

                    // verifica ca in fata tunului sa nu mai fie plasat vreun bloc
                    if (!CheckCol(i, j, true)) {
                        return false;
                    }

                    // verifica ca sa in stanga si in dreapta pe toata coloana de la pozitia lui in sus sa in fie bumpere
                    if (!CheckCol(i, j - 1, true, true, "bumper")) {
                        return false;
                    }


                    if (!CheckCol(i, j + 1, true, true, "bumper")) {
                        return false;
                    }
                   
                }

                if (obj.meshName == "engine") {
                    // sub motor sa nu fie niciun block
                    if (!CheckCol(i, j, false)) {
                        return false;
                    }
                }


                if (obj.meshName == "bumper") {

                    // verificarea celor 3 coloane
                    if (!CheckCol(i, j, true)) {
                        return false;
                    }

                    if (!CheckCol(i, j + 1, true)) {
                        return false;
                    }

                    if (!CheckCol(i, j - 1, true)) {
                        return false;
                    }

                    // verificare pozitionare tunuri langa bumper

                    if (!CheckPos(i, j - 1, "cannon") || !CheckPos(i, j + 1, "cannon")) {
                        return false;
                    }

                    // verificare pozitionare bumper langa bumper (trebuie sa vad exact ce vrea enuntul)

                    if (!CheckPos(i, j - 1, "bumper") || !CheckPos(i, j + 1, "bumper")) {
                        return false;
                    }

                    // ca sa nu se suprapuna bumperele
                    if (!CheckPos(i, j - 2, "bumper") || !CheckPos(i, j + 2, "bumper")) {
                        return false;
                    }


                }




            }
        }
    }

    // respectarea tuturor conditiilor
    return CheckOverlapping() && CheckConnectedDrawing() && (drawnBlocks <= 10) && (drawnBlocks > 0) && cannonCount > 0;
}

bool Grid::CheckOverlapping()
{
    // design-ul meu permita sa punem blocuri unul peste altul deci se verifica daca aceasta exista
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (gridMatrix[i][j].size() > 2) {
                return false;
            }
        }
    }

    return true;
}

// verificarea conectivitatii
bool Grid::CheckConnectedDrawing()
{
    if (drawnBlocks < 2) {
        return true;
    }

    std::vector<std::pair<int, int>> component;

    int startI = -1, startJ = -1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (gridMatrix[i][j].size() > 1) {
                if (startI == -1) {
                    startI = i;
                    startJ = j;
                }
            }
        }
    }

    component.push_back({startI, startJ});

    bool newBlock;
    do {
        newBlock = false;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (gridMatrix[i][j].size() < 2) {
                    continue;
                }

                bool alreadyInComponent = false;
                for (auto& v : component) {
                    if (v.first == i && v.second == j) {
                        alreadyInComponent = true;
                        break;
                    }
                }

                if (alreadyInComponent) {
                    continue;
                }

                for (auto& v : component) {
                    int di = abs(v.first - i);
                    int dj = abs(v.second - j);
                    if ((di == 1 && dj == 0) || (di == 0 && dj == 1)) {
                        component.push_back({i, j});
                        newBlock = true;
                        break;
                    }
                }
            }
        }
    } while (newBlock);

    return component.size() == drawnBlocks;

}

bool Grid::CheckCol(int i, int j, bool upOrDown, bool checkPos, std::string mesh)
{
    // verificarea unei coloane (aceasta poate in functie de parametrii sa verifice in sus sau in jos de la o pozitie data si poate verifica si conditii diferite)
    if (i > N - 1 || j > M - 1 || j < 0 || i < 0) {
        return true;
    }

    int start, end;

    start =  upOrDown ? i + 1 : 0;
    end   =  upOrDown ? N     : i;
 
    for (int index = start; index < end; index++) {
        if (checkPos) {

            if (!CheckPos(index, j, mesh)) {
                return false;
            }

        }
        else {
            if (gridMatrix[index][j].size() > 1) {
                return false;
            }
        }
    }
    return true;
}

bool Grid::CheckPos(int i, int j, string meshName)
{
    // folosit sa aflam daca la pozitia data nu se afla un anumit bloc

    // in afara grid-ului deci adevarat => nu se afla niciun bloc
    if (i > N - 1 || j > M - 1 || j < 0 || i < 0) {
        return true;
    }

    for (const auto& obj : gridMatrix[i][j]) {
        if (obj.meshName == meshName) {
            return false;
        }
    }

    return true;
}

std::vector<InteractObj> Grid::GetSortedDepthObjects(bool reverse)
{
    // le pun pe toate in lista ca sa le pot sorta pe baza adancimii
    std::vector<InteractObj> renderList;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            for (auto obj : gridMatrix[i][j]) {
                obj.gridI = i;
                obj.gridJ = j;
                renderList.push_back(obj);
            }
        }
    }

    // functionalitate folositoare pentru randare (ordinea de randare de la depth mic la depth mare)
    // sau pentru verificarea celor mai "in fata" obiecte randate la eliminarea cu click dreapta (de la depth mare la depth mic)
    std::sort(renderList.begin(), renderList.end(),
        [reverse](const InteractObj& a, const InteractObj& b) {
            return (reverse ? (a.depth > b.depth) : (a.depth < b.depth));
        });

    return renderList;
}


void Grid::SetDrawing()
{
    // in drawing punem toate elementele sortate de la depth mare la mic si eliminam blocurile din grid 
    drawing = GetSortedDepthObjects(true);
    while (drawing.back().meshName == "square1") {
        drawing.pop_back();
    }

}


void Grid::UpdateHoveredBlock(glm::vec2 pos)
{
    // actualizarea blocului care este hovered pentru obtinerea efectului de hover
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (IsInsideRectangle(pos, gridMatrix[i][j][0].position, gridMatrix[i][j][0].size)) {
                hoveredBlockCoords = gridMatrix[i][j][0].position;
                return;
            }
        }
    }

    hoveredBlockCoords = glm::vec2(-1, -1);
}

// eliminarea ultimului bloc desenat dintr-o anumita zona ce tine cont de cate blocuri si in cel fel ocupa fiecare element

void Grid::RemoveLastDrew(glm::vec2 mousePos)
{
    std::vector<InteractObj> renderList = GetSortedDepthObjects(true);

    for (auto& obj : renderList) {

        if (obj.meshName == baseMeshName) {
            continue;
        }


        if (obj.meshName == "engine") {
            glm::vec2 pos = obj.position - glm::vec2(0, obj.size.y);
            glm::vec2 size = glm::vec2(obj.size.x, 2 * obj.size.y);

            if (IsInsideRectangle(mousePos, pos, size)) {
                if (gridMatrix[obj.gridI][obj.gridJ].size() > 1) {
                    RemoveObject(obj.gridI, obj.gridJ);
                    return;
                }
            }

        }

        if (obj.meshName == "bumper") {
            glm::vec2 topRectanglePos;
            topRectanglePos.x = obj.position.x - obj.size.x;
            topRectanglePos.y = obj.position.y + (float) obj.size.y / 2;
            glm::vec2 topSize = glm::vec2(obj.size.x * 3, obj.size.x);

            if (IsInsideRectangle(mousePos, obj.position, obj.size) || IsInsideRectangle(mousePos, topRectanglePos, topSize)) {
                if (gridMatrix[obj.gridI][obj.gridJ].size() > 1) {
                    RemoveObject(obj.gridI, obj.gridJ);
                    return;
                }
            }

        }

        if (IsInsideRectangle(mousePos, obj.position, obj.size)) {

            if (gridMatrix[obj.gridI][obj.gridJ].size() > 1) {
                RemoveObject(obj.gridI, obj.gridJ);
                return;
            }


        }
    }
}

