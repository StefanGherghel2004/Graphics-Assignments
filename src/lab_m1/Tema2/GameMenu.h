#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <iostream>
#include "lab_m1/Tema1/meshObj.h"
#include <components/text_renderer.h>

using namespace gfxc;
using namespace mesh;
using namespace std;


class GameMenu
{
public:

    GameMenu();

    // rendering text taking into account the character width on the x axis for centering
    static void RenderCenteredText(TextRenderer* renderer, float charWidth, string text, glm::vec2 pos, glm::vec3 color);

    // called when the menu is active
    void Render(TextRenderer* renderer, glm::ivec2 resolution, int score, float time);

	// used at the start of a new game to reset the menu state and update the high score from the file
    void Reset();

    ~GameMenu();

    void setActive() { active = true;  };
    void setInactive() { active = false; };
    bool IsActive() { return active; };

private:

protected:
    
	// the menu is unique for the game so it should be set active/inactive as needed
    bool active = false;

	// variable that determines if a new high score was achieved in the round that just ended
    bool newHigh = false;

	// at reset, the high score is updated from "score.txt"
    int allTimehighscore = 0;


};
