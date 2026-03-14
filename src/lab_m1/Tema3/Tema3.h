#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"


namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {

  
    public:

        struct FireFly {
            float radius;
            float speed;
            float phase;
            float verticalAmp;
            float verticalSpeed;
            glm::vec3 color;
        };

        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void GenerateRandTrees();
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawTree(glm::vec3 pos, float height);

        void DrawTower(glm::vec3 pos);

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        std::unordered_map<std::string, Texture2D*> mapTextures;
        std::vector<FireFly> fireFlies;
        std::vector<glm::vec3> treePositions;
        std::vector<float> treeHeights;

    };
}   // namespace m1
