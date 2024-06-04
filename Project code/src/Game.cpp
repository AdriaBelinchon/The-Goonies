#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>

Game::Game()
{
    state = GameState::CREDITS;
   
    scene = nullptr;
    img_menu = nullptr;
    img_menu2 = nullptr;
    img_lose = nullptr;
    img_win = nullptr;
    img_credits = nullptr;

    fxMenu[0] = {};
    music[0] = {};
    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "The Goonies");

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    ToggleFullscreen();

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    //Initialise the fade in effect
    fade_transition.Set(GameState::CREDITS, 60, dst);

    InitAudioDevice();

    fxMenu[0] = LoadSound("sound/fx/GameClear.wav");
    fxMenu[1] = LoadSound("sound/fx/GameOver.wav");

    music[0] = LoadMusicStream("sound/music/OpeningDemo.ogg");
    music[1] = LoadMusicStream("sound/music/GooniesRGoodEnough.ogg");
    music[2] = LoadMusicStream("sound/music/GooniesRGoodEnoughAlt.ogg");

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    
    if (data.LoadTexture(Resource::IMG_MENU, "images/menu.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);

   

    if (data.LoadTexture(Resource::IMG_MENU2, "images/PlayStart.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu2 = data.GetTexture(Resource::IMG_MENU2);

    

    if (data.LoadTexture(Resource::IMG_CREDITS, "images/Credits.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_credits = data.GetTexture(Resource::IMG_CREDITS);

   

    if (data.LoadTexture(Resource::IMG_LOSE, "images/GameOver.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_lose = data.GetTexture(Resource::IMG_LOSE);

    

    if (data.LoadTexture(Resource::IMG_WIN, "images/Win.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_win = data.GetTexture(Resource::IMG_WIN);
    
    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{

    
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if (WindowShouldClose()) return AppStatus::QUIT;

    if (fade_transition.IsActive())
    {
        GameState prev_frame = state;
        state = fade_transition.Update();

        //Begin play and finish play are delayed due to the fading transition effect
      /*  
        if (prev_frame == GameState::CREDITS && state == GameState::MAIN_MENU)
        {
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
        }
        else if (prev_frame == GameState::MAIN_MENU2 && state == GameState::PLAYING)
        {
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
        }*/
       
    }
    else
    {
        switch (state)
        {

            case GameState::CREDITS:
                if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
                if (IsKeyPressed(KEY_SPACE))
                {
                    if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
                    fade_transition.Set(GameState::CREDITS, 60, GameState::MAIN_MENU, 60, dst);
                    
                    
                    
                }
                break;

            case GameState::MAIN_MENU:
                PlayMusicStream(music[0]);
                UpdateMusicStream(music[0]);
                if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
                if (IsKeyPressed(KEY_SPACE))
                {

                    if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
                    state = GameState::MAIN_MENU2;



                }
                break;

            case GameState::MAIN_MENU2:
                UpdateMusicStream(music[0]);
                if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
                if (IsKeyPressed(KEY_SPACE))
                {
                    if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
                    

                    fade_transition.Set(GameState::MAIN_MENU2, 60, GameState::PLAYING, 0, dst);
                    
                    

                }
                break;

            case GameState::PLAYING:
                StopMusicStream(music[0]);
                PlayMusicStream(music[1]);
                UpdateMusicStream(music[1]);
                if (IsKeyPressed(KEY_ESCAPE))
                {
                    /*FinishPlay();*/
                    fade_transition.Set(GameState::PLAYING, 60, GameState::MAIN_MENU, 60, dst);
                    
                    StopMusicStream(music[1]);
                }
                if (IsKeyPressed(KEY_F2))
                {
                    fade_transition.Set(GameState::PLAYING, 60, GameState::LOSE, 60, dst);
                    


                    
                    StopMusicStream(music[1]);
                    PlaySound(fxMenu[1]);
                }
                if (IsKeyPressed(KEY_F3) || scene->GetPlayer()->GetScore() >= 900) 
                {
                    fade_transition.Set(GameState::PLAYING, 60, GameState::WIN, 60, dst);
                    
                    StopMusicStream(music[1]);
                    PlaySound(fxMenu[0]);
                }
                
                else
                {
                    //Game logic
                    scene->Update();
                }
                break;

            case GameState::LOSE:

                if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
                
                else if (IsKeyPressed(KEY_SPACE))
                {
                    fade_transition.Set(GameState::LOSE, 60, GameState::MAIN_MENU, 60, dst);
                    

                    
                }
                break;

            case GameState::WIN:
                if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
                else if (IsKeyPressed(KEY_SPACE))
                {
                    fade_transition.Set(GameState::WIN, 60, GameState::MAIN_MENU, 60, dst);
                    

                    
                }
                break;

        }
    }
    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);
    
    switch (state)
    {
        case GameState::MAIN_MENU:
            DrawTexture(*img_menu, 0, 0, WHITE);
            break;

        case GameState::MAIN_MENU2:
            DrawTexture(*img_menu2, 0, 0, WHITE);
            break;

        case GameState::CREDITS:
            DrawTexture(*img_credits, 0, 0, WHITE);
            DrawText("The Goonies", 10, 5, 20, BLACK);
            DrawText("This academic project is a tribute to the", 4, 30, 1, BLACK);
            DrawText("original game, The Goonies (MSX), where Sloth ", 5, 40, 1, BLACK);
            DrawText("has to save all 7 goonies from Fratelli", 5, 50, 1, BLACK);
            DrawText("Subject: Projecte 1", 5, 70, 8, BLACK);
            DrawText("Degree: Disseny i desenvolupament de videojocs", 5, 80, 8, BLACK);
            DrawText("University: UPC CITM", 5, 90, 8, BLACK);
            DrawText("Team members:", 5, 110, 10, BLACK);
            DrawText("Adria Belinchon", 10, 120, 1, BLACK);
            DrawText("Pau Gutsens", 10, 130, 1, BLACK);
            DrawText("Tutors:", 5, 150, 10, BLACK);
            DrawText("Jesus Alonso", 10, 160, 10, BLACK);
            DrawText("Marc Pavon", 10, 170, 10, BLACK);
            break;

        case GameState::LOSE:
            DrawTexture(*img_lose, 0, 0, WHITE);
            break;

        case GameState::WIN:
            DrawTexture(*img_win, 0, 0, WHITE);
            break;

        case GameState::PLAYING:
            scene->Render();
            break;
    }
    
    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    if (fade_transition.IsActive()) fade_transition.Render();
    EndDrawing();
}
void Game::Cleanup()
{
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);
    data.ReleaseTexture(Resource::IMG_CREDITS);
    data.ReleaseTexture(Resource::IMG_MENU2);
    data.ReleaseTexture(Resource::IMG_WIN);
    data.ReleaseTexture(Resource::IMG_LOSE);

    UnloadRenderTexture(target);
}