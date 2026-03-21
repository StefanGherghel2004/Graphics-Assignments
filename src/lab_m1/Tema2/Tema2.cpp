#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <lab_m1/Tema1/meshObj.h>
#include <lab_m1/Tema1/colors.h>

using namespace std;
using namespace m1;
using namespace trainMap;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{

    // initialisations for the 3 cameras
    miniCamera = new Tema2Camera::Camera();
    miniCamera->Set(glm::vec3(0, 100, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));

    normalCamera = new Tema2Camera::Camera();
    normalCamera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    endGameCamera = new Tema2Camera::Camera();

    // initial camera => train POV (3rd person)
    camera = normalCamera;

    textRenderer = new TextRenderer(window->props.selfDir, window->props.resolution.x, window->props.resolution.y);
    textRenderer->Load("assets/fonts/Hack-Bold.ttf", 24);
    projectionMatrix = glm::perspective(RADIANS(80), window->props.aspectRatio, 0.01f, 200.0f);


    // shader dupa culoarea varfurilor (trebuie schimbat numele)
    {
        Shader* shader = new Shader("MyShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }


    
    // vagoanele pentru cele 4 simboluri
    Mesh* train_car1 = mesh::CreateBox("train_car1", glm::vec3(0, 3.0f, 0), 3, 3.4, 3.5, GREEN07, 0.4f);
    AddMeshToList(train_car1);
    Mesh* train_car2 = mesh::CreateBox("train_car2", glm::vec3(0, 3.0f, 0), 3, 3.4, 3.5, ORANGE2, 0.75f);
    AddMeshToList(train_car2);
    Mesh* train_car3 = mesh::CreateBox("train_car3", glm::vec3(0, 3.0f, 0), 3, 3.4, 3.5, RED, 0.75f);
    AddMeshToList(train_car3);
    Mesh* train_car4 = mesh::CreateBox("train_car4", glm::vec3(0, 3.0f, 0), 3, 3.4, 3.5, BLUE08, 0.75f);
    AddMeshToList(train_car4);

    // componentele trenului
    Mesh* cil = mesh::CreateCylinder("cil", glm::vec3(0, 0, 0), 1.2f, 5, RED, 0.85, true);
    Mesh* box = mesh::CreateBox("box", glm::vec3(0, 0.3f, -3.75f), 3, 3, 2.5f, GREEN, 0.75f);
    Mesh* platform = mesh::CreateBox("platform", glm::vec3(0, -1.3, -1.25), 3, 0.2f, 7.5, YELLOW, 0.75f);
    
    Mesh* train = MergeMeshes(std::vector<Mesh*>{cil, box, platform}, "train");
    AddMeshToList(train);

    // roti pentru tren / vagoane
    Mesh* wheel = mesh::CreateCylinder("wheel", glm::vec3(0, 0, 0), 0.4f, 3, RED, 0.75, false);
    AddMeshToList(wheel);
    
    // sine si variantele lor pentru minimap
    Mesh* grasstrack = mesh::CreateTrack("grasstrack", glm::vec3(0, 0.2, 0), 0.7f, 0.4f, 4, GREY_02, GREY_02, GREY_02, 0.6f, "grass");
    AddMeshToList(grasstrack);
    Mesh* mini_grasstrack = mesh::CreateBox("mini_grasstrack", glm::vec3(0, 0.2, 0), 2, 0.4f, 4, GREY_02, 0.6f);
    AddMeshToList(mini_grasstrack);
    Mesh* watertrack = mesh::CreateTrack("watertrack", glm::vec3(0, 0.2, 0), 0.7f, 0.4f, 4, GREY_02, BROWN, DARK_BROWN, 0.6f, "water");
    AddMeshToList(watertrack);
    Mesh* mini_watertrack = mesh::CreateBox("mini_watertrack", glm::vec3(0, 0.2, 0), 2, 0.4f, 4, DARK_BROWN, 0.6f);
    AddMeshToList(mini_watertrack);
    Mesh* tunneltrack = mesh::CreateTrack("tunneltrack", glm::vec3(0, 0.2, 0), 0.7f, 0.4f, 4, GREY_02, GREY_08, GREY_06, 0.6f, "tunnel");
    AddMeshToList(tunneltrack);
    Mesh* mini_tunneltrack = mesh::CreateBox("mini_tunneltrack", glm::vec3(0, 0.2, 0), 2, 0.4f, 4, GREY_08, 0.6f);
    AddMeshToList(mini_tunneltrack);

    // simboluri si statii in varianta de miniMap (simbolurile dar mai mari)
    Mesh* symbol1 = mesh::CreateSymbol("symbol1", glm::vec3(0, 0, 0), 10, GREEN02, 0.3, 1);
    AddMeshToList(symbol1);
    Mesh* station1 = mesh::CreateSymbol("mini_station1", glm::vec3(0, 0, 0), 20, GREEN02, 0.3, 1);
    AddMeshToList(station1);

    Mesh* symbol2 = mesh::CreateSymbol("symbol2", glm::vec3(0, 0, 0), 10, ORANGE2, 0.5, 2);
    AddMeshToList(symbol2);
    Mesh* station2 = mesh::CreateSymbol("mini_station2", glm::vec3(0, 0, 0), 20, ORANGE2, 0.5, 2);
    AddMeshToList(station2);

    Mesh* symbol3 = mesh::CreateSymbol("symbol3", glm::vec3(0, 0, 0), 10, RED, 0.5, 3);
    AddMeshToList(symbol3);
    Mesh* station3 = mesh::CreateSymbol("mini_station3", glm::vec3(0, 0, 0), 20, RED, 0.5, 3);
    AddMeshToList(station3);

    Mesh* symbol4 = mesh::CreateSymbol("symbol4", glm::vec3(0, 0, 0), 10, BLUE08, 0.3, 4);
    AddMeshToList(symbol4);
    Mesh* station4 = mesh::CreateSymbol("mini_station4", glm::vec3(0, 0, 0), 20, BLUE08, 0.3, 4);
    AddMeshToList(station4);

    // mesh de tren pentru minimap
    Mesh* miniTrain = mesh::CreateBox("mini_train", glm::vec3(0, 50, 0), 8, 8, 8, RED, 0.8);
    AddMeshToList(miniTrain);

    // setul de statii cu accentul de gold
    for (int i = 0; i < 5; i++) {
        Mesh* station = mesh::CreateTrainStation("station" + to_string(i), glm::vec3(0, 6, 0), 25, 12, 15, GREY_099, GREY_06, GOLD, 0.6f, i);
        AddMeshToList(station);

    }

    // mesh-uri pentru teren
    Mesh* mountain = mesh::Create4PPoly(
        "mountain",
        VertexFormat(glm::vec3(-2, 0, -2), BROWN),
        VertexFormat(glm::vec3(-2, 0, 2), BROWN),
        VertexFormat(glm::vec3(2, 0, 2), BROWN),
        VertexFormat(glm::vec3(2, 0, -2), BROWN),
        0.8f
    );
    AddMeshToList(mountain);

    Mesh* grass = mesh::Create4PPoly(
        "grass",
        VertexFormat(glm::vec3(-2, 0, -2), GREEN),
        VertexFormat(glm::vec3(-2, 0, 2), GREEN),
        VertexFormat(glm::vec3(2, 0, 2), GREEN),
        VertexFormat(glm::vec3(2, 0, -2), GREEN),
        0.6f
    );
    AddMeshToList(grass);

    Mesh* water = mesh::Create4PPoly(
        "water",
        VertexFormat(glm::vec3(-2, 0, -2), WATER),
        VertexFormat(glm::vec3(-2, 0, 2), WATER),
        VertexFormat(glm::vec3(2, 0, 2), WATER),
        VertexFormat(glm::vec3(2, 0, -2), WATER),
        0.4f
    );
    AddMeshToList(water);

    gameMap = Map(50, 60, 4.0f, glm::vec3(0, 0, 0), tileMeshes);

}


void Tema2::FrameStart()
{
    // in meniul de final fundalul este un negru total iar in joc un albastru foarte inchis
    if (gameMenu) {
        glClearColor(0, 0, 0, 1);

    }
    else {
        glClearColor(0, 0, 0.15, 1);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{

    // logica de endgame
    if (gameMap.EndGame()) {
        gameMenu = true;

        // daca abia s-a intrat in endGame se schimba camera la cea de endGame
        if (camera != endGameCamera) {
            camera = endGameCamera;
            camera->Set(glm::vec3(0, 2, 10.0f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
        }

        // se randeaza meniul de endgame
        gameMap.getMenu()->Render(textRenderer, window->GetResolution(), gameMap.getScore(), gameMap.getTime());
        RenderTrain(true, false);

        // cand jucatorul a apasat space ca sa continue cu un nou joc
        if (gameMap.EndMenuEnded()) {
            gameMap.getMenu()->Reset(); // resetare meniu de final
            gameMap = Map(50, 60, 4.0f, glm::vec3(0, 0, 0), tileMeshes);
            textRenderer->Load("assets/fonts/Hack-Bold.ttf", 24);
            // se reseteaza la starea initiala de joc mai multe variabile
            miniScale = 1.0f;
            firstCameraUpdate = true; // util deoarece altfel ar prelua rotatia in care a pierdut jucatorul la jocul anterior
            camera = normalCamera;
            gameMenu = false;
            
        }
        return;
    }

    // camera care urmareste trenul
    UpdateNormalCameraBehindTrain();

    // randarea scenei principale
    camera = normalCamera;
    RenderScene(deltaTimeSeconds, false);

    glm::ivec2 resolution = window->GetResolution();

    resolution.x = resolution.x * 0.3f * miniScale;
    resolution.y = resolution.y * 0.3f * miniScale;

    glViewport(0, 0, resolution.x, resolution.y);

    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, resolution.x, resolution.y);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    // proiectia pentru minimap
    projectionMatrix = glm::ortho(-128.0f, 125.0f, -108.0f, 110.0f, 0.1f, 120.0f);
    camera = miniCamera;

    // randare cu minimap
    RenderScene(deltaTimeSeconds, true);
    // proiectia pentru camera normala
    projectionMatrix = glm::perspective(RADIANS(80), window->props.aspectRatio, 0.01f, 250.0f);
    camera = normalCamera;

    // miscarea trenuloui si actualizarea statiilor
    gameMap.MoveTrain(deltaTimeSeconds);
    for (auto& station : gameMap.getStations()) {
        station->UpdateStation(deltaTimeSeconds);
    }


}


void Tema2::RenderScene(float deltaTimeSeconds, bool miniMap)
{

    RenderTrain(false, miniMap);

    RenderMap(miniMap);

    RenderStations(miniMap);

    // scor, timp ramas, timp total
    if (!miniMap) {
        RenderGameUI();
    }

}


void Tema2::RenderRail(string mesh, glm::mat4 modelMatrix, bool miniMap)
{
    string type = miniMap ? "mini_" : "";

    if (mesh == "grass") {
        RenderMesh(meshes[type + "grasstrack"], shaders["VertexColor"], modelMatrix);
    }
    else if (mesh == "water") {
        RenderMesh(meshes[type + "watertrack"], shaders["VertexColor"], modelMatrix);
    }
    else if (mesh == "mountain") {
        RenderMesh(meshes[type + "tunneltrack"], shaders["VertexColor"], modelMatrix);
    }

}

void Tema2::RenderTrain(bool gameMenu, bool miniMap)
{
    Train* train = gameMap.getTrain();
    glm::vec2 trainPos = gameMap.getTrainPos();

    glm::mat4 trainTransform = glm::translate(glm::mat4(1.0f), glm::vec3(trainPos.x, 2.8f, trainPos.y)) * glm::rotate(glm::mat4(1.0f), RADIANS(train->angle), glm::vec3(0, 1, 0));
    if (gameMenu) {
        // in meniul de final locomotiva sta in origine
        trainTransform = glm::mat4(1);
    }

    string mesh = miniMap ? "mini_train" : "train";
    RenderMesh(meshes[mesh], shaders["VertexColor"], trainTransform);


    // randarea rotilor locomotivei
    glm::mat4 wheelRot = glm::rotate(glm::mat4(1.0f), RADIANS(90.0f), glm::vec3(0, 1, 0));

    for (int i = 0; i < 6; i++) {
        glm::mat4 wheelLoc = glm::translate(glm::mat4(1.0f), glm::vec3(0, -1.8f, -4.0f + i * 1.0f)) * wheelRot;
        glm::mat4 wheelModel = trainTransform * wheelLoc;

        RenderMesh(meshes["wheel"], shaders["VertexColor"], wheelModel);
    }

    // randare vagoane
    if (gameMenu || miniMap) {
        return;
    }

   for (int i = 0; i < train->wagons.size(); i++) {
        Wagon& w = train->wagons[i];

        glm::mat4 wagonTransform = glm::translate(glm::mat4(1.0f), w.position) * glm::rotate(glm::mat4(1.0f), RADIANS(w.rail->angle), glm::vec3(0, 1, 0));

        RenderMesh(meshes["train_car" + std::to_string(train->cargo[i])], shaders["MyShader"], wagonTransform);

        // rotile vagoanelor
        for (int i = 0; i < 2; i++) {
            glm::mat4 wheelLoc = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.9f, - 1.25 + i * 2.5)) * wheelRot;
            glm::mat4 wheelModel = wagonTransform * wheelLoc;

            RenderMesh(meshes["wheel"], shaders["VertexColor"], wheelModel);
        }
   }


}


void Tema2::FrameEnd()
{

}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // TODO(student): Get shader location for uniform mat4 "Model"
    int loc_model = glGetUniformLocation(shader->program, "Model");

    // TODO(student): Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // TODO(student): Get shader location for uniform mat4 "View"
    int loc_view = glGetUniformLocation(shader->program, "View");

    // TODO(student): Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // TODO(student): Get shader location for uniform mat4 "Projection"
    int loc_projection = glGetUniformLocation(shader->program, "Projection");

    // TODO(student): Set shader uniform "Projection" to projectionMatrix
    glm::mat4 projectionMatrix = this->projectionMatrix;

    glUniformMatrix4fv(loc_projection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // TODO(student): If you look closely in the `Init()` and `Update()`
    // functions, you will see that we have three objects which we load
    // and use in three different ways:
    // - LoadMesh   + LabShader (this lab's shader)
    // - CreateMesh + VertexNormal (this shader is already implemented)
    // - CreateMesh + LabShader (this lab's shader)
    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{

    // in camera de meniu de final putem doar sa rotim camera
    if (camera == endGameCamera) {
        return;
    }

    // gestionare marime/micsorare minimap
    float scaleSpeed = 3.0f;

    if (window->KeyHold(GLFW_KEY_1) || window->KeyHold(GLFW_KEY_KP_1)) {
        miniScale += scaleSpeed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_2) || window->KeyHold(GLFW_KEY_KP_2)) {
        miniScale -= scaleSpeed * deltaTime;
    }

    // clamp rezonabil
    miniScale = glm::clamp(miniScale, 0.5f, 1.5f);

 
    if (gameMap.getTrain()->stop) {
        return;
    }

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;
        float f = 10.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            camera->TranslateForward(cameraSpeed * deltaTime * f);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-cameraSpeed * deltaTime * f);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->TranslateForward(-cameraSpeed * deltaTime * f);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(cameraSpeed * deltaTime * f);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            camera->TranslateUpward(-cameraSpeed * deltaTime * f);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            camera->TranslateUpward(cameraSpeed * deltaTime * f);
        }

    }


}


void Tema2::OnKeyPress(int key, int mods)
{
    
    // actualizarea controloalelor pentru tren
    gameMap.UpdateKeyPressed(key);
   
    // gestionarea iesirii din meniul de final
    if (gameMap.getMenu()->IsActive()) {

        if (key == GLFW_KEY_SPACE) {

            gameMap.getMenu()->setInactive();
        }


    }

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
            camera->RotateFirstPerson_OX(-deltaY * sensivityOY);

        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
    // ca sa poata scrie in continuare in chenarul corect
    textRenderer = new TextRenderer(window->props.selfDir, window->props.resolution.x, window->props.resolution.y);
    textRenderer->Load("assets/fonts/Hack-Bold.ttf", 24);
}


static float NormalizeAngle(float angle)
{
    angle = fmod(angle, 360.0f);
    if (angle < 0)
        angle += 360.0f;
    return angle;
}

void Tema2::UpdateNormalCameraBehindTrain()
{
   
    if (gameMap.getTrain()->stop) {
        return;
    }

    Train* train = gameMap.getTrain();
    glm::vec2 trainPos2D = gameMap.getTrainPos();
    glm::vec3 trainPos(trainPos2D.x, 2.8f, trainPos2D.y);

    float trainAngle = train->angle;

    // la inceputul unui joc se stabileste prima valoare pentru unghiul smooth ca unghiul initial
    if (firstCameraUpdate) {
        smoothCameraAngle = trainAngle;
        firstCameraUpdate = false;
    }

    float time = GetLastFrameTime();
    // vireaza 90 de grade in 0.5 secunde
    float maxTurnSpeed = 180.0f;

    float angleDiff = trainAngle - smoothCameraAngle;

    if (angleDiff > 180.0f) {
        angleDiff -= 360.0f;
    }

    if (angleDiff < -180.0f) {
        angleDiff += 360.0f;
    }

    // clamp ca sa nu poata vira instant
    float maxDiff = maxTurnSpeed * time;
    angleDiff = glm::clamp(angleDiff, -maxDiff, maxDiff);

    smoothCameraAngle += angleDiff;
    smoothCameraAngle = NormalizeAngle(smoothCameraAngle);

    float lookDown = -0.3f;

    glm::vec3 forward(sin(RADIANS(smoothCameraAngle)), lookDown, cos(RADIANS(smoothCameraAngle)));
    forward = glm::normalize(forward);

    // se gestioneaza departarea camerei de locomotiva cu adaugarea fiecarui vagon
    float distance = 12 + train->wagons.size() * 5.0f;
    float height = 5.0f;

    glm::vec3 camPos = trainPos - forward * distance + glm::vec3(0, height, 0);
    glm::vec3 target = trainPos + forward * 2.0f + glm::vec3(0, 6.0f, 0);

    normalCamera->Set(camPos, target, glm::vec3(0, 1, 0));
}


void Tema2::RenderMap(bool miniMap)
{
    for (int i = 0; i < gameMap.N; i++) {
        for (int j = 0; j < gameMap.M; j++) {
            auto& cell = gameMap.getTileInteract(i, j);

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(cell.position.x, 0, cell.position.y));

            RenderMesh(meshes[cell.meshName], shaders["VertexColor"], modelMatrix);

            if (gameMap.getTileRail(i, j) == nullptr) {
                continue;
            }

            if (gameMap.getTileRail(i, j)->rotated) {
                modelMatrix = modelMatrix * glm::rotate(glm::mat4(1), RADIANS(90.0f), glm::vec3(0, 1, 0));
            }

            RenderRail(cell.meshName, modelMatrix, miniMap);

            // intersectie
            if (gameMap.getTileRail(i, j)->junction) {
                modelMatrix = modelMatrix * glm::rotate(glm::mat4(1), RADIANS(90.0f), glm::vec3(0, 1, 0));
                RenderRail(cell.meshName, modelMatrix, miniMap);

            }



        }
    }
}

void Tema2::RenderGameUI()
{
    string score = "Score: " + std::to_string(gameMap.getScore());
    string time = "Time: " + std::to_string((int)gameMap.getTime()) + "s";
    string remaining = "Remaining: " + std::to_string((int)gameMap.getRemainingTime() + 1) + "s";

    glm::ivec2 res = window->GetResolution();

    textRenderer->RenderText(score, 10, 10, 1.0f, WHITE);
    textRenderer->RenderText(time, window->GetResolution().x - 14.4 * time.length() - 10, 10, 1.0f, WHITE);
    GameMenu::RenderCenteredText(textRenderer, 14.4, remaining, glm::vec2(res.x * 0.5f, 10), RED);
}

void Tema2::RenderStations(bool miniMap)
{
    for (auto& station : gameMap.getStations()) {

        glm::vec2 pos = station->getPos();
        glm::mat4 rot = glm::rotate(glm::mat4(1), RADIANS(station->getAngle()), glm::vec3(0, 1, 0));

        if (station->isCentralStation()) {

            // statie centrala colorata in rosu in functie de timp
            float alertFactor = station->getAlertFactor();

            // stabilirea valorii uniformei din shader 
            Shader* normalShader = shaders["MyShader"];
            glUseProgram(normalShader->program);
            int loc_alert = glGetUniformLocation(normalShader->program, "alertFactor");
            glUniform1f(loc_alert, alertFactor);
            // in miniMap statia centrala e mai mare
            glm::mat4 scale = miniMap ? glm::scale(glm::mat4(1), glm::vec3(1.5, 1, 1.5)) : glm::mat4(1);

            RenderMesh(meshes[station->getMesh()], normalShader, glm::translate(glm::mat4(1), glm::vec3(pos.x, 0, pos.y)) * rot * scale);
            // setare la 0 pentru ca restul mesh-urilor randate cu acest shader sa nu fie afectate
            glUniform1f(loc_alert, 0.0f);

            // simbolurile comenzii
            auto& order = station->getOrder();

            for (int i = 0; i < order.size(); i++) {

                if (order[i] < 0) {
                    continue;
                }

                float heigth;
                if (order[i] == 1) {
                    heigth = 20;
                }
                else {
                    heigth = 22;
                }

                glm::vec3 symbolPos(pos.x, heigth, 26 + pos.y - 14 * i);

                // al treilea simbol (din 5) e la centru => facem ca simbolurile sa fie orientate fata de directia dintre simbolul din centru si tren
                glm::vec3 centerSymbolPos(pos.x, heigth, 26 + pos.y - 14 * 2);
                glm::vec2 trainPos2D = gameMap.getTrainPos();
                glm::vec3 trainPos3D(trainPos2D.x, symbolPos.y, trainPos2D.y);
                glm::vec3 dir = glm::normalize(trainPos3D - centerSymbolPos);
                float angleY = atan2(dir.x, dir.z);

                glm::mat4 trans;

                if (miniMap) {
                    // in cazul minimapului reprezint simbolurile din comanda mai departe de gara centrala (nu deasupra)
                    trans = glm::translate(glm::mat4(1.0f), symbolPos - glm::vec3(20, 0, 0)) * glm::rotate(glm::mat4(1.0f), RADIANS(-90.0f), glm::vec3(1, 0, 0));
                }
                else {
                    trans = glm::translate(glm::mat4(1.0f), symbolPos) * glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0, 1, 0));
                }

                RenderMesh(meshes["symbol" + std::to_string(order[i])], shaders["VertexColor"], trans);
            }

        }
        // celelalte statii
        else {

            if (miniMap) {
                // in cazul miniMap-ului rotim pe axa 0x simbolul ca sa sa fie in planul vizibil si o ridicam mai sus
                rot = glm::rotate(glm::mat4(1), RADIANS(-90.0f), glm::vec3(1, 0, 0));
                rot = glm::translate(glm::mat4(1), glm::vec3(0, 30, 0)) * rot;
            }

            RenderMesh(meshes[station->getMesh(miniMap)], shaders["MyShader"], glm::translate(glm::mat4(1), glm::vec3(pos.x, 0, pos.y)) * rot);


            if (!miniMap) {
                if (station->getCounter() >= 1) {
                    glm::vec3 symbolPos(pos.x, symbolHeight.at(station->getSymbolMesh()), pos.y);

                    // ca mai sus simbolurile se orienteaza dupa pozitia trenului pe harta
                    glm::vec2 trainPos2D = gameMap.getTrainPos();
                    glm::vec3 trainPos3D(trainPos2D.x, symbolPos.y, trainPos2D.y);
                    glm::vec3 dir = glm::normalize(trainPos3D - symbolPos);
                    float angleY = atan2(dir.x, dir.z);

                    glm::mat4 trans = glm::translate(glm::mat4(1.0f), symbolPos) * glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0, 1, 0));

                    RenderMesh(meshes[station->getSymbolMesh()], shaders["VertexColor"], trans);
                }
            }

        }
    }
}