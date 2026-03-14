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

    // randeaza text pe ecran tinand cont de marimea caracterelor pe axa ox pentru a centra la pozitia data
    static void RenderCenteredText(TextRenderer* renderer, float charWidth, string text, glm::vec2 pos, glm::vec3 color);

    // apelat atunci cand meniul este activ
    void Render(TextRenderer* renderer, glm::ivec2 resolution, int score, float time);

    // la inceputul unui nou joc este folosit pentru a prelua din nou high score si a reseta flag-ul newHigh
    void Reset();

    ~GameMenu();

    void setActive() { active = true;  };
    void setInactive() { active = false; };
    bool IsActive() { return active; };

private:

protected:
    
    // meniul este unic pentru joc deci el trebuie setat activ/inactiv in functie de nevoie
    bool active = false;
    //  variabila bool ce stabileste daca a fost facut un nou record in runda tocmai incheiata
    bool newHigh = false;

    // la reset este preluata valorea din fisierul "score.txt"
    int allTimehighscore = 0;


};
