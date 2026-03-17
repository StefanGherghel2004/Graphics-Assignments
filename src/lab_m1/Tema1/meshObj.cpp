#include "meshObj.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include <math.h>
using namespace std;
#include <lab_m1/Tema1/colors.h>

Mesh* mesh::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    return CreateRectangle(name, leftBottomCorner,length, length, color, fill);
}

Mesh* mesh::CreateLine(
    const std::string& name,
    glm::vec3 point1,
    glm::vec3 point2,
    glm::vec3 color)
{

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(point1, color),
        VertexFormat(point2, color)
    };

    Mesh* line = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1};

    line->InitFromData(vertices, indices);
    line->SetDrawMode(GL_LINES);
    return line;
}

Mesh* mesh::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float lenx,
    float leny,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(lenx, 0, 0), color),
        VertexFormat(corner + glm::vec3(lenx, leny, 0), color),
        VertexFormat(corner + glm::vec3(0, leny, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}


// TEMA 2
Mesh* mesh::Create4PPoly(const std::string& name, VertexFormat p1, VertexFormat  p2, VertexFormat p3, VertexFormat p4, float gradient)
{
    glm::vec3 centerPos = (p1.position + p2.position + p3.position + p4.position) / 4.0f;

    glm::vec3 edgeColor = p1.color * gradient;

    glm::vec3 centerColor = p1.color;

    std::vector<VertexFormat> vertices = {
        VertexFormat(p1.position, edgeColor),
        VertexFormat(p2.position, edgeColor),
        VertexFormat(p3.position, edgeColor),
        VertexFormat(p4.position, edgeColor),
        VertexFormat(centerPos, centerColor)
    };

    std::vector<unsigned int> indices = {
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    Mesh* poly = new Mesh(name);
    poly->InitFromData(vertices, indices);
    return poly;
}



Mesh* mesh::CreateStartButton(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color),
        VertexFormat(corner + glm::vec3(length / 2, length / 2, 0), color)

    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 4, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // draw 3 triangles
        indices.push_back(4);
        indices.push_back(0);
        indices.push_back(4);
        indices.push_back(3);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* mesh::CreateSquareBlock(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    glm::vec3 centrColor
    )
{
    glm::vec3 corner = leftBottomCorner + glm::vec3(0, 0, 1);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color),
        VertexFormat(corner + glm::vec3(length / 2, length / 2, 0), centrColor)

    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 4, 2, 3 };

    // draw 4 triangles
    indices.push_back(4);
    indices.push_back(0);
    indices.push_back(4);
    indices.push_back(3);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(4);
    

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* mesh::CreateEngine(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    glm::vec3 centrColor,
    glm::vec3 fireCol
)
{
    glm::vec3 corner = leftBottomCorner + glm::vec3(0, 0, 1);
    
    Mesh* squareBlock = CreateSquareBlock(name, corner, length, color, centrColor);

    std::vector<VertexFormat> vertices =
    {
		// points for the fire, with a gradient color
        VertexFormat(corner + glm::vec3(0.05 * length, 0, 0), fireCol),
        VertexFormat(corner + glm::vec3(0.95 * length, 0, 0), fireCol),
        
        VertexFormat(corner + glm::vec3(0.05 * length, -length, 0), 0.7f * fireCol),
        VertexFormat(corner + glm::vec3(0.95 * length, -length, 0), 0.7f * fireCol),
        VertexFormat(corner + glm::vec3(0.33 * length, -1.05 * length, 0), 0.7f * fireCol),
        VertexFormat(corner + glm::vec3(0.66 * length, -0.95 * length, 0), 0.7f * fireCol),
    };

    Mesh* fire = new Mesh(name);
    std::vector<unsigned int> indices;

    for (int i = 2; i < 6; i++) {
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(i);
    }

    fire->InitFromData(vertices, indices);

    return MergeMeshes(std::vector<Mesh*>{squareBlock, fire}, name);
}

Mesh* mesh::CreateCannon(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 baseCol,
    glm::vec3 circleCol,
    glm::vec3 barrelCol
)
{
    glm::vec3 corner = leftBottomCorner + glm::vec3(0, 0, 1);
    glm::vec3 barrelCorner = corner + glm::vec3(0.1 * length, length / 2, 0);
    glm::vec3 circleCenter = corner + glm::vec3(length / 2, length / 2, 1);

    Mesh* halfSquare = CreateRectangle(name, corner, length, length / 2, baseCol, true);
    Mesh* barrel = CreateRectangle(name, barrelCorner, 0.8 * length, 2.5 * length, barrelCol, true);
    Mesh* halfCircle = CreateEllipsoide(name, circleCenter, length / 2, length / 2, circleCol, 0.8, true);

    return MergeMeshes(std::vector<Mesh*>{halfSquare, barrel, halfCircle}, name);
}

Mesh* mesh::CreateBumper(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 baseCol,
    glm::vec3 circleCol
)
{
    glm::vec3 corner = leftBottomCorner + glm::vec3(0, 0, 1);
    glm::vec3 center = corner + glm::vec3(length / 2, length, 1);

    Mesh* square = CreateSquare(name, corner, length, baseCol, true);
    Mesh* halfEllipse = CreateEllipsoide(name, center, 1.5 * length, length, circleCol, 0.8, true);

    return MergeMeshes(std::vector<Mesh*>{square, halfEllipse}, name);
}

Mesh* mesh::CreateHeart(const std::string& name, glm::vec3 center, float length, glm::vec3 color)
{
    std::vector<VertexFormat> vertices;


    Mesh* heart = new Mesh(name);
    std::vector<unsigned int> indices;


    vertices.push_back(VertexFormat(center, color));
    int centerIndex = vertices.size() - 1;

    int N = 30;
    for (float i = 0; i <= N; i++) {
        float angle = 2 * glm::pi<float>() * i / N;
        float x = center.x + length * (16 * pow(sinf(angle), 3));
        float y = center.y + length * ((13 * cosf(angle) - 5 * cosf(2 * angle) - 2 * cosf(3 * angle) - cos(4 * angle)));
        vertices.push_back(VertexFormat(glm::vec3(x, y, center.z), color));
    }

    for (int i = 0; i < N; i++) {
        indices.push_back(centerIndex);
        indices.push_back(centerIndex + i + 1);
        indices.push_back(centerIndex + i + 2);
    }

    heart->InitFromData(vertices, indices);
    return heart;
}

Mesh* mesh::CreateEllipsoide(const std::string& name, glm::vec3 center, float radiusX, float radiusY, glm::vec3 color, float gradient, bool half, float zCenterOff)
{

    std::vector<VertexFormat> vertices;

    vertices.push_back(VertexFormat(center, color));
    int centerIndex = vertices.size() - 1;
    float fullAngle = half ? glm::pi<float>() : 2 * glm::pi<float>();

    int N = 16;

    for (float i = 0; i <= N; i++) {
        float angle = fullAngle * i / N;
        float x = center.x + radiusX * cosf(angle);
        float y = center.y + radiusY * sinf(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, y, center.z + zCenterOff), color * gradient));
    }

    Mesh* ellipsoide = new Mesh(name);
    std::vector<unsigned int> indices;

    for (int i = 0; i < N; i++) {
        indices.push_back(centerIndex);
        indices.push_back(centerIndex + i + 1);
        indices.push_back(centerIndex + i + 2);
    }

    ellipsoide->InitFromData(vertices, indices);
    return ellipsoide;
}

Mesh* mesh::CreateTriangle(const std::string& name, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color, float gradient)
{

    std::vector<VertexFormat> vertices = {VertexFormat(p1, color), VertexFormat(p2, color), VertexFormat(p3, color * (1.0f / gradient))};

    Mesh* meshTr = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    meshTr->InitFromData(vertices, indices);
    return meshTr;
}



bool mesh::CheckEllipseEllipseCollision(glm::vec2 ellipse1Pos, glm::vec2 ellipse1Size, glm::vec2 ellipse2Pos, glm::vec2 ellipse2Size, bool half)
{
	// coordinates of the center of ellipse 1
    float e1x = ellipse1Pos.x;
    float e1y = ellipse1Pos.y;
    float r1x = ellipse1Size.x;
    float r1y = ellipse1Size.y;

    // coordinates of the center of ellipse 2
    float e2x = ellipse2Pos.x;
    float e2y = ellipse2Pos.y;
    float r2x = ellipse2Size.x;
    float r2y = ellipse2Size.y;

	// finding the closest point on ellipse 2 to the center of ellipse 1
    float closestX = glm::clamp(e1x, e2x - r2x, e2x + r2x);
    float closestY = glm::clamp(e1y, e2y - r2y, e2y + r2y);

	// compute the normalized distance between the center of ellipse 1 and that point
    float dx = (closestX - e1x) / r1x;
    float dy = (closestY - e1y) / r1y;

    // boolean condition used for half ellipses
    bool halfCond = half ? closestY > e2y : true;

	// if the sum of squares is <= 1, the point is inside the ellipse
    return ((dx * dx + dy * dy) <= 1.0f) && halfCond;
}


bool mesh::CheckEllipseRectCollision(glm::vec2 ellipsePos, glm::vec2 ellipseSize, glm::vec2 rectanglePos, glm::vec2 rectangleSize)
{
    // coordinates of the center of ellipse
    float ex = ellipsePos.x;
    float ey = ellipsePos.y;
    float rx = ellipseSize.x;
    float ry = ellipseSize.y;

    // coordinates of the rectangle
    float recx = rectanglePos.x;
    float recy = rectanglePos.y;
    float recw = rectangleSize.x;
    float rech = rectangleSize.y;

	// finding the closest point on the rectangle to the center of the ellipse
    float closestX = glm::clamp(ex, recx, recx + recw);
    float closestY = glm::clamp(ey, recy, recy + rech);

	// computeing the normalized distance between the center of the ellipse and that point
    float dx = (closestX - ex) / rx;
    float dy = (closestY - ey) / ry;

	// if the sum of squares is <= 1, the point is inside the ellipse
    return (dx * dx + dy * dy) <= 1.0f;
}


Mesh* mesh::MergeMeshes(std::vector<Mesh*>& meshes, std::string meshName)
{
    if (meshes.empty()) {
        return nullptr;
    }

    std::vector<VertexFormat> combinedVertices;
    std::vector<unsigned int> combinedIndices;
    int offset = 0;

    for (auto& m : meshes) {

        const std::vector<VertexFormat>& verts = m->vertices;
        const std::vector<unsigned int>& inds = m->indices;

        combinedVertices.insert(combinedVertices.end(), verts.begin(), verts.end());

        for (auto idx : inds) {
            combinedIndices.push_back(idx + offset);
        }

        offset += verts.size();
    }

    Mesh* merged = new Mesh(meshName);
    merged->InitFromData(combinedVertices, combinedIndices);

    return merged;
}


// TEMA 2
Mesh* mesh::CreateCylinder(const std::string& name, glm::vec3 center, float radius, float heigth, glm::vec3 color, float gradient, bool pointy)
{
    
    std::vector<Mesh*> meshes;

    
    Mesh* meshCircle1 = CreateEllipsoide(name, center - glm::vec3(0, 0, heigth / 2.0f), radius, radius, color, gradient, false, 0);
    meshes.push_back(meshCircle1);

    Mesh* meshCircle2;
    if (pointy) {
        meshCircle2 =  CreateEllipsoide(name, center + glm::vec3(0, 0, heigth / 2.0f), radius, radius, color, gradient);
    }
    else {
        meshCircle2 = CreateEllipsoide(name, center + glm::vec3(0, 0, heigth / 2.0f), radius, radius, color, gradient, false, 0);
    }

    meshes.push_back(meshCircle2);
    

    for (int i = 1; i < meshCircle1->vertices.size() - 1; i++) {
        VertexFormat p1 = meshCircle1->vertices[i];
        VertexFormat p2 = meshCircle1->vertices[i + 1];

        VertexFormat p3 = meshCircle2->vertices[i + 1];
        VertexFormat p4 = meshCircle2->vertices[i];

        meshes.push_back(Create4PPoly(name, p1, p2, p3, p4));
    }

    return MergeMeshes(meshes, name);

}

// TEMA 2
Mesh* mesh::CreateBox(const std::string& name, glm::vec3 center, float lenX, float lenY, float lenZ,  glm::vec3 color, float gradient)
{

    std::vector<Mesh*> meshes;

    float x = lenX / 2.0f;
    float y = lenY / 2.0f;
    float z = lenZ / 2.0f;

    VertexFormat p1 = { center + glm::vec3(-x, -y, z), color };
    VertexFormat p2 = { center + glm::vec3(x, -y, z), color };
    VertexFormat p3 = { center + glm::vec3(x, -y, -z), color };
    VertexFormat p4 = { center + glm::vec3(-x, -y, -z), color };
    VertexFormat p5 = { center + glm::vec3(-x, y, z), color };
    VertexFormat p6 = { center + glm::vec3(x, y, z), color };
    VertexFormat p7 = { center + glm::vec3(x, y, -z), color };
    VertexFormat p8 = { center + glm::vec3(-x, y, -z), color };

    meshes.push_back(Create4PPoly(name, p1, p2, p3, p4, gradient));
    meshes.push_back(Create4PPoly(name, p5, p6, p7, p8, gradient));
    meshes.push_back(Create4PPoly(name, p1, p2, p6, p5, gradient));
    meshes.push_back(Create4PPoly(name, p4, p3, p7, p8, gradient));
    meshes.push_back(Create4PPoly(name, p2, p3, p7, p6, gradient));
    meshes.push_back(Create4PPoly(name, p1, p4, p8, p5, gradient));

    return MergeMeshes(meshes, name);

}



Mesh* mesh::CreateChickenHead(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, glm::vec3 eyesColor, glm::vec3 combCol, glm::vec3 beakCol)
{

    Mesh* baseMesh = CreateEllipsoide(name, center, radius, radius, color, 0.8);

    Mesh* eye1Mesh = CreateEllipsoide(name, center + glm::vec3(radius / 3.5, radius / 3.5, 1), 0.15 * radius, 0.2 * radius, eyesColor,  1);
    Mesh* eye2Mesh = CreateEllipsoide(name, center + glm::vec3(-radius / 3.5, radius / 3.5, 1), 0.15 * radius, 0.2 * radius, eyesColor, 1);

    Mesh* comb1Mesh = CreateTriangle(name, center + glm::vec3(-radius / 3, 0.8 * radius, 1), glm::vec3(radius / 3, 0.8 * radius, 1), glm::vec3(radius / 4, 1.5 * radius, 1), combCol);
    Mesh* comb2Mesh = CreateTriangle(name, center + glm::vec3(-radius / 3, 0.8 * radius, 1), glm::vec3(radius / 3, 0.8 * radius, 1), glm::vec3(radius / 1.5, 1.6 * radius, 1), combCol);

    Mesh* beakMesh = CreateTriangle(name, center + glm::vec3(0, 0, 1), center + glm::vec3(-radius / 5, -radius / 3, 1), center + glm::vec3(0.35 * radius, -0.6 * radius, 1), beakCol);
    return MergeMeshes(std::vector<Mesh*>{ baseMesh, eye1Mesh, eye2Mesh, comb1Mesh, comb2Mesh, beakMesh}, name);
}


bool mesh::IsInsideRectangle(glm::vec2 pos, glm::vec2 recpos, glm::vec2 size)
{
   return pos.x >= recpos.x && pos.x <= recpos.x + size.x && pos.y >= recpos.y && pos.y <= recpos.y + size.y;
}


Mesh* mesh::CreateExplosion(const std::string& name, glm::vec3 center, float radius, glm::vec3 color1, glm::vec3 color2)
{
    Mesh* mesh1 = CreateStar(name, center, radius, color1);
    Mesh* mesh2 = CreateStar(name, center + glm::vec3(0, 0, 1), radius, color2, -glm::pi<float>() / 2);

    return MergeMeshes(std::vector<Mesh*>{mesh1, mesh2}, name);
}

Mesh* mesh::CreateStar(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    float offset)
{
    std::vector<Mesh*> triangles;

    const int N = 5;
    float innerRadius = radius * 0.45f;
    float outerRadius = radius;

    std::vector<glm::vec3> starPoints;

    for (int i = 0; i < N; i++) {
        float angleOuter = offset + (2 * glm::pi<float>() * i / N);
        float angleInner = angleOuter + glm::pi<float>() / N;

        starPoints.push_back(center + glm::vec3(
            outerRadius * cos(angleOuter),
            outerRadius * sin(angleOuter),
            center.z
        ));

        starPoints.push_back(center + glm::vec3(
            innerRadius * cos(angleInner),
            innerRadius * sin(angleInner),
            center.z
        ));
    }

    for (int i = 0; i < N; i++) {
        int outerIndex = 2 * i;
        int innerIndex = 2 * i + 1;
        int nextOuterIndex = (2 * ((i + 1) % N));


        triangles.push_back(CreateTriangle(name,
            center,
            starPoints[outerIndex],
            starPoints[innerIndex],
            color
        ));


        triangles.push_back(CreateTriangle(name,
            center,
            starPoints[innerIndex],
            starPoints[nextOuterIndex],
            color
        ));
    }

    return MergeMeshes(triangles, name);
}


Mesh* mesh::CreateStarBackground(const std::string& name, glm::vec3 corner, float radius, glm::vec3 color, glm::vec3 backColor, float width, float Heigth)
{
    std::vector<Mesh*> meshes;

    Mesh* background = CreateRectangle(name, corner, width, Heigth, backColor, true);
    meshes.push_back(background);

    int N = width / (48 * radius);
    int M = Heigth / (48 * radius);

    float gridW = (N - 1) * 48 * radius;
    float gridH = (M - 1) * 48 * radius;

    glm::vec3 startPos = corner + glm::vec3((width - gridW) / 2.0f, (Heigth - gridH) / 2.0f, 101);

    srand((unsigned)time(NULL));

    // generating stars with a random offset
    for (int j = 0; j < M; j++) {
        for (int i = 0; i < N; i++) {

            if (rand() % 4 == 0)
                continue;

            glm::vec3 pos = startPos + glm::vec3(i * 48 * radius, j * 48 * radius, 0);
            float sign = (rand() % 2) ? -1 : 1;

            pos.x += sign * (rand() % 20 + 2);
            pos.y += sign * (rand() % 20 + 2);

            Mesh* star = CreateStar(name, pos, radius, color);
            meshes.push_back(star);
        }
    }

    return MergeMeshes(meshes, name);
}


Mesh* mesh::CreateChickenLeg(const std::string& name, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color)
{
    Mesh* mesh1 = CreateTriangle(name, p1, p2, p3, color);

    float dist = glm::distance(p1, p2);

    Mesh *mesh2 = CreateRectangle(name, p1 + glm::vec3(0, - dist/ 6, 0), dist / 2.7, dist / 2, color, true);

    return MergeMeshes(std::vector<Mesh*>{mesh1, mesh2}, name);
}


// TEMA 2 
Mesh* mesh::CreateTrack(const std::string& name, glm::vec3 center, float lenX, float lenY, float lenZ, glm::vec3 color1, glm::vec3 color2, glm::vec3 color3, float gradient, string type)
{

    vector<Mesh*> meshes;

     Mesh* track1 = CreateBox("track", center - glm::vec3(lenZ / 3.5f, 0, 0), lenX, lenY, lenZ, color1, gradient);
     Mesh* track2 = CreateBox("track", center + glm::vec3(lenZ / 3.5f, 0, 0), lenX, lenY, lenZ, color1, gradient);
     meshes.push_back(track1);
     meshes.push_back(track2);


    if (type == "water") {
        Mesh* track1 = CreateBox("track", center - glm::vec3(0, 0.2f * lenY, 0), lenX, 0.8f * lenY, lenZ, color3, gradient);
        Mesh* track2 = CreateBox("track", center - glm::vec3(lenZ / 6.0f, 0.25f * lenY, 0), lenZ / 3.0f - lenX, lenY * 0.75f, lenZ, color2, gradient);
        Mesh* track3 = CreateBox("track", center + glm::vec3(lenZ / 6.0f, -0.25f * lenY, 0), lenZ / 3.0f - lenX, lenY * 0.75f, lenZ, color2, gradient);
        Mesh* track4 = CreateBox("track", center - glm::vec3(0.85 * (lenZ / 2.0f - (lenZ / 3.0f - lenX) / 3.0f), 0.25f * lenY, 0), (lenZ / 3.0f - lenX) / 3.0f, lenY * 0.75f, lenZ, color3, gradient);
        Mesh* track5 = CreateBox("track", center + glm::vec3(0.85 * (lenZ / 2.0f - (lenZ / 3.0f - lenX) / 3.0f), - 0.25f * lenY, 0), (lenZ / 3.0f - lenX) / 3.0f, lenY * 0.75f, lenZ, color3, gradient);


        meshes.push_back(track1);
        meshes.push_back(track2);
        meshes.push_back(track3);
        meshes.push_back(track4);
        meshes.push_back(track5);
    }

    if (type == "tunnel") {
        Mesh* track1 = CreateBox("track", center - glm::vec3(0, 0.25f * lenY, lenZ / 4), 2 * lenZ / 3.0f - lenX, 0.75f * lenY, lenX / 2.0f, color3, gradient);
        Mesh* track2 = CreateBox("track", center + glm::vec3(0, -0.25f * lenY, lenZ / 4), 2 * lenZ / 3.0f - lenX, 0.75f * lenY, lenX / 2.0f, color3, gradient);
        Mesh* track3 = CreateBox("track", center + glm::vec3(0, -0.5f * lenY, 0), 2 * lenZ / 3.0f - lenX, 0.5f * lenY, lenZ, color2, gradient);

        meshes.push_back(track1);
        meshes.push_back(track2);
        meshes.push_back(track3);
    }

    return MergeMeshes(meshes, name);

}
// basically a method that generates 5 possible types of stations based on an input of 3 initial colors
// for type 0, the central station is generated that contains these colors
// for the rest, a station is generated that has some of these colors as accents but is in a different theme (in this case each one is a green, red, blue, yellow/orange color)
// calling in a loop with the same parameters (the type being incremented) a set of central station + 4 normal stations is obtained
Mesh* mesh::CreateTrainStation(const std::string& name, glm::vec3 center, float lenX, float lenY, float lenZ, glm::vec3 color1, glm::vec3 color2, glm::vec3 color3, float gradient, int type)
{

    vector<Mesh*> meshes;
    float off = 0.05f * lenY;


    switch (type) {

        case 0:

            meshes.push_back(CreateBox("center", center + glm::vec3(0, - lenY * 0.1f, 0), 0.6f * lenX, lenY * 0.8, lenZ * 0.8, color1, gradient));
            meshes.push_back(CreatePyramid(name, center + glm::vec3(0, lenY * 0.3f, 0), 0.65f * lenX, lenZ * 0.85, 0.25 * lenY, color2, color2, gradient));
            meshes.push_back(CreateBox(name, center + glm::vec3(0, lenY * 0.3f - off * 0.5f, 0), 0.65f * lenX, off, 0.85f * lenZ, color3, gradient));

            meshes.push_back(CreateBox(name, center + glm::vec3(0.4f * lenX, 0, lenZ * 0.5f - lenX * 0.1f), 0.2f * lenX, lenY, 0.2f * lenX, color1, gradient));
            meshes.push_back(CreateBox(name, center - glm::vec3(0.4f * lenX, 0, lenZ * 0.5f - lenX * 0.1f), 0.2f * lenX, lenY, 0.2f * lenX, color1, gradient));
            meshes.push_back(CreateBox(name, center + glm::vec3(0.4f * lenX, 0, -lenZ * 0.5f + lenX * 0.1f), 0.2f * lenX, lenY, 0.2f * lenX, color1, gradient));
            meshes.push_back(CreateBox(name, center - glm::vec3(0.4f * lenX, 0, -lenZ * 0.5f + lenX * 0.1f), 0.2f * lenX, lenY, 0.2f * lenX, color1, gradient));

            meshes.push_back(CreatePyramid(name, center + glm::vec3(0.4f * lenX, off + lenY - 0.5 * lenY, lenZ * 0.5f - lenX * 0.1f), 0.25f * lenX, 0.25f * lenX, 0.2 * lenY, color2, color2, gradient));
            meshes.push_back(CreatePyramid(name, center - glm::vec3(0.4f * lenX, -off - lenY + 0.5 * lenY, lenZ * 0.5f - lenX * 0.1f), 0.25f * lenX, 0.25f * lenX, 0.2 * lenY, color2, color2, gradient));
            meshes.push_back(CreatePyramid(name, center + glm::vec3(0.4f * lenX, off + lenY - 0.5 * lenY, -lenZ * 0.5f + lenX * 0.1f), 0.25f * lenX, 0.25f * lenX, 0.2 * lenY, color2, color2, gradient));
            meshes.push_back(CreatePyramid(name, center - glm::vec3(0.4f * lenX, -off -lenY + 0.5 * lenY, -lenZ * 0.5f + lenX * 0.1f), 0.25f * lenX, 0.25f * lenX, 0.2 * lenY, color2, color2, gradient));

            meshes.push_back(CreateBox(name, center + glm::vec3(0.4f * lenX, off * 0.5f + lenY - 0.5 * lenY, lenZ * 0.5f - lenX * 0.1f), 0.25f * lenX, off, 0.25f * lenX, color3, gradient));
            meshes.push_back(CreateBox(name, center - glm::vec3(0.4f * lenX, -off * 0.5f -lenY + 0.5 * lenY, lenZ * 0.5f - lenX * 0.1f), 0.25f * lenX, off, 0.25f * lenX, color3, gradient));
            meshes.push_back(CreateBox(name, center + glm::vec3(0.4f * lenX, off * 0.5f + lenY - 0.5 * lenY, -lenZ * 0.5f + lenX * 0.1f), 0.25f * lenX, off, 0.25f * lenX, color3, gradient));
            meshes.push_back(CreateBox(name, center - glm::vec3(0.4f * lenX, -off * 0.5f - lenY + 0.5 * lenY, -lenZ * 0.5f + lenX * 0.1f), 0.25f * lenX, off, 0.25f * lenX, color3, gradient));

            meshes.push_back(CreateBox(name, center + glm::vec3(0, - lenY * 0.2f, lenZ * 0.5f - lenX * 0.1f), 0.4f * lenX, 0.6 * lenY, 0.2f * lenX, color1, gradient));
            meshes.push_back(CreatePyramid(name, center + glm::vec3(0, lenY * 0.1f, lenZ * 0.4f - lenX * 0.1f), 0.45f * lenX, 0.40f * lenX, 0.2 * lenY, color2, color2, gradient));
            meshes.push_back(CreateBox(name, center + glm::vec3(0, lenY * 0.1f - off * 0.5f, lenZ * 0.4f - lenX * 0.1f), 0.45f * lenX, off, 0.40f * lenX, color3, gradient));

 
            break;
    
        case 1:

            meshes.push_back(CreateBox("center", center + glm::vec3(0, -lenY * 0.3f, 0), 0.3f * lenX, lenY * 0.4, lenZ * 0.4, GREEN05, gradient));
            meshes.push_back(CreateBox("center", center + glm::vec3(0, -lenY * 0.475f, 0), 0.4f * lenX, lenY * 0.05, lenZ * 0.5, DARKER_BROWN, 0.8));

            meshes.push_back(CreatePyramid(name, center + glm::vec3(0, -lenY * 0.1f, 0), 0.4f * lenX, lenZ * 0.53, 0.25 * lenY, DARKER_BROWN, DARKER_BROWN, gradient));

            meshes.push_back(CreateBox(name, center + glm::vec3(4 * off, -lenY * 0.15f, - off), 0.05f * lenX, lenY * 0.7, 0.05f * lenX, color1, gradient));
            break;

        case 2:
            meshes.push_back(CreateCylinder("center", center + glm::vec3(0, -lenY * 0.5f, 0), lenY * 0.6f, lenZ * 0.65, ORANGE2, 0.6, false));

            meshes.push_back(CreateBox("center", center + glm::vec3(0, -lenY * 0.1f, -0.18 * lenZ), 0.15f * lenX, lenY * 0.8, lenZ * 0.2, BROWN, gradient));
            meshes.push_back(CreateBox("center", center + glm::vec3(0, -lenY * 0.1f, 0.18 * lenZ), 0.15f * lenX, lenY * 0.8, lenZ * 0.2, BROWN, gradient));
            meshes.push_back(CreatePyramid(name, center + glm::vec3(0, lenY * 0.3f, -0.18 * lenZ), 0.18f * lenX, 0.18f * lenX, 0.2 * lenY, color3, color3, gradient));
            meshes.push_back(CreatePyramid(name, center + glm::vec3(0, lenY * 0.3f, 0.18 * lenZ), 0.18f * lenX, 0.18f * lenX, 0.2 * lenY, color3, color3, gradient));

            break;

        case 3:
            meshes.push_back(CreateBox("center", center + glm::vec3(0, -lenY * 0.1f, 0), 0.3f * lenX, lenY * 0.8, lenZ * 0.6, RED09, 0.4));
            meshes.push_back(CreateBox("center", center + glm::vec3(0, lenY * 0.325f, 0), 0.35f * lenX, lenY * 0.05, lenZ * 0.65, color1, gradient));

            meshes.push_back(CreateBox("center", center + glm::vec3(- 0.2 * lenX, -lenY * 0.25f, 0), 0.3f * lenX, lenY * 0.5, lenZ * 0.4, RED09, 0.6));
            meshes.push_back(CreateBox("center", center + glm::vec3(0.2 * lenX, -lenY * 0.25f, 0), 0.3f * lenX, lenY * 0.5, lenZ * 0.4, RED09, 0.6));
            meshes.push_back(CreateBox("center", center + glm::vec3(-0.2 * lenX, lenY * 0.025f, 0), 0.35f * lenX, lenY * 0.05, lenZ * 0.45, color1, 0.4));
            meshes.push_back(CreateBox("center", center + glm::vec3(0.2 * lenX, lenY * 0.025f, 0), 0.35f * lenX, lenY * 0.05, lenZ * 0.45, color1, 0.4));
            

            break;

        case 4:
            meshes.push_back(CreatePyramid(name, center + glm::vec3(0, -lenY * 0.5f, 0), 0.5f * lenX, 0.5f * lenX, 0.7 * lenY, BLUE05, BLUE05, gradient));
            meshes.push_back(CreateBox("center", center + glm::vec3(0, -lenY * 0.1f, 0), 0.3f * lenX, lenY * 0.8, lenX * 0.3, CYAN, gradient));


            meshes.push_back(CreatePyramid(name, center + glm::vec3(0, lenY * 0.3f, 0), 0.35f * lenX, 0.35f * lenX, 0.3 * lenY, color1, color2, gradient));

            meshes.push_back(CreateBox(name, center + glm::vec3(0.28f * lenX, 0, lenZ * 0.5f - lenX * 0.02f), 0.05f * lenX, lenY, 0.05f * lenX, color3, gradient));
            meshes.push_back(CreateBox(name, center - glm::vec3(0.28f * lenX, 0, lenZ * 0.5f - lenX * 0.02f), 0.05f * lenX, lenY, 0.05f * lenX, color3, gradient));
            meshes.push_back(CreateBox(name, center + glm::vec3(0.28f * lenX, 0, -lenZ * 0.5f + lenX * 0.02f), 0.05f * lenX, lenY, 0.05f * lenX, color3, gradient));
            meshes.push_back(CreateBox(name, center - glm::vec3(0.28f * lenX, 0, -lenZ * 0.5f + lenX * 0.02f), 0.05f * lenX, lenY, 0.05f * lenX, color3, gradient));

            break;



        default:

            meshes.push_back(CreateBox(name, center, lenX, lenY, lenZ, color1, gradient));
            break;

 
    }
    

    return MergeMeshes(meshes, name);

}

// TEMA 2
Mesh* mesh::CreatePyramid(
    const std::string& name,
    glm::vec3 center, // base center
    float baseLenX,
    float baseLenZ,
    float height,
    glm::vec3 baseColor,
    glm::vec3 sideColor,
    float gradient)
{
    std::vector<Mesh*> meshes;

    float halfX = baseLenX / 2.0f;
    float halfZ = baseLenZ / 2.0f;

    VertexFormat p1 = { center + glm::vec3(-halfX, 0,  halfZ), baseColor };
    VertexFormat p2 = { center + glm::vec3(halfX, 0,  halfZ), baseColor };
    VertexFormat p3 = { center + glm::vec3(halfX, 0, -halfZ), baseColor };
    VertexFormat p4 = { center + glm::vec3(-halfX, 0, -halfZ), baseColor };

    auto top = center + glm::vec3(0, height, 0);

    meshes.push_back(Create4PPoly(name, p1, p2, p3, p4, gradient));

    meshes.push_back(CreateTriangle(name, p2.position, p1.position, top, sideColor, gradient));
    meshes.push_back(CreateTriangle(name, p3.position, p2.position, top, sideColor, gradient));
    meshes.push_back(CreateTriangle(name, p4.position, p3.position, top, sideColor, gradient));
    meshes.push_back(CreateTriangle(name, p1.position, p4.position, top, sideColor, gradient));

    return MergeMeshes(meshes, name);
}

// TEMA2
Mesh* mesh::CreateSymbol(
    const std::string& name,
    glm::vec3 center,
    float size,
    glm::vec3 color,
    float gradient,
    int type)
{
    Mesh* mesh;


    if (type == 1) {
        float h = sqrt(3.0f) * size / 2.0f;

        glm::vec3 top = center + glm::vec3(0, 2.0f * h / 3.0f, 0);
        glm::vec3 left = center + glm::vec3(-size / 2.0f, -h / 3.0f, 0);
        glm::vec3 right = center + glm::vec3(size / 2.0f, -h / 3.0f, 0);

        mesh = CreateTriangle(name, left, right, top, color, gradient);
    }
    else if (type == 2) {
        mesh = CreateEllipsoide(name, center, size / 2.0f, size / 2.0f, color, gradient, false, 0);
    }
    else if (type == 3) {
        float w = size / 2.0f;

        VertexFormat topleft = { center + glm::vec3(-w, w, 0), color };
        VertexFormat topright = { center + glm::vec3(w, w, 0), color };
        VertexFormat bottomleft = { center + glm::vec3(-w, -w, 0), color };
        VertexFormat bottomright = { center + glm::vec3(w, -w, 0), color };

        mesh = Create4PPoly(name, bottomleft, bottomright, topright, topleft, gradient);
    }
    else if (type == 4) {
        
        float w = sqrt(2.0f) * size / 2.0f;

        VertexFormat top = { center + glm::vec3(0, w, 0), color };
        VertexFormat right = { center + glm::vec3(w, 0, 0), color };
        VertexFormat left = { center + glm::vec3(-w, 0, 0), color };
        VertexFormat bottom = { center + glm::vec3(0, -w, 0), color };

        mesh = Create4PPoly(name, bottom, right, top, left, gradient);
    }



    return mesh;
}


