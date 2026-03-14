#include "GameMenu.h"
#include <lab_m1/Tema1/colors.h>
#include <fstream>

GameMenu::GameMenu()
{
	Reset();
}


void GameMenu::RenderCenteredText(TextRenderer* renderer, float charWidth, string text, glm::vec2 pos, glm::vec3 color)
{
	float off = (float)text.length() / 2.0f;

	renderer->RenderText(text, pos.x - off * charWidth, pos.y, 1.0f, color);
}


void GameMenu::Render(TextRenderer* renderer, glm::ivec2 resolution, int score, float time)
{
	if (!active) {
		return;
	}

	
	if (score > allTimehighscore) {
		allTimehighscore = score;
		std::ofstream fout("score.txt");
		fout << allTimehighscore;
		fout.close();
		newHigh = true;

	}

	int fontSize;

	int x = resolution.x;
	int y = resolution.y;

	//  se stabileste o valoarea a fontului pe baza marimii ecranului (in loc de folosirea unui factor de scalare)
	if (x < 400 || y < 225) {
		fontSize = 12;
	}
	else if (x < 600 || y < 337) {
		fontSize = 24;
	}
	else if (x < 900 || y < 506) {
		fontSize = 36;
	}
	else if (x < 1400 || y < 787) {
		fontSize = 48;
	}
	else if (x < 1800 || y < 1012) {
		fontSize = 60;
	}
	else {
		fontSize = 72;
	}

	renderer->Load("assets/fonts/Hack-Bold.ttf", fontSize);

	float charWidth = 0.6f * fontSize;

	if (newHigh) {
		RenderCenteredText(renderer, charWidth, "CONGRATULATIONS!", glm::vec2(x * 0.5f, 0.1f * y), RED);
		RenderCenteredText(renderer, charWidth, "Score: " + std::to_string(allTimehighscore), glm::vec2(x * 0.25f, 0.25f * y), RED);
	}
	else {
		RenderCenteredText(renderer, charWidth, "All-time high score: " + std::to_string(allTimehighscore), glm::vec2(x * 0.5f, 0.1f * y), WHITE);
	}


	RenderCenteredText(renderer, charWidth, "Time: " + std::to_string((int) time) + "s", glm::vec2(x * 0.75, 0.25f * y), WHITE);

	if (!newHigh) {
		RenderCenteredText(renderer, charWidth, "Score: " + std::to_string(score), glm::vec2(x * 0.25f, 0.25f * y), WHITE);
	}
	RenderCenteredText(renderer, charWidth, "END GAME", glm::vec2(x / 2.0f, y / 2.0f), WHITE);
	RenderCenteredText(renderer, charWidth, "Press SPACE for restart", glm::vec2(x / 2.0f, 0.75f * y), WHITE);
}


void GameMenu::Reset()
{
	std::ifstream fin("score.txt");

	if (fin.is_open()) {
		fin >> allTimehighscore;
		fin.close();
	}

	newHigh = false;
}

GameMenu::~GameMenu()
{
}
