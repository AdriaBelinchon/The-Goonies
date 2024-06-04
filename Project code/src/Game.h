#pragma once
#include "Player.h"
#include "FadeTransition.h"
#include "Globals.h"
#include "Scene.h"

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();
    
    

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene *scene;
    const Texture2D *img_menu, *img_menu2, *img_lose, *img_win, *img_credits;

  

    Sound fxMenu[2];
    Music music[NUM_MUSIC];

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;

    //To make fade out/in transitions effects
    FadeTransition fade_transition;
};