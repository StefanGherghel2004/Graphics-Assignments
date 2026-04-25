#include "lab_m1/Tema1/Tema1.h"
#include "lab_m1/Tema1/ChickenInvadersGame.h"

#include <vector>
#include <iostream>

#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/Tema1/meshObj.h"
#include "colors.h"
#include "core/engine.h"
#include "components/text_renderer.h"
#include <fstream>

using namespace std;
using namespace m1;
using namespace mesh;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;          // logic x
    logicSpace.y = 0;          // logic y
    logicSpace.width = 1280;   // logic width
    logicSpace.height = 720;   // logic height
    components = 10;
    canStart = false;
    renderGame = false;
    returnMenu = false;

    glm::vec3 corner = glm::vec3(0, 0, 0);
    glm::vec3 start = glm::vec3(1180, 630, 0);
    glm::vec3 remainingElementsPos = glm::vec3(360, 640, 0);

    objectDepth = 0;
    hoveredBlockIndex = -1;
    resizeFactor = 1;
    resizeFactorX = 1;
    resizeFactorY = 1;

	// used for rendering the block being dragged by the mouse
    mouseSel = false;
    mousePosX = 0;
    mousePosY = 0;

    blocksGrid = new Grid(row, col, unitLen, glm::vec2(500, 100), "square1");

    // grid square
    Mesh* square1 = mesh::CreateSquare("square1", corner, 0.75 * unitLen, glm::vec3(0.67, 0.8, 1), true);
    AddMeshToList(square1);

    // hovered grid square
    Mesh* squareGridHover = mesh::CreateSquare("squareGridHover", corner, 0.75 * unitLen, glm::vec3(0.3, 0.4, 0.7), true);
    AddMeshToList(squareGridHover);

	// squares for the remaining elements to be placed in the ship
    Mesh* square2 = mesh::CreateSquare("square2", remainingElementsPos, 1.35 * unitLen, GREEN, true);
    AddMeshToList(square2);

    // simple block in the ship
    Mesh* squareBlock = mesh::CreateSquareBlock("squareBlock", corner + glm::vec3(0, 0, 40), unitLen, GREY_08, GREY_05);
    AddMeshToList(squareBlock);

    Mesh* engine = mesh::CreateEngine("engine", corner + glm::vec3(0, 0, 40), unitLen, ORANGE, glm::vec3(0.8, 0.2, 0), RED);
    AddMeshToList(engine);

    Mesh* cannon = mesh::CreateCannon("cannon", corner + glm::vec3(0, 0, 40), unitLen, GREY_05, GREY_07, GREY_02);
    AddMeshToList(cannon);

    Mesh* bumper = mesh::CreateBumper("bumper", corner + glm::vec3(0, 0, 40), unitLen, GREY_08, glm::vec3(0.8, 0.8, 0));
    AddMeshToList(bumper);

    // different stages of the bumper 
    Mesh* bumper1 = mesh::CreateBumper("bumper1", corner + glm::vec3(0, 0, 40), unitLen, GREY_06, glm::vec3(0.6, 0.6, 0));
    AddMeshToList(bumper1);

    Mesh* bumper2 = mesh::CreateBumper("bumper2", corner + glm::vec3(0, 0, 40), unitLen, GREY_04, glm::vec3(0.4, 0.4, 0));
    AddMeshToList(bumper2);

    Mesh* bumper3 = mesh::CreateBumper("bumper3", corner + glm::vec3(0, 0, 40), unitLen, GREY_02, glm::vec3(0.2, 0.2, 0));
    AddMeshToList(bumper3);

    // red outline around the blocks to select
    Mesh* componentSelect = mesh::CreateRectangle("componentSelect", corner, logicSpace.width / 4, logicSpace.height / 4, RED);
    AddMeshToList(componentSelect);

    Mesh* componentSelectHover = mesh::CreateRectangle("componentSelectHover", corner - glm::vec3(0, 0, 5), logicSpace.width / 4, logicSpace.height / 4, glm::vec3(0.3, 0, 0), true);
    AddMeshToList(componentSelectHover);

    // red start button
    Mesh* startButtonRed = mesh::CreateStartButton("startButtonRed", start, 2 * unitLen, RED, true);
    AddMeshToList(startButtonRed);

    // green start button
    Mesh* startButtonGreen = mesh::CreateStartButton("startButtonGreen", start, 2 * unitLen, GREEN, true);
    AddMeshToList(startButtonGreen);

    float startSize = 2 * unitLen;
    startButton = {glm::vec2(1180, 630), glm::vec2(2 * unitLen,2 * unitLen), glm::vec2(unitLen, unitLen), "startButtonGreen" };

    Mesh* gameLivesRectangle = mesh::CreateRectangle("livesRectangle", glm::vec3(0, 0, 45), 220, 70, RED);
    AddMeshToList(gameLivesRectangle);

    Mesh* gameBorderRectangle = mesh::CreateRectangle("borderRectangle", glm::vec3(0.05, 0.05, 45), 1279.95, 719.95, RED);
    AddMeshToList(gameBorderRectangle);

    Mesh* gameMenuRectangle = mesh::CreateRectangle("gameMenuRectangle", glm::vec3(0.05, 0.05, 45), 1279.95, 719.95, WHITE);
    AddMeshToList(gameMenuRectangle);

    Mesh* gameMenuButton = mesh::CreateRectangle("gameMenuButton", glm::vec3(240, 200, 45), 320, 180, WHITE);
    AddMeshToList(gameMenuButton);

    Mesh* maskRectangle1 = mesh::CreateRectangle("mask1", glm::vec3(0, -100, 44), 1280, 100, BLACK, true);
    AddMeshToList(maskRectangle1);


    Mesh* maskRectangle2 = mesh::CreateRectangle("mask2", glm::vec3(-100, 0, 44), 100, 720, BLACK, true);
    AddMeshToList(maskRectangle2);

    // the button positions in the return menu
    returnWithClear = { glm::vec2(240, 200), glm::vec2(320, 180), glm::vec2(0,0), "gameMenuButton", 0};
    returnButton = { glm::vec2(720, 200), glm::vec2(320, 180), glm::vec2(0,0), "gameMenuButton", 0 };

    Mesh* projectile = mesh::CreateEllipsoide("projectile", corner, 8, 8, GREY_06, 0.5);
    AddMeshToList(projectile);

    Mesh* heart = mesh::CreateHeart("heart", glm::vec3(0, 0, 45), 2, RED);
    AddMeshToList(heart);

	// the egg (the z coordinate is set to -1 so that it is under the chicken in case of overlapping rows of chickens)
    Mesh* egg = mesh::CreateEllipsoide("egg", glm::vec3(0, 0, -1), 10, 16, glm::vec3(1, 0.937, 0.682), 0.625);
    AddMeshToList(egg);

    Mesh* eggGold = mesh::CreateEllipsoide("eggGold", glm::vec3(0, 0, -1), 10, 16, glm::vec3(0.9, 0.6, 0.25), 0.625);
    AddMeshToList(eggGold);

    glm::vec3 newCorner = glm::vec3(-0.5 * unitLen, - 0.5 * unitLen, 0);

	// setting the z coordinate so that the head is above the body
    Mesh* chickenBody = mesh::CreateSquare("chickenBody", newCorner, unitLen, glm::vec3(0, 0.588, 1), true);
    AddMeshToList(chickenBody);

    Mesh* chickenBodyRed = mesh::CreateSquare("chickenBodyRed", newCorner, unitLen, RED, true);
    AddMeshToList(chickenBodyRed);

    Mesh* chickenHead = mesh::CreateChickenHead("chickenHead", glm::vec3(0, 0, 1), 16, GREY_08, BLACK, RED, ORANGE);
    AddMeshToList(chickenHead);

    Mesh* chickenHeadGold = mesh::CreateChickenHead("chickenHeadGold", glm::vec3(0, 0, 1), 16, GREY_08, BLACK, glm::vec3(0.9, 0.6, 0.25), glm::vec3(0.9, 0.6, 0.25));
    AddMeshToList(chickenHeadGold);

    Mesh* chickenWing = mesh::CreateTriangle("chickenWing", glm::vec3(0, 0, 3), glm::vec3(32, 12, 3), glm::vec3(20, 48, 3), GREY_08);
    AddMeshToList(chickenWing);

    Mesh* chickenWingRed = mesh::CreateTriangle("chickenWingGold", glm::vec3(0, 0, 3), glm::vec3(32, 12, 3), glm::vec3(20, 48, 3), glm::vec3(0.9, 0.6, 0.25));
    AddMeshToList(chickenWingRed);

    Mesh* chickenLeg = mesh::CreateChickenLeg("chickenLeg", glm::vec3(0, 0, 3), glm::vec3(5, -10, 3), glm::vec3(15, -5, 3), ORANGE);
    AddMeshToList(chickenLeg);

    Mesh* chickenGold = mesh::CreateChickenLeg("chickenLegGold", glm::vec3(0, 0, 3), glm::vec3(5, -10, 3), glm::vec3(15, -5, 3), glm::vec3(0.9, 0.6, 0.25));
    AddMeshToList(chickenGold);

    Mesh* debugLine = mesh::CreateLine("debugLine", glm::vec3(980, 0, 0), glm::vec3(980, 720, 0), RED);
    AddMeshToList(debugLine);

    Mesh* hitBoxTest = mesh::CreateEllipsoide("hitbox", corner, 25, 25, GREY_06, 0.5);
    AddMeshToList(hitBoxTest);

    Mesh* explosion = mesh::CreateExplosion("explosion", corner , 10, YELLOW, RED);
    AddMeshToList(explosion);

    Mesh* background = mesh::CreateStarBackground("background", corner - glm::vec3(0, 0, 100), 2, glm::vec3(0.8, 0.8, 0), glm::vec3(0, 0, 0.05f), logicSpace.width, logicSpace.height);
    AddMeshToList(background);

    Mesh* coolDown = mesh::CreateRectangle("coolDown", glm::vec3(0, 0, 43), 218, 10, RED, true);
    AddMeshToList(coolDown);


	// used for rendering text (score, lives, highscore)
    textRenderer = new TextRenderer(window->props.selfDir, window->props.resolution.x, window->props.resolution.y);
    textRenderer->Load("assets/fonts/Hack-Bold.ttf", 24);

    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;

	// the outline for the block selection buttons and the blocks to select
    for (int i = 0; i < 4; i++) {
        glm::vec2 pos(0, i * (logicSpace.height / 4));
        glm::vec2 size(logicSpace.width / 4, logicSpace.height / 4);
   
        blockSelectButtons.push_back({ pos, size, glm::vec2(0), "componentSelect", 0 });
    }


	// extracting the all time high score from the text file
    std::ifstream fin("highscore.txt");
    if (fin.good()) {
        fin >> allTimeHighScore;
    }
    fin.close();

    // music logic
    std::string path = PATH_JOIN(window->props.selfDir, "assets", "music", "music.wav");

    backgroundMusic.openFromFile(path);
    backgroundMusic.setLooping(true);
    backgroundMusic.setVolume(50);
    backgroundMusic.play();


}



void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// the overlapping should be based on the order of rendering in the first part (with the grid)
    // and based on depth in the second part
    if (!renderGame) {
        glDisable(GL_DEPTH_TEST);
    }
    else {
        glEnable(GL_DEPTH_TEST);
    }
}


void Tema1::Update(float deltaTimeSeconds)
{

    glm::ivec2 resolution = window->GetResolution();
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    if (renderGame && shakeTimer < shakePeriod) {

        // generating the camera shake effect

        shakeTimer += deltaTimeSeconds;

        float progress = (shakePeriod - shakeTimer) / shakePeriod;
        float intensity = shakeStrength * progress;

        float offsetX = (rand() / (float)RAND_MAX - 0.5f) * intensity;
        float offsetY = (rand() / (float)RAND_MAX - 0.5f) * intensity;

        visMatrix *= transform2D::Translate(offsetX, offsetY);
    }

    if (!renderGame) {

		// drawing the grid (including current ship and hover effect)
        DrawGrid();

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;

		// rendering the blocks to select and the hover effect if the mouse is above one of the blocks
        for (int i = 0; i < 4; i++) {
            if (i == hoveredBlockIndex) {
                RenderMesh2D(meshes["componentSelectHover"], shaders["VertexColor"], modelMatrix);
            }
            RenderMesh2D(meshes["componentSelect"], shaders["VertexColor"], modelMatrix);
            InteractObj block = selectBlocks.at(i);
            RenderMesh2D(meshes[block.meshName], shaders["VertexColor"], modelMatrix * transform2D::Translate(logicSpace.width / 8 - block.center.x, logicSpace.height / 8 - block.center.y));

            modelMatrix *= transform2D::Translate(0, logicSpace.height / 4);
        }

		// remaining components to be placed in the ship
        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;

        for (int i = 0; i < components - blocksGrid->getDrawnBlocks(); i++) {
            RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
            modelMatrix *= transform2D::Translate(2 * unitLen, 0);
        }

        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix;

		// checking if the ship configuration respects the restrictions and rendering the corresponding start button
        CheckRestrictions();

        if (canStart) {
            RenderMesh2D(meshes["startButtonGreen"], shaders["VertexColor"], modelMatrix);
        }
        else {
            RenderMesh2D(meshes["startButtonRed"], shaders["VertexColor"], modelMatrix);
        }

		// rendering the block dragged by the mouse
        if (mouseSel) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= visMatrix;
            modelMatrix *= transform2D::Translate(mousePosX, mousePosY);
            RenderMesh2D(meshes[mouseMesh], shaders["VertexColor"], modelMatrix);
        }


    } else {

        RenderGame(deltaTimeSeconds);

    }


}


void Tema1::FrameEnd()
{
    if (!renderGame) {
        return;
    }

	// at the end of the frame we check the projectiles positions
    game->CheckProjectilesPos((float) logicSpace.height);

}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{

    if (!renderGame) {
        return;
    }

    float dist = 400 * deltaTime;

    if (window->KeyHold(GLFW_KEY_W) || window->KeyHold(GLFW_KEY_UP)) {
        game->MovePlayerUp(dist);
    }

    if (window->KeyHold(GLFW_KEY_S) || window->KeyHold(GLFW_KEY_DOWN)) {
        game->MovePlayerDown(dist);
    }

    if (window->KeyHold(GLFW_KEY_A) || window->KeyHold(GLFW_KEY_LEFT)) {
        game->MovePlayerLeft(dist);
    }

    if (window->KeyHold(GLFW_KEY_D) || window->KeyHold(GLFW_KEY_RIGHT)) {
        game->MovePlayerRight(dist);
    }

}


void Tema1::OnKeyPress(int key, int mods)
{
	// don't use keys if we're not in game
    if (!renderGame) {
        return;
    }

    if (key == GLFW_KEY_SPACE) {
        game->FireCannons();
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (renderGame)
        return;

    glm::vec3 mouseLogic = glm::inverse(visMatrix) * glm::vec3(mouseX, window->GetResolution().y - mouseY, 1);

    blocksGrid->UpdateHoveredBlock(mouseLogic);

	// updating the correct position of the dragged block
    if (mouseSel) {
        mousePosX = mouseLogic.x - globalOff;
        mousePosY = mouseLogic.y - globalOff;

        return;
    }

    // updating the hovered selection block 
    for (int i = 0; i < blockSelectButtons.size(); i++) {
        const auto& btn = blockSelectButtons[i];
        if (IsInsideRectangle(mouseLogic, btn.position, btn.size)) {
            hoveredBlockIndex = i;
            break;
        }
		//  reached this point => the mouse is outside the selection blocks
        if (i == blockSelectButtons.size() - 1) {
            hoveredBlockIndex = -1;
        }
    }

}

bool Tema1::IsOverStartButton(int mouseX, int mouseY) {

    glm::vec2 pos = startButton.position;
    glm::vec2 size = startButton.size;

    if (!IsInsideRectangle(glm::vec2(mouseX, mouseY), pos, size)) {
        return false;
    }

    float halfX = pos.x + size.x / 2.0f;
    float slope = (size.y / 2.0f) / (size.x / 2.0f);

    float topLineY = pos.y + size.y - (mouseX - halfX) * slope;
    float bottomLineY = pos.y + (mouseX - halfX) * slope;

    return !((mouseX > halfX) && (mouseY >= bottomLineY) && (mouseY <= topLineY));
}

void Tema1::DrawGrid()
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= visMatrix;

	// putting all the blocks in the list so that I can sort them based on depth and render them correctly
    std::vector<InteractObj> renderList = blocksGrid->GetSortedDepthObjects();

    for (const auto& obj : renderList) {
        glm::mat3 m = modelMatrix;
        m *= transform2D::Translate(obj.position.x, obj.position.y);
        
        RenderMesh2D(meshes[obj.meshName], shaders["VertexColor"], m);

        if (obj.position == blocksGrid->getHoveredBlock()) {
            RenderMesh2D(meshes["squareGridHover"], shaders["VertexColor"], m);
        }
    }


}

void Tema1::RenderGame(float deltaTime)
{
    
    if (game->GameEnded()) {

		// rendering the return menu with the corresponding buttons and updating the scores
        RenderMesh2D(meshes["gameMenuRectangle"], shaders["VertexColor"], visMatrix);
        RenderMesh2D(meshes["gameMenuButton"], shaders["VertexColor"], visMatrix);
        RenderMesh2D(meshes["gameMenuButton"], shaders["VertexColor"], visMatrix * transform2D::Translate(480, 0));
        std::string editorReturn = "Edit the ship";
        std::string editorReturnClear = "Build a new ship";

        if (game->getScore() > currentSessionHighScore) {
            currentSessionHighScore = game->getScore();
        }

        if (game->getScore() > allTimeHighScore) {
            allTimeHighScore = game->getScore();

            std::ofstream fout("highscore.txt");
            fout << allTimeHighScore;
            fout.close();
        }


        glm::vec2 off1 = 0.5f * returnWithClear.size + glm::vec2(- (float) returnWithClear.size.x / 3.6, 12 * resizeFactor);
        glm::vec2 off2 = 0.5f * returnWithClear.size + glm::vec2(- (float) returnWithClear.size.x / 3.0, 12 * resizeFactor);

        glm::vec3 logicalPos(returnWithClear.position + off2, 1);
        glm::vec3 logicalPos1(returnButton.position + off1, 1);


        glm::vec3 transf1 = visMatrix * logicalPos;
        glm::vec3 transf2 = visMatrix * logicalPos1;

        glm::vec2 pixelPos(transf1.x,
            window->GetResolution().y - transf1.y);

        glm::vec2 pixelPos1(transf2.x,
            window->GetResolution().y - transf2.y);

        textRenderer->RenderText(editorReturnClear, pixelPos.x, pixelPos.y, 1.0f, glm::vec3(1, 1, 1));
        textRenderer->RenderText(editorReturn, pixelPos1.x, pixelPos1.y, 1.0f, glm::vec3(1, 1, 1));

        float baseY = (window->GetResolution().y / 2) - 250 * resizeFactor;

        RenderCenteredText("Score: " + std::to_string(game->getScore()), baseY + 160 * resizeFactor, 1.2f, { 1,1,1 });
        RenderCenteredText("Current session high score: " + std::to_string(currentSessionHighScore), baseY + 80 * resizeFactor, 1.2f, { 1,1,1 });
        RenderCenteredText("All-time high score: " + std::to_string(allTimeHighScore), baseY, 1.4f, { 1,1,1 });


        returnMenu = true;
        return;
    }

	// rendering the game background and the black outline that hides the chickens outside of the window in special aspects ratios
    RenderMesh2D(meshes["background"], shaders["VertexColor"], visMatrix);
    RenderMesh2D(meshes["mask1"], shaders["VertexColor"], visMatrix);
    RenderMesh2D(meshes["mask1"], shaders["VertexColor"], visMatrix * transform2D::Translate(0, 820));
    RenderMesh2D(meshes["mask2"], shaders["VertexColor"], visMatrix);
    RenderMesh2D(meshes["mask2"], shaders["VertexColor"], visMatrix * transform2D::Translate(1380, 0));

    game->RenderProjectiles(visMatrix, meshes, shaders, [this](Mesh* mesh, Shader* shader, const glm::mat3& modelMatrix) {
        this->RenderMesh2D(mesh, shader, modelMatrix);
        },
        deltaTime
    );

    // player's ship
    game->RenderPlayer(visMatrix, meshes, shaders, [this](Mesh* mesh, Shader* shader, const glm::mat3& modelMatrix) {
        this->RenderMesh2D(mesh, shader, modelMatrix);
        }
    );

    game->RenderEnemies(visMatrix, meshes, shaders, [this](Mesh* mesh, Shader* shader, const glm::mat3& modelMatrix) {
        this->RenderMesh2D(mesh, shader, modelMatrix);
        },
        deltaTime
    );

    game->RenderRemainingLives(visMatrix, meshes, shaders, [this](Mesh* mesh, Shader* shader, const glm::mat3& modelMatrix) {
        this->RenderMesh2D(mesh, shader, modelMatrix);
        }
    );

	// game collision = camera shake
    if (game->CheckCollisions()) {
        CameraShake(0.5f, 10.0f);
    }

	// disabling depth test for text rendering so that it appears above everything else
    glDisable(GL_DEPTH_TEST);

    std::string score = "Score: " + std::to_string(game->getScore());
    glm::vec3 logicalPos(5, 20, 1);
    glm::vec3 transf1 = visMatrix * logicalPos;
    glm::vec2 pixelPos(transf1.x,
        window->GetResolution().y - transf1.y);
    textRenderer->RenderText(score, pixelPos.x, pixelPos.y, 1.0f,
        glm::vec3(1, 1, 1));
    glEnable(GL_DEPTH_TEST);



}

void Tema1::CheckRestrictions()
{
    canStart = blocksGrid->CheckRestrictions();

}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    glm::vec3 mouseLogic = glm::inverse(visMatrix) * glm::vec3(mouseX, window->GetResolution().y - mouseY, 1);

    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && !mouseSel) {

        if (IsOverStartButton(mouseLogic.x, mouseLogic.y) && canStart) {
			// can start the game
            renderGame = true;
            blocksGrid->SetDrawing();
            InteractObj gameBorderRectangle = {glm::vec2(0.05, 0.05), glm::vec2(1279.95, 719.95), glm::vec2(0), "borderRectangle", 0};
            game = new ChickenInvadersGame(blocksGrid->getDrawing(), 640, glm::vec2(1060, 0), gameBorderRectangle);
        }

		// click for selecting a block to drag with the mouse
        for (auto& obj : blockSelectButtons) {
            if (IsInsideRectangle(mouseLogic, obj.position, obj.size)) {

                mouseSel = true;
                int i = obj.position.y / obj.size.y;
                InteractObj block = selectBlocks.at(i);

                mousePosX = mouseLogic.x - globalOff;
                mousePosY = mouseLogic.y - globalOff;
                mouseMesh = block.meshName;

                mouseHoldBlock = block;
            }
        }
    }

    // right click to erase block from the grid
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT) && !mouseSel) {
        blocksGrid->RemoveLastDrew(mouseLogic);

    }


    if (returnMenu) {
        if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) ) {

            // two button options: build a new ship or restart wih the same one
            if (IsInsideRectangle(mouseLogic, returnWithClear.position, returnWithClear.size)) {
                blocksGrid->ClearDrawing();
                renderGame = false;
                returnMenu = false;
            }

            if (IsInsideRectangle(mouseLogic, returnButton.position, returnButton.size)) {
                renderGame = false;
                returnMenu = false;
            }

        }
    }



}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && mouseSel) {
        glm::vec3 mouseLogic = glm::inverse(visMatrix) * glm::vec3(mouseX, window->GetResolution().y - mouseY, 1);
        for (int i = 0; i < blocksGrid->getMatrix().size(); i++) {
            for (int j = 0; j < blocksGrid->getMatrix()[i].size(); j++) {

                auto& obj = blocksGrid->getMatrix()[i][j][0];
                if (mouseLogic.x >= obj.position.x && mouseLogic.x <= obj.position.x + obj.size.x &&
                    mouseLogic.y >= obj.position.y && mouseLogic.y <= obj.position.y + obj.size.y) {

                    InteractObj placedBlock;
                    placedBlock.meshName = mouseMesh;
                    placedBlock.position.x = obj.position.x + obj.size.x / 2 - globalOff;
                    placedBlock.position.y = obj.position.y + obj.size.y / 2 - globalOff;
                    placedBlock.size = mouseHoldBlock.size;
                    placedBlock.center = ComputeBlockCenter(mouseMesh, unitLen, placedBlock.size);

					// incremented at each addition for correct rendering
                    placedBlock.depth = ++objectDepth;
                    blocksGrid->InsertObject(i, j, placedBlock);
                    mouseSel = false;
                    return;
                }
                
            }
        }
        mouseSel = false;

    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{

}


void Tema1::OnWindowResize(int width, int height)
{
    
    resizeFactor = std::min((float) (width / logicSpace.width), (float) (height / logicSpace.height));
    resizeFactorX = (float)(width / logicSpace.width);
    resizeFactorY = (float)(height / logicSpace.height);

    textRenderer = new TextRenderer(window->props.selfDir, window->props.resolution.x, window->props.resolution.y);
    textRenderer->Load("assets/fonts/Hack-Bold.ttf", 24 * resizeFactor);
    
}


// 2D visualization matrix
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}

void Tema1::CameraShake(float period, float intensity)
{
    shakeTimer = 0;
    shakePeriod = period;
    shakeStrength = intensity;
}

glm::vec2 Tema1::ComputeBlockCenter(const std::string& meshName, float unitLen, glm::vec2 size)
{
    if (meshName == "squareBlock")
        return glm::vec2(unitLen / 2, unitLen / 2);

    if (meshName == "engine")
        return glm::vec2(unitLen / 2, 0);

    if (meshName == "cannon")
        return glm::vec2(unitLen / 2, unitLen * 1.5f);

    if (meshName == "bumper")
        return glm::vec2(unitLen / 2, unitLen);

    // default value for center
    return size * 0.5f;
}


void Tema1::RenderCenteredText(const std::string& text, float y, float scale, glm::vec3 color)
{
    float textWidth = 24.0f * text.length() * scale * 0.6f * resizeFactor;
    float centerX = window->GetResolution().x / 2.0f - textWidth / 2.0f;

    textRenderer->RenderText(text, centerX, y, scale, color);
}

