    #include "lab_m1/Tema3/Tema3.h"

    #include <vector>
    #include <string>
    #include <iostream>

    using namespace std;
    using namespace m1;


    /*
     *  To find out more about `FrameStart`, `Update`, `FrameEnd`
     *  and the order in which they are called, see `world.cpp`.
     */


    Tema3::Tema3()
    {
    }


    Tema3::~Tema3()
    {
    }

    void Tema3::Init()
    {
        const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures");

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "wood.jpg").c_str(), GL_REPEAT);
            mapTextures["wood"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "leaves.png").c_str(), GL_REPEAT);
            mapTextures["leaves"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpg").c_str(), GL_REPEAT);
            mapTextures["snow"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "green.png").c_str(), GL_REPEAT);
            mapTextures["green"] = texture;
        }

        {
            Texture2D* texture = new Texture2D();
            texture->Load2D(PATH_JOIN(sourceTextureDir, "black.jpg").c_str(), GL_REPEAT);
            mapTextures["black"] = texture;
        }



        {
            Mesh* mesh = new Mesh("box");
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
            meshes["box"] = mesh;
        }

        {
            Mesh* mesh = new Mesh("sphere");
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
            meshes["sphere"] = mesh;
        }

        {
            Mesh* mesh = new Mesh("plane");
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
            meshes["plane"] = mesh;
        }

        {
            Mesh* mesh = new Mesh("cone");
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
            meshes["cone"] = mesh;
        }

        {
            Mesh* mesh = new Mesh("cylinder");
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cylinder.obj");
            meshes["cylinder"] = mesh;
        }

        {
            Shader* shader = new Shader("LabShader");
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
            shader->CreateAndLink();
            shaders["LabShader"] = shader;
        }

        {
            srand(time(NULL));
            for (int i = 0; i < 8; i++) {
                FireFly ff;
                ff.radius = 2.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f;
                ff.speed = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.0f;
                ff.phase = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;
                ff.verticalAmp = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.3f;
                ff.verticalSpeed = 1.0f + static_cast<float>(rand()) / RAND_MAX * 1.0f;
                ff.color = glm::vec3(0.6f + static_cast<float>(rand()) / RAND_MAX * 0.4f, 0.6f + static_cast<float>(rand()) / RAND_MAX * 0.4f, 0.0f); // galben-verde
                fireFlies.push_back(ff);
            }
        }


        GenerateRandTrees();

    }


    void Tema3::GenerateRandTrees()
    {
        treePositions.clear();
        treeHeights.clear();

        int numTrees = 60;
        float minDistOrigin = 5.0f;
        float minDistTrees = 3.0f;

        srand(time(NULL));

        for (int i = 0; i < numTrees; i++) {
            glm::vec3 pos;
            bool valid = false;
            int attempts = 0;

            while (!valid && attempts < 100) {
                float x = -25.0f + static_cast<float>(rand()) / RAND_MAX * 50.0f;
                float z = -25.0f + static_cast<float>(rand()) / RAND_MAX * 50.0f;

                pos = glm::vec3(x, 0, z);

                if (glm::length(glm::vec2(pos.x, pos.z)) < minDistOrigin) {
                    attempts++;
                    continue;
                }

                valid = true;
                for (const auto& other : treePositions) {
                    if (glm::length(glm::vec2(pos.x - other.x, pos.z - other.z)) < minDistTrees) {
                        valid = false;
                        break;
                    }
                }

                attempts++;
            }

            if (valid) {
                treePositions.push_back(pos);

                float height = 3.0f + static_cast<float>(rand()) / RAND_MAX * 2.0f;
                treeHeights.push_back(height);
            }
        }
    }

    void Tema3::FrameStart()
    {
        glClearColor(0.5f, 0.5f, 0.5f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::ivec2 res = window->GetResolution();
        glViewport(0, 0, res.x, res.y);
    }


    void Tema3::Update(float deltaTimeSeconds)
    {
        Shader* shader = shaders["LabShader"];
        glUseProgram(shader->program);
        glm::mat4 model;
        
        glm::vec3 cameraPos = GetSceneCamera()->m_transform->GetWorldPosition();

        glUniform3f(glGetUniformLocation(shader->program, "cameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);
        glUniform3f(glGetUniformLocation(shader->program, "fogColor"), 0.5f, 0.5f, 0.5f); // gri
        glUniform1f(glGetUniformLocation(shader->program, "fogDistance"), 15.0f);

        glUniform1f(glGetUniformLocation(shader->program, "repeatFactor"), 75.0f);
        model = glm::scale(glm::mat4(1), glm::vec3(50, 1, 50));
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], model, mapTextures["snow"]);
        glUniform1f(glGetUniformLocation(shader->program, "repeatFactor"), 1.0f);

        for (int i = 0; i < treePositions.size(); i++) {
            DrawTree(treePositions[i], treeHeights[i]);
        }

        glUniform1i(glGetUniformLocation(shader->program, "isTrunk"), 0);
        glUniform1i(glGetUniformLocation(shader->program, "isLeaves"), 0);
        glUniform1f(glGetUniformLocation(shader->program, "BendStrength"), 0.0f);

        DrawTower(glm::vec3(0, 0, 0));

        glUniform1i(glGetUniformLocation(shader->program, "isFireFly"), 1);

        glm::vec3 towerPos(0, 0, 0);
        float towerHeight = 6.0f;

        float time = Engine::GetElapsedTime();

        for (int i = 0; i < fireFlies.size(); i++) {
            const FireFly& ff = fireFlies[i];

            float angle = ff.speed * time + ff.phase;
            float x = towerPos.x + ff.radius * cos(angle);
            float z = towerPos.z + ff.radius * sin(angle);
            float y = towerPos.y + towerHeight * 0.9f + ff.verticalAmp * sin(ff.verticalSpeed * time + ff.phase);

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
            model = glm::scale(model, glm::vec3(0.1f));

            glUniform3fv(glGetUniformLocation(shader->program, "fireFlyColor"), 1, glm::value_ptr(ff.color));
            RenderSimpleMesh(meshes["sphere"], shader, model, nullptr);
        }

        glUniform1i(glGetUniformLocation(shader->program, "isFireFly"), 0);
    }

    void Tema3::FrameEnd()
    {
    
    }

    void Tema3::DrawTree(glm::vec3 pos, float height)
    {
        glm::mat4 model;

        model = glm::translate(glm::mat4(1), pos + glm::vec3(0, 1.8f, 0));
        model = glm::scale(model, glm::vec3(0.3f, 1.0f, 0.3f));
    
        Shader* shader = shaders["LabShader"];
        glUseProgram(shader->program);

        glUniform1i(glGetUniformLocation(shader->program, "isTrunk"), 1);
        glUniform1i(glGetUniformLocation(shader->program, "isLeaves"), 0);

        glUniform1f(glGetUniformLocation(shader->program, "TreeHeight"), height);
        glUniform1f(glGetUniformLocation(shader->program, "BendStrength"), 0.0f);
        glUniform1f(glGetUniformLocation(shader->program, "BendFrequency"), 0.0f);
        glUniform1f(glGetUniformLocation(shader->program, "BendPhase"), 0.0f);

        RenderSimpleMesh(meshes["cylinder"], shader, model, mapTextures["wood"]);

        int numSegments = 6;
        float baseHeight = 0.4f * height;
        float segmentHeight = (height - baseHeight) / numSegments;

        for (int i = 0; i < numSegments; i++) {
            float h = baseHeight + i * segmentHeight;
            float scale = 1.2f - i * 0.15f;

  
            glm::mat4 modelSegment = glm::translate(glm::mat4(1), pos + glm::vec3(0, h, 0));
            modelSegment = glm::scale(modelSegment, glm::vec3(scale, 0.3f * (height / 3.5f), scale));

            Shader* shader = shaders["LabShader"];
            glUseProgram(shader->program);

            glUniform1i(glGetUniformLocation(shader->program, "isTrunk"), 0);
            glUniform1i(glGetUniformLocation(shader->program, "isLeaves"), 1);

            glUniform1f(glGetUniformLocation(shader->program, "TreeHeight"), 3.0f);
            glUniform1f(glGetUniformLocation(shader->program, "BendFrequency"), 2.0f);
            glUniform1f(glGetUniformLocation(shader->program, "BendStrength"), 1.0f);
            glUniform1i(glGetUniformLocation(shader->program, "Level"), i / 2);

            float phase = pos.x * 3.5f + pos.z * 6.0f;
            glUniform1f(glGetUniformLocation(shader->program, "BendPhase"), phase);

            RenderSimpleMesh(meshes["box"], shader, modelSegment, mapTextures["leaves"], mapTextures["green"]);
        }
    }

    void Tema3::DrawTower(glm::vec3 pos)
    {
        glm::mat4 model;
        float h = 6.0f;
        float d = 1.2f;

        for (int x = -1; x <= 1; x += 2)
            for (int z = -1; z <= 1; z += 2) {
                model = glm::translate(glm::mat4(1), pos + glm::vec3(x * d, h / 2, z * d));
                model = glm::scale(model, glm::vec3(0.2f, h, 0.2f));
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], model, mapTextures["wood"]);
            }

        model = glm::translate(glm::mat4(1), pos + glm::vec3(0, h * 0.75f, 0));
        model = glm::scale(model, glm::vec3(3, 0.3f, 3));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], model, mapTextures["wood"]);

        model = glm::translate(glm::mat4(1), pos + glm::vec3(0, h + 0.6f, 0));
        model = glm::scale(model, glm::vec3(2, 0.6f, 2));
        RenderSimpleMesh(meshes["cone"], shaders["LabShader"], model, mapTextures["wood"]);

        model = glm::translate(glm::mat4(1), pos + glm::vec3(0, h * 0.75, -1.6));
        model = glm::rotate(model, RADIANS(45), glm::vec3(1, 0, 0));
        model = glm::scale(model, glm::vec3(0.2f));
        RenderSimpleMesh(meshes["cone"], shaders["LabShader"], model, mapTextures["black"]);

        model = glm::translate(glm::mat4(1), pos + glm::vec3(0, h * 0.75, 1.6));
        model = glm::rotate(model, RADIANS(-45), glm::vec3(1, 0, 0));
        model = glm::scale(model, glm::vec3(0.2f));
        RenderSimpleMesh(meshes["cone"], shaders["LabShader"], model, mapTextures["black"]);
    }



    void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
    {
        if (!mesh || !shader || !shader->GetProgramID())
            return;

        // Render an object using the specified shader and the specified position
        glUseProgram(shader->program);

        // Bind model matrix
        GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
        glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        // Bind view matrix
        glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
        int loc_view_matrix = glGetUniformLocation(shader->program, "View");
        glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

        // Bind projection matrix
        glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
        int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
        glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        float time = Engine::GetElapsedTime();
        glUniform1f(glGetUniformLocation(shader->program, "Time"), time);


        // TODO(student): Set any other shader uniforms that you need

        if (texture1)
        {

            // TODO(student): Do these:
            glUniform1i(glGetUniformLocation(shader->program, "is_texture_2"), 0);
            // - activate texture location 0
            glActiveTexture(GL_TEXTURE0);
            // - bind the texture1 ID
            glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
            // - send theuniform value
            glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

        }

        if (texture2)
        {
            // TODO(student): Do these:
            glUniform1i(glGetUniformLocation(shader->program, "is_texture_2"), 1);
            // - activate texture location 1
            glActiveTexture(GL_TEXTURE1);
            // - bind the texture2 ID
            glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
            // - send the uniform value
            glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
        }

        // Draw the object
        glBindVertexArray(mesh->GetBuffers()->m_VAO);
        glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
    }


    void Tema3::OnInputUpdate(float deltaTime, int mods)
    {
        if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
        {
            glm::vec3 up = glm::vec3(0, 1, 0);
            glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
            glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
            forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
        }
    }


    void Tema3::OnKeyPress(int key, int mods)
    {
        // Add key press event

        if (key == GLFW_KEY_G) {
             GenerateRandTrees();
        }
    }


    void Tema3::OnKeyRelease(int key, int mods)
    {
        // Add key release event
    }


    void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
    {
        // Add mouse move event
    }


    void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
    {
        // Add mouse button press event
    }


    void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
    {
        // Add mouse button release event
    }


    void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
    {
    }


    void Tema3::OnWindowResize(int width, int height)
    {
    }
