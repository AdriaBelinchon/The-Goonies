#include "Skull.h"
#include "Sprite.h"

Skull::Skull(const Point& p, int width, int height, int frame_width, int frame_height) :
    Enemy(p, width, height, frame_width, frame_height)
{
    state = SkullState::ROAMING;
    current_step = 0;
    current_frames = 0;
    look = Look::RIGHT; // Inicialmente mirando a la derecha
    map = nullptr;
}

Skull::~Skull() {}

AppStatus Skull::Initialise(Look look, const AABB& area)
{
    int i;
    const int n = SKULL_FRAME_SIZE;

    ResourceManager& data = ResourceManager::Instance();
    render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
    if (render == nullptr)
    {
        LOG("Failed to allocate memory for skull sprite");
        return AppStatus::ERROR;
    }

    Sprite* sprite = dynamic_cast<Sprite*>(render);
    sprite->SetNumberAnimations((int)SkullAnim::NUM_ANIMATIONS);

    sprite->SetAnimationDelay((int)SkullAnim::IDLE_RIGHT, SKULL_ANIM_DELAY);
    sprite->AddKeyFrame((int)SkullAnim::IDLE_RIGHT, { 0, 2 * n, n, n + 2 });
    sprite->SetAnimationDelay((int)SkullAnim::IDLE_LEFT, SKULL_ANIM_DELAY);
    sprite->AddKeyFrame((int)SkullAnim::IDLE_LEFT, { 0, 2 * n, -n, n + 2 });

    sprite->SetAnimationDelay((int)SkullAnim::WALKING_RIGHT, SKULL_ANIM_DELAY);
    for (int i = 0; i < 2; ++i)
        sprite->AddKeyFrame((int)SkullAnim::WALKING_RIGHT, { (float)i * n, 2 * n, n, n + 2 });
    sprite->SetAnimationDelay((int)SkullAnim::WALKING_LEFT, SKULL_ANIM_DELAY);
    for (int i = 0; i < 2; ++i)
        sprite->AddKeyFrame((int)SkullAnim::WALKING_LEFT, { (float)i * n, 2 * n, -n, n + 2 });

    this->look = look;
    if (look == Look::LEFT) sprite->SetAnimation((int)SkullAnim::WALKING_LEFT);
    else if (look == Look::RIGHT) sprite->SetAnimation((int)SkullAnim::WALKING_RIGHT);

    visibility_area = area;

    return AppStatus::OK;
}

void Skull::SetAnimation(int id)
{
    Sprite* sprite = dynamic_cast<Sprite*>(render);
    sprite->SetAnimation(id);
}

bool Skull::Update(const AABB& box)
{
    Sprite* sprite = dynamic_cast<Sprite*>(render);
    bool shoot = false;

    if (state == SkullState::ROAMING)
    {
        pos += (look == Look::RIGHT) ? Point{ SKULL_SPEED, 0 } : Point{ -SKULL_SPEED, 0 };

        if (CollidesWithWall())
        {
            look = (look == Look::RIGHT) ? Look::LEFT : Look::RIGHT;
            int anim_id = (look == Look::RIGHT) ? (int)SkullAnim::WALKING_RIGHT : (int)SkullAnim::WALKING_LEFT;
            sprite->SetAnimation(anim_id);
            UpdateLook(anim_id);
        }

        current_frames++;
        if (current_frames >= SKULL_ANIM_DELAY)
        {
            current_frames = 0;
            sprite->Update();
        }
    }

    return true;
}

void Skull::UpdateLook(int anim_id)
{
    SkullAnim anim = (SkullAnim)anim_id;
    look = (anim == SkullAnim::IDLE_LEFT || anim == SkullAnim::WALKING_LEFT) ? Look::LEFT : Look::RIGHT;
}

void Skull::Stop()
{
    dir = { 0,0 };
    state = SkullState::IDLE;
    if (look == Look::RIGHT) SetAnimation((int)SkullAnim::IDLE_RIGHT);
    else SetAnimation((int)SkullAnim::IDLE_LEFT);
}

bool Skull::CollidesWithWall()
{
    if (map == nullptr) return false; // Evita el uso de un puntero nulo

    AABB box = GetHitbox();
    int prev_x = pos.x;

    if (look == Look::RIGHT)
    {
        if (map->TestCollisionWallRight(box))
        {
            pos.x -= SKULL_SPEED; // Revertir el movimiento
            pos.x = prev_x;
            return true;
        }
    }
    else
    {
        if (map->TestCollisionWallLeft(box))
        {
            pos.x += SKULL_SPEED; // Revertir el movimiento
            pos.x = prev_x;
            return true;
        }
    }

    return false;
}