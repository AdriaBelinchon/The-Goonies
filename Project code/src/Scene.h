#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "EnemyManager.h"
#include "ShotManager.h"
#include "ParticleManager.h"
#include "Text.h"


enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };


class Scene
{
public:
    Scene();
    ~Scene();

    Player* GetPlayer() const { return player; }

    AppStatus Init();
    void Update();
    void Render();
    void Release();
    void UpdateBackground(int s);
    int currentStage;

private:
    AppStatus LoadLevel(int stage);
    
    void CheckCollisions(); 
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player *player;
    TileMap *level;
    std::vector<Object*> objects;
    
    //Enemies present in the level
    EnemyManager *enemies;

    Text* font;

    Camera2D camera;
    DebugMode debug;
};

