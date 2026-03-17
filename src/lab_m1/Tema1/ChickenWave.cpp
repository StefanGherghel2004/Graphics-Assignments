#include "lab_m1/Tema1/ChickenWave.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace mesh;

float ChickenWave::baseChickenSpeed = 40;
float ChickenWave::baseNumRows = 2;
float ChickenWave::baseEggSpeed = 300;
float ChickenWave::baseEggPeriod = 14;
float ChickenWave::baseNumfullRows = 0;

ChickenWave::ChickenWave(float formationTime, bool initPos)
		   : formationTime(formationTime), formationEnd(false), formationTimer(0), movingDir(1), radians(0)
{
	// updating the parameters of the wave based on the static values that increase with each wave
	chickenSpeed = baseChickenSpeed;
	baseChickenSpeed += 2;

	eggSpeed = baseEggSpeed;
	baseEggSpeed += 10;

	eggPeriod = baseEggPeriod;
	baseEggPeriod -= 0.5;

	numRows = (int)baseNumRows;

	// generate full rows of chickens that appear after the pyramid is formed
	if (baseNumRows < 6) {
		baseNumRows += 0.5;
	}

	if (numRows >= 6) {
		numfullRows = (int) baseNumfullRows;
		baseNumfullRows += 0.5;
	}
	else {
		numfullRows = 0;
	}


	if (initPos)
		FormationInitPos();

}

void ChickenWave::WaveForming(float deltaTime)
{
	if (formationEnd) {
		return;
	}

	if (formationTimer == 0) {
		radians = 0;
	}

	formationTimer += deltaTime;

	// moving in formation
	for (auto& chicken : chickens) {
		chicken->UpdatePos(glm::vec2(0, - 1.2 * chickenSpeed * deltaTime));
	}

	radians += 0.06;

	if (formationTimer >= formationTime) {
		formationEnd = true;
		formationTimer = 0;
		radians = 0;
	}
}

ChickenWave::~ChickenWave()
{

}

void ChickenWave::FormationMove(float deltaTime)
{
	if (!formationEnd) {
		return;
	}

	if (formationTimer == 0) {
		radians = glm::pi<float>() / 2.0f;
	}

	// .8 seconds where the chickens stay still before starting the sinusoidal movement
	if (formationTimer < 0.8) {
		formationTimer += deltaTime;
		return;
	}

	for (auto& chicken : chickens) {
		chicken->UpdatePos(glm::vec2(5 * sinf(radians), -movingDir * chickenSpeed * deltaTime));
	}

	radians += 0.06;
}

void ChickenWave::FormationInitPos(float logicWidth, float logicHeight)
{
	int numCols = 8;

	float marginX = logicWidth * 0.15f;
	float marginY = logicHeight * 0.2f * (float) (numRows / 2.0f);

	float colSpacing = (logicWidth - 2 * marginX) / (float)(numCols - 1);
	float rowSpacing = (logicHeight * 0.08f);

	float startY = logicHeight + 1.15 * marginY;

	// the formation starts higher with each wave
	if (numRows >= 6) {
		startY += ((float) (numfullRows + 1) / 3) * 2.3 * rowSpacing;
	}

	float chickenWidth = 25.0f;

	// chicken pyramid
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols - row; col++) {
			float x = marginX + col * colSpacing + row * (colSpacing * 0.5f);
			float y = startY - row * rowSpacing;

			chickens.push_back(new Chicken(glm::vec2(x, y), chickenWidth, eggSpeed, eggPeriod, "egg"));
		}
	}

	// rows of 8 chickens distanced from the pyramid that appear when numRows >= 6
	if (numRows >= 6) {

		float offset = startY + (numfullRows + 1) * rowSpacing;

		for (int i = 0; i < numfullRows; i++) {
			for (int j = 0; j < numCols; j++) {
				float x = marginX + j * colSpacing;
				float y = offset + i * rowSpacing;
				chickens.push_back(new Chicken(glm::vec2(x, y), chickenWidth, eggSpeed, eggPeriod, "egg"));
			}
		}


	}
}

void ChickenWave::ResetBaseValues()
{
	// resetting the base values that increase with each wave to the initial values for the next game
	baseChickenSpeed = 40;
	baseNumRows = 2;
	baseEggSpeed = 300;
	baseEggPeriod = 14;
	baseNumfullRows = 0;
}

void ChickenWave::RemoveChicken(Chicken* c)
{
	chickens.erase(std::remove(chickens.begin(), chickens.end(), c), chickens.end());
}
