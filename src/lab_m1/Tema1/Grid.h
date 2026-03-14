#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <iostream>
#include "lab_m1/Tema1/meshObj.h"

using namespace mesh;
using namespace std;

namespace grid
{

    class Grid
    {
        public:
            Grid();
            Grid(int rows, int cols, float size, glm::vec2 corner, std::string meshName);

            void InsertObject(int i, int j, InteractObj object);

            void RemoveObject(int i, int j);

            bool CheckRestrictions();

            bool CheckOverlapping();

            bool CheckConnectedDrawing();

            bool CheckCol(int i, int j, bool upOrDown, bool checkPos = false, std::string mesh = "null");


            bool CheckPos(int i, int j, string meshName);
            
            std::vector<InteractObj> GetSortedDepthObjects(bool reverse = false);

            void SetDrawing();

            void UpdateHoveredBlock(glm::vec2 pos);

            void RemoveLastDrew(glm::vec2 mousePos);

            glm::vec2 getHoveredBlock() { return hoveredBlockCoords; };

            ~Grid();
            std::vector<std::vector<std::vector<mesh::InteractObj>>>& getMatrix() { return gridMatrix; };
            std::vector<InteractObj>& getDrawing() { return drawing; };
            int getDrawnBlocks() { return drawnBlocks; };

            void ClearDrawing();

        private:


        protected:
            std::vector<std::vector<std::vector<InteractObj>>> gridMatrix;
            std::vector<InteractObj> drawing;

            int N;
            int M;

            int drawnBlocks;

            glm::vec2 hoveredBlockCoords;
            std::string baseMeshName;

    };



}  // namespace grid