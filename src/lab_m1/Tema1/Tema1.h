#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/meshObj.h"
#include "lab_m1/Tema1/Grid.h"
#include "lab_m1/Tema1/ChickenInvadersGame.h"
#include <components/text_renderer.h>
#include <SFML/Audio.hpp>

using namespace mesh;
using namespace grid;
using namespace gfxc;

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {
            }
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {
            }
            float x;
            float y;
            float width;
            float height;
        };


    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        bool IsOverStartButton(int mouseX, int mouseY);
        void DrawGrid();
        void RenderGame(float deltaTime);
        void CheckRestrictions();
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

        void CameraShake(float period, float intensity);

        glm::vec2 ComputeBlockCenter(const std::string& meshName, float unitLen, glm::vec2 size);

        void RenderCenteredText(const std::string& text, float y, float scale, glm::vec3 color);

    protected:
        glm::mat3 modelMatrix;
        glm::mat3 visMatrix;

        const float unitLen = 40;
        const int col = 16;
        const int row = 10;

		// used for the position of the dragged block
        int mousePosX;
        int mousePosY;
        bool mouseSel;

        // redundant variables
        std::string mouseMesh;
        InteractObj mouseHoldBlock;

		// max number of components (set to 10 in init)
        int components;

        // true if the ship respects the restrictions
        bool canStart;

        // game in course
        bool renderGame;

        ViewportSpace viewSpace;
        LogicSpace logicSpace;

        // editor grid
        Grid *blocksGrid;

		// select buttons for drag and drop
        std::vector<InteractObj> blockSelectButtons;

        InteractObj startButton;

        // interactive buttons in the return menu
        InteractObj returnWithClear;
        InteractObj returnButton;

		// idx of the selection block that the mouse is hovering over
        int hoveredBlockIndex;

		// map between the selection blocks and the corresponding ship blocks
        const std::unordered_map<int, InteractObj> selectBlocks = {
            {0, {glm::vec2(0, 0), glm::vec2(unitLen, unitLen), glm::vec2(unitLen / 2, unitLen / 2), "squareBlock", 0}},
            {1, {glm::vec2(0, 0), glm::vec2(unitLen, unitLen), glm::vec2(unitLen / 2, 0), "engine", 0}},
            {2, {glm::vec2(0, 0), glm::vec2(unitLen, unitLen * 3), glm::vec2(unitLen / 2, unitLen * 1.5f), "cannon", 0}},
            {3, {glm::vec2(0, 0), glm::vec2(unitLen, unitLen * 2), glm::vec2(unitLen / 2, unitLen), "bumper", 0}}
        };


		// for all objects the offset to the grabbing point/ center of the controlled block
        const int globalOff = unitLen / 2;

        // used for rendering in the correct order
        int objectDepth;

        bool returnMenu;

		// factors used for resizing the window and maintaining the aspect ratio
        float resizeFactor;
        float resizeFactorX;
        float resizeFactorY;

        ChickenInvadersGame* game;

        TextRenderer* textRenderer;

		// camera shake parameters
        float shakeTimer = 0;
        float shakePeriod = 0;
        float shakeStrength = 0;

        // handling statistics 
        int currentSessionHighScore = 0;
        int allTimeHighScore = 0;

        sf::Music backgroundMusic;

        
    };
}   // namespace m1
