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
	// actualizarea valorilor la fiecare wave nou creat
	chickenSpeed = baseChickenSpeed;
	baseChickenSpeed += 2;

	eggSpeed = baseEggSpeed;
	baseEggSpeed += 10;

	eggPeriod = baseEggPeriod;
	baseEggPeriod -= 0.5;

	numRows = (int)baseNumRows;


	// functionalitate folosita pentru a genera coloane intregi de pui dupa formarea piramidei 
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

	// miscarea pana in pozitia de inceput
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

	// 0.8 secunde in care stau pe loc gainile inainte sa si inceapa miscarea sinusoidala
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

	// formatia sa inceapa mai de sus cu trecerea valurilor
	if (numRows >= 6) {
		startY += ((float) (numfullRows + 1) / 3) * 2.3 * rowSpacing;
	}

	float chickenWidth = 25.0f;

	// piramida de gaini care creste de la val la val
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols - row; col++) {
			float x = marginX + col * colSpacing + row * (colSpacing * 0.5f);
			float y = startY - row * rowSpacing;

			chickens.push_back(new Chicken(glm::vec2(x, y), chickenWidth, eggSpeed, eggPeriod, "egg"));
		}
	}

	// randuri de gate 8 gaini distantate de piramida care apar cand numRows >= 6

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
	// resetarea valorilor initiale
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
