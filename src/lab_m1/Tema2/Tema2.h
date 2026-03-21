#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/lab_camera.h"
#include "lab_m1/Tema2/Map.h"
#include "lab_m1/Tema1/colors.h"
#include <components/text_renderer.h>
#include "GameMenu.h"

using namespace gfxc;

using namespace trainMap;

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

		// called every frame for the camera to follow the train
        void UpdateNormalCameraBehindTrain();

        void RenderMap(bool miniMap);
        void RenderGameUI();
        void RenderStations(bool miniMap);
        void RenderScene(float deltaTimeSeconds, bool miniMap);
        void RenderRail(string mesh, glm::mat4 modelMatrix, bool miniMap);
        void RenderTrain(bool gameMenu, bool miniMap);

    protected:
        Tema2Camera::Camera* camera;

        // the 3 cameras used
        Tema2Camera::Camera* miniCamera;
        Tema2Camera::Camera* normalCamera;
        Tema2Camera::Camera* endGameCamera;

        Map gameMap;
        
		// map between a symbol and the height at which it is represented (the stations having different heights)
        std::map<string, float> symbolHeight = { {"symbol1", 14.0f}, {"symbol2", 19.0f}, {"symbol3", 19.0f}, {"symbol4", 23.0f} };
        vector<string> tileMeshes = { "grass", "water", "mountain" };

        glm::mat4 projectionMatrix;
        TextRenderer* textRenderer;

		// used for smoothing the camera movement when the train turns
        bool firstCameraUpdate = true;
        float smoothCameraAngle = 0.0f;

		// scale used for changing the size of the minimap (using keys)
        float miniScale = 1.0f;

        bool gameMenu = false;
        
    };
}   // namespace m1
