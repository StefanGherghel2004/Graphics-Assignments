#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"
using namespace std;


namespace mesh
{
    struct InteractObj {
        glm::vec2 position; // bottom left corner
        glm::vec2 size; // folosit ca un hit box de la bottom left corner
        glm::vec2 center; // folosit pentru centrare
        std::string meshName;
        int depth;

        // pozitii in grid
        int gridI = -1;
        int gridJ = -1;

    };

    // definite fata de left bottom corner
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateSquareBlock(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, glm::vec3 centrCol);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float lenx, float leny, glm::vec3 color, bool fill = false);
    Mesh* CreateStartButton(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateEngine(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, glm::vec3 centrCol, glm::vec3 fireCol);
    Mesh* CreateCannon(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 baseCol, glm::vec3 circleCol, glm::vec3 barrelCol);
    Mesh* CreateBumper(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 baseCol, glm::vec3 circleCol);
    Mesh* CreateStarBackground(const std::string& name, glm::vec3 corner, float radius, glm::vec3 color, glm::vec3 backColor, float width, float Heigth);

    // definite fata de centru
    Mesh* CreateHeart(const std::string& name, glm::vec3 center, float length, glm::vec3 color);
    Mesh* CreateEllipsoide(const std::string& name, glm::vec3 center, float radiusX, float radiusY, glm::vec3 color, float gradient, bool half = false, float zCenterOff = -0.5f);
    Mesh* CreateCylinder(const std::string& name, glm::vec3 center, float radius, float heigth, glm::vec3 color, float gradient, bool pointy); // tema 2
    Mesh* CreateTrack(const std::string& name, glm::vec3 center, float lenX, float lenY, float lenZ, glm::vec3 color1, glm::vec3 color2, glm::vec3 color3, float gradient, string type); // tema 2
    Mesh* CreateTrainStation(const std::string& name, glm::vec3 center, float lenX, float lenY, float lenZ, glm::vec3 color1, glm::vec3 color2, glm::vec3 color3, float gradient, int type); // tema 2
    Mesh* CreatePyramid(const std::string& name, glm::vec3 center, float baseLenX, float baseLenZ, float height, glm::vec3 baseColor, glm::vec3 sideColor, float gradient); // tema 2
    Mesh* CreateSymbol(const std::string& name, glm::vec3 center, float size, glm::vec3 color, float gradient, int type); // tema 2
    Mesh* CreateBox(const std::string& name, glm::vec3 center, float lenX, float lenY, float lenZ, glm::vec3 color, float gradient = 1); // tema 2
    Mesh* CreateChickenHead(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, glm::vec3 eyesColor, glm::vec3 combColor, glm::vec3 beakCol);
    Mesh* CreateExplosion(const std::string& name, glm::vec3 center, float radius, glm::vec3 color1, glm::vec3 color2);
    Mesh* CreateStar(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, float offset = glm::pi<float>() / 2);

    // definire cu puncte
    Mesh* CreateTriangle(const std::string& name, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color, float gradient = 1.0f);
    Mesh* CreateChickenLeg(const std::string& name, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color);
    Mesh* CreateLine(const std::string& name, glm::vec3 point1, glm::vec3 point2, glm::vec3 color);
    Mesh* Create4PPoly(const std::string& name, VertexFormat p1, VertexFormat p2, VertexFormat p3, VertexFormat p4, float gradient = 1.0f); // tema 2 

    // functii ajutatoare
    bool CheckEllipseEllipseCollision(glm::vec2 ellipse1Pos, glm::vec2 ellipse1Size, glm::vec2 ellipse2Pos, glm::vec2 ellipse2Size, bool half = false);
    bool CheckEllipseRectCollision(glm::vec2 ellipsePos, glm::vec2 ellipseSize, glm::vec2 rectanglePos, glm::vec2 rectangleSize);
    bool IsInsideRectangle(glm::vec2 pos, glm::vec2 recpos, glm::vec2 size);

    // functia pentru dat merge (merge doar daca toate formele sunt filled)
    Mesh* MergeMeshes(std::vector<Mesh*>& meshes, std::string meshName);

}
