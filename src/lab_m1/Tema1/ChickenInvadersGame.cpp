#include "lab_m1/Tema1/ChickenInvadersGame.h"
#include "lab_m1/Tema1/LateralWave.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/Tema1/Grid.h"
#include "Tema1.h"

using namespace std;
using namespace mesh;


ChickenInvadersGame::ChickenInvadersGame(vector<InteractObj> playerShip, float playerPosX, glm::vec2 livesCoords, InteractObj gameBorder)
					: player(playerShip), remainingLives(3), remainingLivesCoords(livesCoords), gameBorder(gameBorder), score(0), cannonTimer(0)
{
    // compute the center of the ship for centering it in the window and for scaling
    float minX = FLT_MAX, maxX = -FLT_MAX;
    float minY = FLT_MAX, maxY = -FLT_MAX;

    speedFactor = 1;
    radians = 0;
    waveForming = false;
    endGame = false;
    angle = 0;
    direction = 1;

    for (auto& playerBlock : player) {
        auto centerPos = playerBlock.position + playerBlock.center;
        if (centerPos.x < minX)
            minX = centerPos.x;
        if (centerPos.x > maxX)
            maxX = centerPos.x;
        if (centerPos.y < minY)
            minY = centerPos.y;
        if (centerPos.y > maxY)
            maxY = centerPos.y;

        // setting the speedFactor based on the number of engines the ship has
        if (playerBlock.meshName == "engine") {
            speedFactor *= 1.2f;
        }
    }

    auto center = glm::vec2((minX + maxX) / 2.f, (minY + maxY) / 2.f);
    ScaleShip(0.6, center);

    // compute the bounding box after scaling
    minX = FLT_MAX; maxX = -FLT_MAX;
    minY = FLT_MAX; maxY = -FLT_MAX;
    float minYSize = 0;

    for (auto& playerBlock : player) {
        auto centerPos = playerBlock.position + playerBlock.center;

        if (centerPos.x < minX) {
            minX = centerPos.x;
            leftBlock = &playerBlock;
        }

        if (centerPos.x > maxX) {
            maxX = centerPos.x;
            rightBlock = &playerBlock;
        }

        if (centerPos.y < minY) {
            minY = centerPos.y;
            minYSize = playerBlock.size.y;
            bottomBlock = &playerBlock;
        }

        if (centerPos.y > maxY) {
            maxY = centerPos.y;
        }
    }

    playerPos = glm::vec2(playerPosX, (maxY - minY) / 2.f + minYSize);

    glm::vec2 newCenter = glm::vec2((minX + maxX) / 2.f, (minY + maxY) / 2.f);
    for (auto& playerBlock : player) {
        playerBlock.position += (playerPos - newCenter);
    }

	// reset the values that increase with each wave to the base values
    ChickenWave::ResetBaseValues();

    // hardcoded chicken waves
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
    waves.push_back(new LateralWave(5.0f));
    waves.push_back(new ChickenWave(5.0f, true));
   
    enemies = waves[0]->getChickens();
}

ChickenInvadersGame::~ChickenInvadersGame()
{

}

void ChickenInvadersGame::RenderEnemies(glm::mat3& visMatrix,
    std::unordered_map<std::string, Mesh*>& meshes,
    std::unordered_map<std::string, Shader*>& shaders,
    std::function<void(Mesh*, Shader*, const glm::mat3&)> RenderFunc,
    float deltaTime)
{
    auto modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;

    cannonTimer += deltaTime;

	// rendering the cannon cooldown bar
    float scale = glm::clamp(cannonTimer / cannonCooldown, 0.0f, 1.0f);
    modelMatrix *= transform2D::Translate(1060, 75);
    modelMatrix *= transform2D::Scale(scale, 1);
    RenderFunc(meshes["coolDown"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix;

    // update the positions of the enemies and eggs
    MoveEnemies(deltaTime);
    UpdateEggsPos(deltaTime);

    float chickSpeed = waves[0]->getChickenSpeed();

    for (auto& obj : enemies) {
        
        // Chickens should not be rendered if they are outside the game
        // (important because they use uniform view transformation, and in certain
        // window aspect ratios they could be visible)
        if (obj->getPos().y > (gameBorder.position.y + gameBorder.size.y) + 32) {
            continue;
        }

        if (obj->getPos().x > (gameBorder.position.x + gameBorder.size.x) + 32) {
            continue;
        }

        if (obj->getPos().x < gameBorder.position.x - 32) {
            continue;
        }
           
        // can lay eggs only after forming the initial formation
        if (waves[0]->getFormationEnd()) {
            obj->LayEgg(deltaTime, eggs); // if possible (internal timer)
        }
        

		// rendering the chicken body, head, wings and legs
        glm::mat3 m = modelMatrix;
        m *= transform2D::Translate(obj->getX(), obj->getY());
        RenderFunc(meshes[obj->getMeshBody()], shaders["VertexColor"], m);
        m *= transform2D::Translate(0, 25);
        RenderFunc(meshes[obj->getMeshHead()], shaders["VertexColor"], m);
        m = modelMatrix;
        
        m = modelMatrix;
        m *= transform2D::Translate(obj->getX() + obj->getRadius() / 2, obj->getY());
        m *= transform2D::Rotate(RADIANS(angle));
        RenderFunc(meshes[obj->getMeshWing()], shaders["VertexColor"], m);

        m = modelMatrix;
        m *= transform2D::Translate(obj->getX() - obj->getRadius() / 2, obj->getY());
        m *= transform2D::Scale(-1, 1);   
        m *= transform2D::Rotate(RADIANS(angle));
        RenderFunc(meshes[obj->getMeshWing()], shaders["VertexColor"], m);

        m = modelMatrix;
        m *= transform2D::Translate(obj->getX() + 2,obj->getY() - 18);
        RenderFunc(meshes[obj->getMeshLeg()], shaders["VertexColor"], m);

        m = modelMatrix;
        m *= transform2D::Translate(obj->getX() - 2,obj->getY() - 18);
        m *= glm::mat3(-1, 0, 0, 0, 1, 0, 0, 0, 1);
        RenderFunc(meshes[obj->getMeshLeg()], shaders["VertexColor"], m);

    }

    // rendering the eggs
    for (const auto& egg : eggs) {
        glm::mat3 m = modelMatrix;
        m *= transform2D::Translate(egg.position.x, egg.position.y);
        RenderFunc(meshes[egg.meshName], shaders["VertexColor"], m);
    }

    // update the angle for the wing flapping animation
    angle += direction * 2 * chickSpeed * deltaTime;

    // limit the angle to avoid unnatural flapping 
    if (angle > 15) {
        angle = 15;
        direction = -1;
    }
    if (angle < -30) {
        angle = -30;
        direction = 1;
    }


    // rendering explosions generated by collisions and removing them if their given time has elapsed
    int i = 0;
    for (auto& explosion : explosions) {
        explosion.timer += deltaTime;
        glm::mat3 mat = modelMatrix;
        mat *= transform2D::Translate(explosion.position.x, explosion.position.y);
        RenderFunc(meshes["explosion"], shaders["VertexColor"], mat);
        if (explosion.timer > explosion.duration) {
            explosions.erase(explosions.begin() + i);
            i--;
        }
        i++;
    }


}

void ChickenInvadersGame::RenderPlayer(glm::mat3& visMatrix,
    std::unordered_map<std::string, Mesh*>& meshes,
    std::unordered_map<std::string, Shader*>& shaders,
    std::function<void(Mesh*, Shader*, const glm::mat3&)> RenderFunc)
{
    auto modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;

    // rendering the blocks that form the player ship
    for (const auto& obj : player) {
        glm::mat3 m = modelMatrix;
        m *= transform2D::Translate(obj.position.x, obj.position.y);
        m *= transform2D::Scale(0.6, 0.6);
        RenderFunc(meshes[obj.meshName], shaders["VertexColor"], m);
    }
}


void ChickenInvadersGame::RenderRemainingLives(glm::mat3& visMatrix,
    std::unordered_map<std::string, Mesh*>& meshes,
    std::unordered_map<std::string, Shader*>& shaders,
    std::function<void(Mesh*, Shader*, const glm::mat3&)> RenderFunc)
{
    
    auto modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;

	// rendering the hearts that represent the remaining lives and the rectangle that frames them
    modelMatrix *= transform2D::Translate(remainingLivesCoords.x, remainingLivesCoords.y);
    RenderFunc(meshes["livesRectangle"], shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Translate(220 / 6 + 5, 38);

    for (int i = 0; i < remainingLives; i++) {
        RenderFunc(meshes["heart"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(70, 0);
    }

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    RenderFunc(meshes["borderRectangle"], shaders["VertexColor"], modelMatrix);
    
}

void ChickenInvadersGame::RenderProjectiles(glm::mat3& visMatrix,
    std::unordered_map<std::string, Mesh*>& meshes,
    std::unordered_map<std::string, Shader*>& shaders,
    std::function<void(Mesh*, Shader*, const glm::mat3&)> RenderFunc,
    float deltaTime)
{
    auto modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;
    
    // update the positions before the actual rendering
    UpdateProjectiles(deltaTime);

    for (const auto& projectile : projectiles) {
        glm::mat3 m = modelMatrix;
        m *= transform2D::Translate(projectile.position.x, projectile.position.y);
        RenderFunc(meshes[projectile.meshName], shaders["VertexColor"], m);
    }
}

void ChickenInvadersGame::MovePlayerRight(float dist)
{
    // the player can move after the chickens have formed the initial formation
    if (!waves[0]->getFormationEnd()) {
        return;
    }

    float moveDist = dist * speedFactor;

	// conditions defining how the player is blocked in the movement to the right in case of a bumper
    float bumper = (rightBlock->meshName.rfind("bumper", 0) == 0) ? 2 : 1;

    if (rightBlock->position.x + bumper * rightBlock->size.x + moveDist >= gameBorder.position.x + gameBorder.size.x) {
        moveDist = (gameBorder.position.x + gameBorder.size.x) - (rightBlock->position.x + bumper * rightBlock->size.x);
    }

    for (auto& playerBlock : player) {
        playerBlock.position.x += moveDist;
    }
}

void ChickenInvadersGame::MovePlayerLeft(float dist)
{
    // the player can move after the chickens have formed the initial formation
    if (!waves[0]->getFormationEnd()) {
        return;
    }

    float moveDist = dist * speedFactor;


    // conditions defining how the player is blocked in the movement to the left in case of a bumper
    float bumper = (leftBlock->meshName.rfind("bumper", 0) == 0) ? 1 : 0;

    if (leftBlock->position.x - moveDist - bumper * leftBlock->size.x <= gameBorder.position.x) {
        moveDist = leftBlock->position.x - gameBorder.position.x - bumper * leftBlock->size.x;
    }

    for (auto& playerBlock : player) {
        playerBlock.position.x -= moveDist;
    }
}

void ChickenInvadersGame::MovePlayerUp(float dist)
{
    // the player can move after the chickens have formed the initial formation
    if (waves[0]->getFormationEnd()) {
        for (auto& playerBlock : player) {
            playerBlock.position.y += dist * speedFactor;
        }
    }
}

void ChickenInvadersGame::MovePlayerDown(float dist)
{
    // the player can move after the chickens have formed the initial formation
    if (!waves[0]->getFormationEnd()) {
        return;
    }

    float moveDist = dist * speedFactor;

	// conditions defining how the player is blocked in the movement downwards
    if (bottomBlock->position.y - moveDist <= gameBorder.position.y) {
        moveDist = bottomBlock->position.y - gameBorder.position.y;
    }

    for (auto& playerBlock : player) {
        playerBlock.position.y -= moveDist;
    }

}

void ChickenInvadersGame::FireCannons()
{
	// the cannon can fire only when it is not in cooldown and the chickens have finished forming

    if (cannonTimer > cannonCooldown && waves[0]->getFormationEnd()) {
        for (auto& playerBlock : player) {
            if (playerBlock.meshName == "cannon") {
                projectiles.push_back({ playerBlock.position + playerBlock.center, glm::vec2(8,8), glm::vec2(0), "projectile", 0 });
            }
        }
       
        cannonTimer = 0;
    }


}

void ChickenInvadersGame::UpdateProjectiles(float deltaTime)
{
    for (auto& projectile : projectiles) {
        projectile.position.y += projectileSpeed * deltaTime;

    }
}

void ChickenInvadersGame::CheckProjectilesPos(float windowTop)
{
	// erase the projectiles that go out of the window
    int i = 0;
    for (auto& projectile : projectiles) {
        if (projectile.position.y > windowTop) {
            projectiles.erase(projectiles.begin() + i);
            i--;
        }
        i++;

    }
}

// checking collisions that returns the result of the egg-player collision
// to generate a camera shake in the main class
bool ChickenInvadersGame::CheckCollisions()
{
    CheckProjectileToEggCollision();
    CheckProjectileToEnemyCollision();
    CheckEnemyToBottomCollision();

    return CheckEggToPlayerCollision();
}

bool ChickenInvadersGame::CheckEggToPlayerCollision()
{
    if (!waves[0]->getFormationEnd()) {
        return false;
    }

    bool ret = false;
    int i = 0;
    for (auto& egg : eggs) {
        for (auto& playerBlock : player) {

            if (playerBlock.meshName.rfind("bumper", 0) == 0) {

                glm::vec2 pos = playerBlock.position + playerBlock.center;
                glm::vec2 size = glm::vec2(playerBlock.size.x * 1.5, playerBlock.size.x);

                if (CheckEllipseEllipseCollision(egg.position, egg.size, pos, size, true)) {

                    std::string name;
                    bool destroyed = false;
                    // .3 seconds explosion
                    explosions.push_back({ egg.position - glm::vec2(0, egg.size.y), 0, 0.3});

					// successive degradation states of the bumper
                    if (playerBlock.meshName == "bumper") {
                        name = "bumper1";
                    }

                    if (playerBlock.meshName == "bumper1") {
                        name = "bumper2";
                    }

                    if (playerBlock.meshName == "bumper2") {
                        name = "bumper3";
                    }

					// bumper directly destroyed by a golden egg
                    if (playerBlock.meshName == "bumper3" || egg.meshName == "eggGold") {
                        destroyed = true;
                    }

                    if (!destroyed) {
                        playerBlock.meshName = name;
                    }

					// when destroyed, the bumper transforms into a normal block to maintain the connectivity of the ship
                    if (destroyed) {
                        float size = playerBlock.size.x;
                        playerBlock = { playerBlock.position, glm::vec2(size, size), glm::vec2(size / 2, size / 2), "squareBlock", 0 };
                    }

                    eggs.erase(eggs.begin() + i);
                    i--;
                    ret = true;
                    break;

                }


            }

            if (CheckEllipseRectCollision(egg.position, egg.size, playerBlock.position, playerBlock.size)) {
                remainingLives--;
                if (!remainingLives) {
                    endGame = true;
                }
                explosions.push_back({ egg.position - glm::vec2(0, egg.size.y), 0, 0.3 });
                eggs.erase(eggs.begin() + i);
                i--;
                ret = true;
                break;
            }



        }
        i++;
    }

    return ret;
}

void ChickenInvadersGame::CheckProjectileToEnemyCollision()
{
	// the projectiles that the player managed to fire should not destroy the chickens in formation
    if (!waves[0]->getFormationEnd()) {
        return;
    }


    for (int p = 0; p < projectiles.size();) {
        bool projectileRemoved = false;

        for (int e = 0; e < enemies.size() && !projectileRemoved;) {

            if (CheckEllipseEllipseCollision(projectiles[p].position,
                projectiles[p].size,
                enemies[e]->getPos(),
                glm::vec2(enemies[e]->getRadius(), enemies[e]->getRadius())))
            {
                waves[0]->RemoveChicken(enemies[e]);

                enemies.erase(enemies.begin() + e);
                projectiles.erase(projectiles.begin() + p);

                score++;
                projectileRemoved = true;
            }
            else e++;
        }

        if (!projectileRemoved) {
            p++;
        }
    }
}

void ChickenInvadersGame::CheckEnemyToBottomCollision()
{
    for (auto& enemy : enemies) {
        if (enemy->getPos().y + enemy->getRadius() < gameBorder.position.y) {
            endGame = true;
        }
    }
}

void ChickenInvadersGame::MoveEnemies(float deltaTime)
{

    waves[0]->WaveForming(deltaTime);
    waves[0]->FormationMove(deltaTime);

    enemies = waves[0]->getChickens();

    // next wave of chickens logic
    if (enemies.empty()) {
        if (waves.size() <= 1) {
            endGame = true;
            return;
        }

        waves.erase(waves.begin());
        enemies = waves[0]->getChickens();

        if (!enemies.empty()) {
            eggSpeed = enemies[0]->getEggSpeed();
        }
        else {
            eggSpeed = 0;
        }

        return;
    }

    eggSpeed = enemies[0]->getEggSpeed();
}

void ChickenInvadersGame::UpdateEggsPos(float deltaTime)
{
    //  updating position
    for (auto& egg : eggs) {
        if (egg.meshName == "egg")
            egg.position.y -= eggSpeed * deltaTime;

        if (egg.meshName == "eggGold")
            egg.position.y -= 1.4f * eggSpeed * deltaTime;
    }

	// erasing the eggs that go out of the game window
    for (int i = 0; i < eggs.size();) {
        if (eggs[i].position.y + eggs[i].size.y < gameBorder.position.y) {
            eggs.erase(eggs.begin() + i);
        } else {
            i++;
        }
    }
}

void ChickenInvadersGame::ScaleShip(float scaleFactor, glm::vec2 shipCenter)
{
    for (auto& block : player) {
        glm::vec2 oldCenterWorld = block.position + block.center;
        glm::vec2 offset = oldCenterWorld - shipCenter;
        glm::vec2 newCenterWorld = shipCenter + offset * scaleFactor;

        block.center *= scaleFactor;
        block.size *= scaleFactor;

        block.position = newCenterWorld - block.center;
    }
}

void ChickenInvadersGame::CheckProjectileToEggCollision()
{
    for (int p = 0; p < projectiles.size();) {
        bool projectileRemoved = false;

        for (int e = 0; e < eggs.size() && !projectileRemoved;) {

            if (CheckEllipseEllipseCollision(
                projectiles[p].position,
                projectiles[p].size,
                eggs[e].position,
                eggs[e].size))
            {
                if (eggs[e].meshName == "eggGold") {
                    if (remainingLives < 3)
                        remainingLives++;

                }
                explosions.push_back({ eggs[e].position, 0, 0.25f });

                eggs.erase(eggs.begin() + e);

                projectiles.erase(projectiles.begin() + p);

                projectileRemoved = true;
            }
            else
            {
                e++;
            }
        }

        if (!projectileRemoved) {
            p++;
        }
    }
}
