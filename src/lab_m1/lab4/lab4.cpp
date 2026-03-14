#include "lab_m1/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/lab4/transform3D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;


    sunX = 0;
    earthAngle = 0;
    moonAngle = 0;
    earthZ = 0;
    earthX = 0;

    // Initialize tx, ty and tz (the translation steps)
    translateX = 0;
    translateY = 0;
    translateZ = 0;

    // Initialize sx, sy and sz (the scale factors)
    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    // Initialize angular steps
    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
}

void Lab4::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Lab4::RenderScene(float deltaTimeSeconds) {

    float sunStep = 0.2 * deltaTimeSeconds;
    float earthAngStep = 1 * deltaTimeSeconds;
    float moonAngStep = 5 * deltaTimeSeconds;

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // soarele
   
    sunX += sunStep;



    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(sunX, 0, 0);
    modelMatrix *= transform3D::Scale(2, 2, 2);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // pamantul
    earthAngle += earthAngStep;
    modelMatrix = glm::mat4(1);
    earthZ = cosf(earthAngle) * earthRadius;
    earthX = sunX + sinf(earthAngle) * earthRadius;
    modelMatrix *= transform3D::Translate(sunX, 0, 0);
    modelMatrix *= transform3D::RotateOY(earthAngle);
    modelMatrix *= transform3D::Translate(0, 0, earthRadius);
    modelMatrix *= transform3D::Scale(0.5, 0.5, 0.5);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    // luna
    modelMatrix = glm::mat4(1);
    moonAngle += moonAngStep;
    modelMatrix *= transform3D::Translate(earthX, 0, earthZ);
    modelMatrix *= transform3D::RotateOY(moonAngle);
    modelMatrix *= transform3D::Translate(0, 0, moonRadius);
    modelMatrix *= transform3D::Scale(0.1, 0.1, 0.1);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);


}

void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    RenderScene(deltaTimeSeconds);
    DrawCoordinateSystem();

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    // TODO(student): render the scene again, in the new viewport
    RenderScene(deltaTimeSeconds);
    DrawCoordinateSystem();
}

void Lab4::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add transformation logic

    float dist = 5 * deltaTime;
    float scale = deltaTime;
    float angle = deltaTime;

    // primul cub
    if (window->KeyHold(GLFW_KEY_W)) {
        translateX += dist;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        translateX -= dist;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        translateZ -= dist;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        translateZ += dist;
    }

    if (window->KeyHold(GLFW_KEY_R)) {
        translateY += dist;
    }
    if (window->KeyHold(GLFW_KEY_F)) {
        translateY -= dist;
    }


    // al doilea cub
    if (window->KeyHold(GLFW_KEY_1)) {
        scaleX += scale;
        scaleY += scale;
        scaleZ += scale;
    }

    if (window->KeyHold(GLFW_KEY_2)) {
        scaleX -= scale;
        scaleY -= scale;
        scaleZ -= scale;
    }

    // al treilea cub

    // OX
    if (window->KeyHold(GLFW_KEY_3)) {
        angularStepOX += angle;
    }

    if (window->KeyHold(GLFW_KEY_4)) {
        angularStepOX -= angle;
    }

    // OY
    if (window->KeyHold(GLFW_KEY_5)) {
        angularStepOY += angle;
    }

    if (window->KeyHold(GLFW_KEY_6)) {
        angularStepOY -= angle;
    }

    // OZ
    if (window->KeyHold(GLFW_KEY_7)) {
        angularStepOZ += angle;
    }

    if (window->KeyHold(GLFW_KEY_8)) {
        angularStepOZ -= angle;
    }






}


void Lab4::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
    
    // TODO(student): Add viewport movement and scaling logic
    float scale = 1.2;
    int step = 30;

    // viewport mare
    if (key == GLFW_KEY_U) {
        miniViewportArea.height *= scale;
        miniViewportArea.width  *= scale;
    }

    // viewport mic
    if (key == GLFW_KEY_O) {
        miniViewportArea.height /= scale;
        miniViewportArea.width  /= scale;
    }

    // mutare viewport
    if (key == GLFW_KEY_I) {
        miniViewportArea.y += step;
    }

    if (key == GLFW_KEY_J) {
        miniViewportArea.x -= step;
    }

    if (key == GLFW_KEY_K) {
        miniViewportArea.y -= step;
    }

    if (key == GLFW_KEY_L) {
        miniViewportArea.x += step;
    }


    
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
