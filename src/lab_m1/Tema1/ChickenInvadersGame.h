#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <iostream>
#include "lab_m1/Tema1/meshObj.h"
#include "Chicken.h"
#include "ChickenWave.h"

using namespace mesh;
using namespace std;

class ChickenInvadersGame
{
public:


    struct Explosion {
        glm::vec2 position;
        float timer;
        float duration;
    };

    ChickenInvadersGame(vector<InteractObj> playerShip, float playerPosX, glm::vec2 livesCoords, InteractObj gameBorder);
   
    void RenderPlayer(glm::mat3& visMatrix,
        std::unordered_map<std::string, Mesh*>& meshes,
        std::unordered_map<std::string, Shader*>& shaders,
        std::function<void(Mesh*, Shader*, const glm::mat3&)> RenderFunc);

    void RenderRemainingLives(glm::mat3& visMatrix,
        std::unordered_map<std::string, Mesh*>& meshes,
        std::unordered_map<std::string, Shader*>& shaders,
        std::function<void(Mesh*, Shader*, const glm::mat3&)> RenderFunc);

    void RenderProjectiles(glm::mat3& visMatrix, std::unordered_map<std::string, Mesh*>& meshes, std::unordered_map<std::string, Shader*>& shaders, std::function<void(Mesh*, Shader*, const glm::mat3&)> RenderFunc, float deltaTime);

    void MovePlayerRight(float dist);

    void MovePlayerLeft(float dist);

    void MovePlayerUp(float dist);

    void MovePlayerDown(float dist);

    void FireCannons();

    void UpdateProjectiles(float deltaTime);

    void CheckProjectilesPos(float windowTop);

    bool CheckCollisions();

    bool CheckEggToPlayerCollision();

    void CheckProjectileToEnemyCollision();

    void CheckEnemyToBottomCollision();

    void MoveEnemies(float deltaTime);

    void UpdateEggsPos(float deltaTime);

    void ScaleShip(float scaleFactor, glm::vec2 shipCenter);

    void CheckProjectileToEggCollision();

    ~ChickenInvadersGame();

    void RenderEnemies(glm::mat3& visMatrix, std::unordered_map<std::string, Mesh*>& meshes, std::unordered_map<std::string, Shader*>& shaders, std::function<void(Mesh*, Shader*, const glm::mat3&)> RenderFunc,
        float deltaTime);

    int getScore() { return score; };

    bool GameEnded() { return endGame; };

    private:

protected:

     vector<InteractObj> player;
     
     // folosite ca sa blocheze jucatorul in fereastra
     InteractObj* bottomBlock;
     InteractObj* leftBlock;
     InteractObj* rightBlock;

     vector<Chicken*> enemies;
     vector<InteractObj> projectiles;
     vector<InteractObj> eggs;
     vector<Explosion>  explosions;

     glm::vec2 playerPos;
     int remainingLives;

     glm::vec2 remainingLivesCoords;
     float speedFactor;

     const int projectileSpeed = 1000;
     const float cannonCooldown = 0.2;

     float cannonTimer;
     float eggSpeed;

     vector<ChickenWave*> waves;

     float radians;
     float angle;
     float direction;

     bool waveForming;
     bool endGame;

     InteractObj gameBorder;

     int score;




};
