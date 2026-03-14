#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/meshObj.h"
#include "lab_m1/Tema1/Grid.h"
#include "lab_m1/Tema1/ChickenInvadersGame.h"
#include <components/text_renderer.h>

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

        // variabile folosite pentru pozitia blocului randat la drag
        int mousePosX;
        int mousePosY;

        // daca avem un bloc pe care l tragem
        bool mouseSel;

        // mouseMesh este renundant dar nu mai am timp sa schimb asta
        std::string mouseMesh;
        InteractObj mouseHoldBlock;

        // numarul de componente maxim (10 setat in init)
        int components;

        // canStart reprezinta daca nava desentata respecta constrangerile
        bool canStart;

        // bool ce zice daca jocul este in desfasurare
        bool renderGame;

        ViewportSpace viewSpace;
        LogicSpace logicSpace;

        // grid-ul din editor
        Grid *blocksGrid;

        // butoanele de selectat pentru drag and drop
        std::vector<InteractObj> blockSelectButtons;

        InteractObj startButton;

        // butoanele din meniul din intoarcere dupa ce jcuatorul pierde
        InteractObj returnWithClear;
        InteractObj returnButton;


        // index-ul blocului de selectie  peste care este mouse-ul
        int hoveredBlockIndex;

        // map ca sa facem transformarea dintre un bloc de selectie si ce bloc de nava reprezinta acesta
        const std::unordered_map<int, InteractObj> selectBlocks = {
            {0, {glm::vec2(0, 0), glm::vec2(unitLen, unitLen), glm::vec2(unitLen / 2, unitLen / 2), "squareBlock", 0}},
            {1, {glm::vec2(0, 0), glm::vec2(unitLen, unitLen), glm::vec2(unitLen / 2, 0), "engine", 0}},
            {2, {glm::vec2(0, 0), glm::vec2(unitLen, unitLen * 3), glm::vec2(unitLen / 2, unitLen * 1.5f), "cannon", 0}},
            {3, {glm::vec2(0, 0), glm::vec2(unitLen, unitLen * 2), glm::vec2(unitLen / 2, unitLen), "bumper", 0}}
        };


        // pentru toate obiectele offset-ul pana la punctul de apucare/ centrul blocului controlat
        const int globalOff = unitLen / 2;

        // folosit pentru a putea randa obiectele pe baza unei ordini de randare (objectDepth este incrementat mereu la adaugarea unui element pe grid)
        int objectDepth;

        // daca ne aflam in meniul de return
        bool returnMenu;

        // resize factor x si y nu sunt folosite, parametrii pe care i-am folosit la partea de scris in meniul de intoarcere
        float resizeFactor;
        float resizeFactorX;
        float resizeFactorY;

  
        ChickenInvadersGame* game;

        TextRenderer* textRenderer;

        // parametrii ce controleaza cameraShake
        float shakeTimer = 0;
        float shakePeriod = 0;
        float shakeStrength = 0;

        // parametrii ce tin cont de cel mai mare scor din sesiunea curenta si all time (este extras si scris intr-un fisier cel all-time)
        int currentSessionHighScore = 0;
        int allTimeHighScore = 0;
        
    };
}   // namespace m1
