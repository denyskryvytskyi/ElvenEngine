#include <Elven.h>

#include "Invaders.h"
#include <Core/KeyCodes.h>
#include <Events/KeyEvent.h>
#include <Events/PhysicsEvent.h>
#include <Resources/AudioManager.h>
#include <Resources/FontManager.h>
#include <Scene/ComponentSystem.h>

elv::Application* elv::CreateApplication()
{
    return new Invaders();
}

namespace {
// game
const lia::vec4 kTextColor = { 0.9f, 0.4f, 0.1f, 1.0f };
constexpr float kBackTileSize = 25.6f;
constexpr float kNextWaveShowTime = 3.0f;

// player
constexpr float kPlayerSpeed = 100.0f;
const lia::vec3 kPlayerScale = { 19.8f, 15.0f, 1.0f };
constexpr float kPlayerSizeHalfX = { 9.9f };
constexpr float kPlayerSizeHalfY = { 7.5f };
const lia::vec3 kPlayerLifeScale = { 8.8f, 6.6f, 1.0f };
const lia::vec2 kPlayerLifeOffset = { 10.0f, 8.0f };
const lia::vec2 kPlayerAabbSize = { 16.8f, 12.0f };
constexpr int kPlayerInitHealth = { 3 };
const lia::vec3 kPlayerShieldScale = { 26.6f, 21.6f, 0.0f };

// bullet
float kBulletStartOffset = { 2.0f };
const lia::vec3 kBulletScale = { 2.6f, 7.4f, 1.0f };
const float kBulletSizeHalfY = { 3.7f };

// enemy
const lia::vec3 kEnemyScale = { 20.6f, 16.8f, 1.0f };
const lia::vec2 kEnemyAabbSize = { 17.6f, 13.8f };
constexpr float kEnemySizeHalfY = { 8.4f };
constexpr float kEnemiesGridMargin = { 15.0f };
constexpr float kEnemiesGridShipPadding = { 25.0f };
constexpr int kEnemiesPerGrid = 48;
const lia::vec2 kEnemiesGridSize = { 4.0f, 12.0f };
constexpr float kEnemiesGridStartOffsetY = { 75.0f };
constexpr float kEnemySpeedY = { 10.0f };
constexpr float kEnemySpeedX = { 5.0f };
constexpr int kEnemyTypes = { 4 };

// powerups
constexpr float kPowerUpSize = 8.0f;
const lia::vec3 kPowerUpScale = { kPowerUpSize, kPowerUpSize, 0.0f };
const lia::vec3 kPowerUpVelocity = { 0.0f, -1.0f, 0.0f };
constexpr float kPowerUpSpeed = { 50.0f };
const int kPowerUpChanceToGet = { 20 }; // in percents
constexpr float kPowerUpShieldActiveTime = { 3.0f };
} // namespace

enum class PowerUpType {
    Shield,
    Life
};

struct ObjectVisualInfo {
    std::string textureName;
    std::string texturePath;
};

ObjectVisualInfo GetPowerUpVisualInfo(const PowerUpType& type)
{
    static std::unordered_map<PowerUpType, ObjectVisualInfo> powerupsVisualInfo = {
        { { PowerUpType::Shield }, { "powerupShield", "Powerups/powerupBlue_shield.png" } },
        { { PowerUpType::Life }, { "powerupLife", "Powerups/powerupYellow_bolt.png" } }
    };

    return powerupsVisualInfo.find(type)->second;
}

// ----------------------- Custom events -----------------------
struct PowerUpReceivedEvent : public elv::events::Event {
public:
    PowerUpReceivedEvent(PowerUpType powerUpType)
        : powerUpType(powerUpType)
    {
    }

    EVENT_TYPE("PowerUpReceivedEvent");

public:
    PowerUpType powerUpType;
};

struct SpawnPowerUpEvent : public elv::events::Event {
public:
    SpawnPowerUpEvent(PowerUpType powerUpType, const lia::vec3& pos)
        : powerUpType(powerUpType)
        , position(pos)
    {
    }

    EVENT_TYPE("SpawnPowerUpEvent");

public:
    PowerUpType powerUpType;
    lia::vec3 position;
};

// ----------------------- Custom components -----------------------

struct BulletComponent {
    lia::vec3 velocity { 1.0f, 0.0f, 0.0f };
    float speed { 200.0f };
};

struct PlayerComponent {
    lia::vec2 widthBounds { 0.0f };  // x - left bound, y - right one
    lia::vec2 heightBounds { 0.0f }; // x - top bounds, y - bottom one

    int health { kPlayerInitHealth };
    int score { 0 };
    elv::ecs::Entity lifeEntities[kPlayerInitHealth];
    bool isShieldActive { false };
};

struct EnemyComponent {
    float shootingTimeout { 0.0f };
    float startPositionX { 0.0f };
    float movementX { 0.0f };
    elv::Timer timerToShoot;
};

struct PowerUpComponent {
    PowerUpType type;
};

// ----------------------- Custom systems -----------------------
class CombatSystem : public elv::ecs::IComponentSystem {
public:
    void OnInit() override
    {
        elv::events::Subscribe<elv::events::CollisionEnter2dEvent>([this](const elv::events::CollisionEnter2dEvent& e) { OnCollision2dEnter(e); });
    }

    void OnCollision2dEnter(const elv::events::CollisionEnter2dEvent& e)
    {
        std::string entity1Tag;
        std::string entity2Tag;
        if (m_pScene->HasComponent<elv::TagComponent>(e.entity1)) {
            entity1Tag = m_pScene->GetComponent<elv::TagComponent>(e.entity1).tag;
        }

        if (m_pScene->HasComponent<elv::TagComponent>(e.entity2)) {
            entity2Tag = m_pScene->GetComponent<elv::TagComponent>(e.entity2).tag;
        }

        if (entity1Tag.empty() || entity2Tag.empty()) {
            return;
        }

        if (entity1Tag == "player" && entity2Tag == "enemy_bullet"
            || entity2Tag == "player" && entity1Tag == "enemy_bullet") {
            // player-enemyBullet collision
            m_pScene->DestroyEntity(entity1Tag == "player" ? e.entity2 : e.entity1);
            elv::events::TriggerEvent(PlayerHitEvent(true, 1));
        } else if (entity1Tag == "enemy" && entity2Tag == "player_bullet"
                   || entity2Tag == "enemy" && entity1Tag == "player_bullet") {
            // enemy-playerBullet collision
            m_pScene->MarkToDestroyEntity(e.entity1);
            m_pScene->MarkToDestroyEntity(e.entity2);
            elv::events::TriggerEvent(PlayerHitEvent(false, 1));

            // check chance to spawn powerup and make it if received
            const int chanceToGet = elv::GetRand(0, 100);
            if (chanceToGet <= kPowerUpChanceToGet) {
                // get type of powerup
                const int powerupType = elv::GetRand(0, 1);
                const auto& enemyTransform = m_pScene->GetComponent<elv::TransformComponent>(entity1Tag == "enemy" ? e.entity1 : e.entity2);
                elv::events::TriggerEvent(SpawnPowerUpEvent(static_cast<PowerUpType>(powerupType), enemyTransform.GetPosition()));
            }

        } else if (entity1Tag == "enemy" && entity2Tag == "player"
                   || entity2Tag == "enemy" && entity1Tag == "player") {
            // enemy-player collision
            if (!m_pScene->GetComponents<PlayerComponent>().back().isShieldActive) {
                m_pScene->DestroyEntity(entity1Tag == "enemy" ? e.entity1 : e.entity2);
                elv::events::TriggerEvent(PlayerHitEvent(true, 3));
                elv::gAudioManager.Play("explosion");
            }
        }
    }
};

class EnemyControlSystem : public elv::ecs::IComponentSystem {
public:
    void OnUpdate(float dt) override
    {
        const auto& enemiesPool = m_pScene->GetComponentPool<EnemyComponent>();
        int componentIndex = 0;
        for (auto& enemy : enemiesPool->GetComponents()) {
            const auto& entity = enemiesPool->GetEntity(componentIndex);
            elv::TransformComponent& enemyTransform = m_pScene->GetComponent<elv::TransformComponent>(entity);

            if (enemy.timerToShoot.Elapsed() >= enemy.shootingTimeout) {
                // spawn enemy bullet
                const auto bulletEntity = m_pScene->CreateEntity();

                auto& bulletTransform = m_pScene->AddComponent<elv::TransformComponent>(bulletEntity);
                const auto& enemyPosition = enemyTransform.GetPosition();
                bulletTransform.SetPosition({ enemyPosition.x, enemyPosition.y - kEnemySizeHalfY - kBulletStartOffset, 0.0f });
                bulletTransform.SetScale(kBulletScale);

                auto& bulletSprite = m_pScene->AddComponent<elv::SpriteComponent>(bulletEntity);
                bulletSprite.SetTexture("laserRed", "Lasers/laserRed02.png");

                auto& bulletComponent = m_pScene->AddComponent<BulletComponent>(bulletEntity);
                bulletComponent.velocity = { 0.0f, -1.0f, 0.0f };

                m_pScene->AddComponent<elv::AABBComponent>(bulletEntity, lia::vec2(kBulletScale.x, kBulletScale.y) - 0.5f);
                m_pScene->AddComponent<elv::TagComponent>(bulletEntity, "enemy_bullet");

                enemy.timerToShoot.Restart();
            }

            // slow movement to the player
            enemyTransform.TranslateY(-kEnemySpeedY * dt);
            const auto& cameraBounds = m_pScene->GetComponents<elv::CameraComponent>().back().camera.GetOrthographicsBounds();
            if (enemyTransform.GetPosition().y <= cameraBounds.bottom) {
                elv::events::TriggerEvent(GameOverEvent());
                break;
            }

            // movement by x-axis
            enemy.movementX += kEnemySpeedX * dt;
            enemyTransform.SetPositionX(enemy.startPositionX + std::sin(enemy.movementX) * kEnemySpeedX);

            ++componentIndex;
        }
    }
};

class BulletMovementSystem : public elv::ecs::IComponentSystem {
public:
    void OnUpdate(float dt) override
    {
        auto transformsPool = m_pScene->GetComponentPool<elv::TransformComponent>();
        auto bulletsPool = m_pScene->GetComponentPool<BulletComponent>();

        const auto& cameras = m_pScene->GetComponents<elv::CameraComponent>();
        const auto& orthoCameraBounds = cameras.back().camera.GetOrthographicsBounds();

        // move bullets according to velocity
        for (uint32_t index = 0; index < bulletsPool->Size(); ++index) {
            auto entity = bulletsPool->GetEntity(index);

            auto& bullet = bulletsPool->GetComponent(entity);
            auto& transform = transformsPool->GetComponent(entity);
            transform.Translate(bullet.velocity * bullet.speed * dt);
            const auto posY = transform.GetPosition().y;
            if (posY - kBulletSizeHalfY > orthoCameraBounds.top
                || posY + kBulletSizeHalfY < orthoCameraBounds.bottom) {
                m_pScene->DestroyEntity(entity);
            }
        }
    }
};

class PowerupsSystem : public elv::ecs::IComponentSystem {
public:
    void OnInit() override
    {
        elv::events::Subscribe<elv::events::CollisionEnter2dEvent>([this](const elv::events::CollisionEnter2dEvent& e) { OnCollision2dEnter(e); });
        elv::events::Subscribe<SpawnPowerUpEvent>([this](const SpawnPowerUpEvent& e) { OnSpawnPowerUp(e); });
    }

    void OnUpdate(float dt) override
    {
        auto powerupsPool = m_pScene->GetComponentPool<PowerUpComponent>();
        auto transformsPool = m_pScene->GetComponentPool<elv::TransformComponent>();
        // move down powerups
        for (uint32_t index = 0; index < powerupsPool->Size(); ++index) {
            auto entity = powerupsPool->GetEntity(index);
            auto& transform = transformsPool->GetComponent(entity);
            transform.Translate(kPowerUpVelocity * kPowerUpSpeed * dt);
        }
    }

    void OnCollision2dEnter(const elv::events::CollisionEnter2dEvent& e)
    {
        if (m_pScene->IsEntityAlive(e.entity1) && m_pScene->IsEntityAlive(e.entity2)) {
        }

        std::string entity1Tag;
        std::string entity2Tag;

        if (m_pScene->HasComponent<elv::TagComponent>(e.entity1)) {
            entity1Tag = m_pScene->GetComponent<elv::TagComponent>(e.entity1).tag;
        }

        if (m_pScene->HasComponent<elv::TagComponent>(e.entity2)) {
            entity2Tag = m_pScene->GetComponent<elv::TagComponent>(e.entity2).tag;
        }

        if (entity1Tag.empty() || entity2Tag.empty()) {
            return;
        }

        if (entity1Tag == "player" && entity2Tag == "powerup"
            || entity1Tag == "powerup" && entity2Tag == "player") {
            const auto& powerupEntity = entity1Tag == "player" ? e.entity2 : e.entity1;
            auto& powerupComponent = m_pScene->GetComponent<PowerUpComponent>(powerupEntity);
            elv::events::TriggerEvent(PowerUpReceivedEvent(powerupComponent.type));
            m_pScene->MarkToDestroyEntity(powerupEntity);
            elv::gAudioManager.Play("powerup");
        }
    }

    void OnSpawnPowerUp(const SpawnPowerUpEvent& e)
    {
        const auto powerUpEntity = m_pScene->CreateEntity();

        auto& powerUpTransform = m_pScene->AddComponent<elv::TransformComponent>(powerUpEntity);
        powerUpTransform.SetPosition(e.position);
        powerUpTransform.SetScale(kPowerUpScale);

        auto& powerUpSprite = m_pScene->AddComponent<elv::SpriteComponent>(powerUpEntity);
        auto powerUpvisualInfo = GetPowerUpVisualInfo(e.powerUpType);
        powerUpSprite.SetTexture(powerUpvisualInfo.textureName, powerUpvisualInfo.texturePath);

        m_pScene->AddComponent<PowerUpComponent>(powerUpEntity, e.powerUpType);
        m_pScene->AddComponent<elv::AABBComponent>(powerUpEntity, lia::vec2(kPowerUpSize));
        m_pScene->AddComponent<elv::TagComponent>(powerUpEntity, "powerup");
    }
};

// ----------------------- Custom behaviors -----------------------

class PlayerBehavior : public elv::ecs::IBehavior {
public:
    void OnCreate() override
    {
        m_onHitEventHandler = [this](const PlayerHitEvent& e) { OnHit(e); };
        elv::events::Subscribe<PlayerHitEvent>(m_onHitEventHandler);

        m_powerupReceivedEventHandler = [this](const PowerUpReceivedEvent& e) { OnPowerUpReceived(e); };
        elv::events::Subscribe<PowerUpReceivedEvent>(m_powerupReceivedEventHandler);
    }

    void OnDestroy() override
    {
        elv::events::Unsubscribe<PowerUpReceivedEvent>(m_powerupReceivedEventHandler);
        elv::events::Unsubscribe<PlayerHitEvent>(m_onHitEventHandler);
        if (m_shieldEntity != elv::ecs::INVALID_ENTITY_ID) {
            p_Scene->DestroyEntity(m_shieldEntity);
        }

        auto& playerComponent = GetComponent<PlayerComponent>();
        for (const auto lifeEntity : playerComponent.lifeEntities) {
            p_Scene->DestroyEntity(lifeEntity);
        }
    }

    void OnUpdate(float dt) override
    {
        auto& transform = GetComponent<elv::TransformComponent>();
        auto& playerComponent = GetComponent<PlayerComponent>();

        if (elv::Input::IsKeyPressed(elv::key::A)) {
            transform.SetPositionX(std::max(transform.GetPosition().x - kPlayerSpeed * dt, playerComponent.widthBounds.x));
            if (playerComponent.isShieldActive)
                UpdateShieldPosition(transform.GetPosition());
        } else if (elv::Input::IsKeyPressed(elv::key::D)) {
            transform.SetPositionX(std::min(transform.GetPosition().x + kPlayerSpeed * dt, playerComponent.widthBounds.y));
            if (playerComponent.isShieldActive)
                UpdateShieldPosition(transform.GetPosition());
        }

        if (elv::Input::IsKeyPressed(elv::key::W)) {
            transform.SetPositionY(std::min(transform.GetPosition().y + kPlayerSpeed * dt, playerComponent.heightBounds.x));
            if (playerComponent.isShieldActive)
                UpdateShieldPosition(transform.GetPosition());
        } else if (elv::Input::IsKeyPressed(elv::key::S)) {
            transform.SetPositionY(std::max(transform.GetPosition().y - kPlayerSpeed * dt, playerComponent.heightBounds.y));
            if (playerComponent.isShieldActive)
                UpdateShieldPosition(transform.GetPosition());
        }

        if (playerComponent.isShieldActive && m_shieldTimer.Elapsed() >= kPowerUpShieldActiveTime) {
            playerComponent.isShieldActive = false;
            p_Scene->DestroyEntity(m_shieldEntity);
            m_shieldEntity = elv::ecs::INVALID_ENTITY_ID;
        }
    }

    // TODO: remove it when child entity system will be implemented
    void UpdateShieldPosition(const lia::vec3& pos)
    {
        auto& shieldTransform = p_Scene->GetComponent<elv::TransformComponent>(m_shieldEntity);
        shieldTransform.SetPosition(pos);
    }

    void OnHit(const PlayerHitEvent& e)
    {
        auto& playerComponent = GetComponent<PlayerComponent>();
        if (e.isReceived && !playerComponent.isShieldActive) {
            for (size_t i = 0; i < e.damage && playerComponent.health > 0; ++i) {
                // decrease player health
                p_Scene->DestroyEntity(playerComponent.lifeEntities[playerComponent.health - 1]);
                --playerComponent.health;
                if (playerComponent.health == 0) {
                    elv::events::TriggerEvent(GameOverEvent());
                }
            }

        } else if (!e.isReceived) {
            // increase score
            ++playerComponent.score;
            auto& scoreText = GetComponent<elv::TextComponent>();
            scoreText.text = fmt::format("Score: {}", playerComponent.score);
        }
    }

    void OnPowerUpReceived(const PowerUpReceivedEvent& e)
    {
        auto& playerComponent = GetComponent<PlayerComponent>();

        if (e.powerUpType == PowerUpType::Life && playerComponent.health < kPlayerInitHealth) {
            ++playerComponent.health;
            // generate player life icon
            auto playerLifeEntity = p_Scene->CreateEntity();
            playerComponent.lifeEntities[playerComponent.health - 1] = playerLifeEntity;

            auto& shipLifeSprite = p_Scene->AddComponent<elv::SpriteComponent>(playerLifeEntity);
            shipLifeSprite.SetTexture("playerLife", "ships/playerLife3_green.png");

            auto& shipLifeTransform = p_Scene->AddComponent<elv::TransformComponent>(playerLifeEntity);
            shipLifeTransform.SetScale(kPlayerLifeScale);

            const auto& cameraBounds = p_Scene->GetComponents<elv::CameraComponent>().back().camera.GetOrthographicsBounds();
            shipLifeTransform.SetPosition({ cameraBounds.left + kPlayerLifeOffset.x * playerComponent.health,
                                            cameraBounds.bottom + kPlayerLifeOffset.y, 0.0f });
        } else if (e.powerUpType == PowerUpType::Shield) {
            if (m_shieldEntity != elv::ecs::INVALID_ENTITY_ID) {
                // if shield is aready active then just reset timer
                m_shieldTimer.Restart();
            } else {
                // show shield
                m_shieldEntity = p_Scene->CreateEntity();

                auto& playerTransform = GetComponent<elv::TransformComponent>();

                auto& shipLifeTransform = p_Scene->AddComponent<elv::TransformComponent>(m_shieldEntity);
                shipLifeTransform.SetPosition(playerTransform.GetPosition());
                shipLifeTransform.SetScale(kPlayerShieldScale);

                auto& shipLifeSprite = p_Scene->AddComponent<elv::SpriteComponent>(m_shieldEntity);
                shipLifeSprite.SetTexture("shield", "Effects/shield1.png");

                playerComponent.isShieldActive = true;
                m_shieldTimer.Restart();
            }
        }
    }

private:
    elv::events::EventHandler<PlayerHitEvent> m_onHitEventHandler;
    elv::events::EventHandler<PowerUpReceivedEvent> m_powerupReceivedEventHandler;
    elv::ecs::Entity m_shieldEntity { elv::ecs::INVALID_ENTITY_ID };
    elv::Timer m_shieldTimer;
};

// ----------------------- Game -----------------------

ObjectVisualInfo GetEnemyVisualInfo(int enemyTypeIndex, int enemyColorIndex)
{
    static ObjectVisualInfo enemiesVisualInfo[][4] = {
        {
            { "enemyBlack1", "enemies/enemyBlack1.png" },
            { "enemyBlue1", "enemies/enemyBlue1.png" },
            { "enemyGreen1", "enemies/enemyGreen1.png" },
            { "enemyRed1", "enemies/enemyRed1.png" },
        },
        {
            { "enemyBlack2", "enemies/enemyBlack2.png" },
            { "enemyBlue2", "enemies/enemyBlue2.png" },
            { "enemyGreen2", "enemies/enemyGreen2.png" },
            { "enemyRed2", "enemies/enemyRed2.png" },
        },
        {
            { "enemyBlack3", "enemies/enemyBlack3.png" },
            { "enemyBlue3", "enemies/enemyBlue3.png" },
            { "enemyGreen3", "enemies/enemyGreen3.png" },
            { "enemyRed3", "enemies/enemyRed3.png" },
        },
        {
            { "enemyBlack4", "enemies/enemyBlack4.png" },
            { "enemyBlue4", "enemies/enemyBlue4.png" },
            { "enemyGreen4", "enemies/enemyGreen4.png" },
            { "enemyRed4", "enemies/enemyRed4.png" },
        },
    };

    return enemiesVisualInfo[enemyTypeIndex][enemyColorIndex];
}

void Invaders::OnCreate()
{
    elv::gFontManager.Load("kenvector_future", "assets/fonts/kenvector_future_thin.ttf");

    elv::gAudioManager.AddSound("back", "back.mp3");
    elv::gAudioManager.SetVolume("back", 0.5f);
    elv::gAudioManager.Play("back");

    elv::gAudioManager.AddSound("laser_0", "laser_shot_01.wav");
    elv::gAudioManager.AddSound("explosion", "explosion.wav");
    elv::gAudioManager.SetVolume("explosion", 0.3f);

    elv::gAudioManager.AddSound("game_over", "game_over.wav");
    elv::gAudioManager.AddSound("win", "win.wav");
    elv::gAudioManager.AddSound("powerup", "powerup.wav");

    // Preload textures
    elv::textures::Load("shield", "assets/images/Effects/shield1.png");

    auto& scene = elv::GetScene();

    scene.RegisterComponent<BulletComponent>();
    scene.RegisterComponent<PlayerComponent>();
    scene.RegisterComponent<EnemyComponent>();
    scene.RegisterComponent<PowerUpComponent>();
    scene.RegisterSystem<BulletMovementSystem>();
    scene.RegisterSystem<EnemyControlSystem>();
    scene.RegisterSystem<PowerupsSystem>();
    scene.RegisterSystem<CombatSystem>();

    SetupBackground(scene);

    // Menus
    m_gameName = scene.CreateEntity();
    scene.AddComponent<elv::UITransformComponent>(m_gameName, lia::vec2(37.0f, 30.0f), lia::vec2(1.0f, 1.0f));
    scene.AddComponent<elv::TextComponent>(m_gameName, "INVADERS", kTextColor, "kenvector_future");

    m_startMenuTextEntity = scene.CreateEntity();
    scene.AddComponent<elv::UITransformComponent>(m_startMenuTextEntity, lia::vec2(37.0f, 50.0f), lia::vec2(0.5f, 0.5f));
    scene.AddComponent<elv::TextComponent>(m_startMenuTextEntity, "Press P to start the game", kTextColor, "kenvector_future");

    m_quitMenuTextEntity = scene.CreateEntity();
    scene.AddComponent<elv::UITransformComponent>(m_quitMenuTextEntity, lia::vec2(37.0f, 60.0f), lia::vec2(0.5f, 0.5f));
    auto& quitText = scene.AddComponent<elv::TextComponent>(m_quitMenuTextEntity, "Press Q to exit", kTextColor, "kenvector_future");

    m_gameOverTextEntity = scene.CreateEntity();
    scene.AddComponent<elv::UITransformComponent>(m_gameOverTextEntity, lia::vec2(37.0f, 30.0f), lia::vec2(1.0f, 1.0f));
    auto& gameOverText = scene.AddComponent<elv::TextComponent>(m_gameOverTextEntity, "Game Over!", kTextColor, "kenvector_future");
    gameOverText.Hide();

    m_restartMenuTextEntity = scene.CreateEntity();
    scene.AddComponent<elv::UITransformComponent>(m_restartMenuTextEntity, lia::vec2(37.0f, 50.0f), lia::vec2(0.6f, 0.6f));
    auto& restartText = scene.AddComponent<elv::TextComponent>(m_restartMenuTextEntity, "Press P to start new game", kTextColor, "kenvector_future");
    restartText.Hide();

    m_nextWaveTextEntity = scene.CreateEntity();
    scene.AddComponent<elv::UITransformComponent>(m_nextWaveTextEntity, lia::vec2(37.0f, 40.0f), lia::vec2(0.8f, 0.8f));
    auto& nextWaveText = scene.AddComponent<elv::TextComponent>(m_nextWaveTextEntity, "Next round", kTextColor, "kenvector_future");
    nextWaveText.Hide();

    // event handlers
    elv::events::Subscribe<elv::events::KeyPressedEvent>([this](const elv::events::KeyPressedEvent& e) { OnKeyPressedEvent(e); });
    elv::events::Subscribe<GameOverEvent>([this](const GameOverEvent& e) { OnGameOver(e); });
    elv::events::Subscribe<PlayerHitEvent>([this](const PlayerHitEvent& e) { OnPlayerHitEvent(e); });
}

void Invaders::OnUpdate(float dt)
{
    if (isNextWaveWait) {
        if (m_nextWaveTimer.Elapsed() >= kNextWaveShowTime) {
            isNextWaveWait = false;

            // hide label
            auto& scene = elv::GetScene();
            auto& nextWaveText = scene.GetComponent<elv::TextComponent>(m_nextWaveTextEntity);
            nextWaveText.isVisible = false;

            GenerateEnemiesGrid();
        }
    }
}

void Invaders::OnWindowResizeApp()
{
    auto& scene = elv::GetScene();
    SetupBackground(scene);
}

void Invaders::GeneratePlayerBullet() const
{
    auto& scene = elv::GetScene();

    const elv::TransformComponent& playerTransform = scene.GetComponent<elv::TransformComponent>(m_playerEntity);

    const auto bulletEntity = scene.CreateEntity();

    auto& bulletTransform = scene.AddComponent<elv::TransformComponent>(bulletEntity);
    const auto& playerPos = playerTransform.GetPosition();
    bulletTransform.SetPosition({ playerPos.x, playerPos.y + kPlayerSizeHalfY + kBulletStartOffset, 0.0f });
    bulletTransform.SetScale(kBulletScale);

    auto& bulletSprite = scene.AddComponent<elv::SpriteComponent>(bulletEntity);
    bulletSprite.SetTexture("laserGreen", "Lasers/laserGreen04.png");

    auto& bulletComponent = scene.AddComponent<BulletComponent>(bulletEntity);
    bulletComponent.velocity = { 0.0f, 1.0f, 0.0f };

    scene.AddComponent<elv::AABBComponent>(bulletEntity, lia::vec2(kBulletScale.x, kBulletScale.y) - 0.5f);
    scene.AddComponent<elv::TagComponent>(bulletEntity, "player_bullet");

    // play shot sound
    elv::gAudioManager.Play("laser_0");
}

void Invaders::GenerateEnemiesGrid()
{
    m_destroyedWaveEnemies = 0;

    auto& scene = elv::GetScene();
    const auto& cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();

    const float startX = cameraBounds.left + kEnemiesGridMargin;
    const float startY = cameraBounds.top + kEnemiesGridStartOffsetY;
    const int enemyType = elv::GetRand(0, kEnemyTypes - 1);
    for (size_t i = 1; i <= kEnemiesGridSize.x; ++i) {
        for (size_t j = 1; j <= kEnemiesGridSize.y; ++j) {
            const elv::ecs::Entity enemy = scene.CreateEntity();

            auto& enemySprite = scene.AddComponent<elv::SpriteComponent>(enemy);
            ObjectVisualInfo visualInfo = GetEnemyVisualInfo(enemyType, j % 4);
            enemySprite.SetTexture(visualInfo.textureName, visualInfo.texturePath);

            auto& enemyTransform = scene.AddComponent<elv::TransformComponent>(enemy);
            enemyTransform.SetScale(kEnemyScale);
            enemyTransform.SetPosition({ startX + j * kEnemiesGridShipPadding, startY - i * kEnemiesGridShipPadding, 0.0f });

            scene.AddComponent<elv::AABBComponent>(enemy, kEnemyAabbSize);
            scene.AddComponent<elv::TagComponent>(enemy, "enemy");

            auto& enemyComponent = scene.AddComponent<EnemyComponent>(enemy);
            enemyComponent.shootingTimeout = static_cast<float>(elv::GetRand(1, 15)); // 1 - 15 seconds to shoot
            enemyComponent.startPositionX = enemyTransform.GetPosition().x;
            enemyComponent.timerToShoot.Restart();
        }
    }
}

void Invaders::SetupBackground(elv::Scene& scene)
{
    for (const auto& backTileEntity : m_backgroundTiles) {
        scene.DestroyEntity(backTileEntity);
    }
    m_backgroundTiles.clear();

    const auto& cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();

    const float cameraWidth = std::abs(cameraBounds.left) + std::abs(cameraBounds.right);
    const float cameraHeight = std::abs(cameraBounds.top) + std::abs(cameraBounds.bottom);

    const int widthTilesCount = static_cast<int>(std::ceilf(cameraWidth / kBackTileSize));
    const int heightTilesCount = static_cast<int>(std::ceilf(cameraHeight / kBackTileSize));

    const float startTileX = cameraBounds.left + kBackTileSize * 0.5f;
    const float startTileY = cameraBounds.top - kBackTileSize * 0.5f;

    for (size_t i = 0; i < heightTilesCount; ++i) {
        for (size_t j = 0; j < widthTilesCount; ++j) {
            const elv::ecs::Entity tileEntity = scene.CreateEntity();
            m_backgroundTiles.push_back(tileEntity);
            auto& tileSprite = scene.AddComponent<elv::SpriteComponent>(tileEntity);
            tileSprite.SetTexture("backTile", "Backgrounds/blue.png");
            auto& tileTransform = scene.AddComponent<elv::TransformComponent>(tileEntity);

            tileTransform.SetScale({ kBackTileSize, kBackTileSize, 1.0f });
            tileTransform.SetPosition({ startTileX + j * kBackTileSize, startTileY - i * kBackTileSize, -1.0f });
        }
    }
}

void Invaders::OnKeyPressedEvent(const elv::events::KeyPressedEvent& e)
{
    switch (e.key) {
    case elv::key::Space:
        if (m_gameState == GameState::Play && e.repeatCount == 0) {
            // spawn only per press, not on hold
            GeneratePlayerBullet();
        }
        break;
    case elv::key::P:
        if (m_gameState == GameState::Menu || m_gameState == GameState::GameOver) {
            OnRoundStart();
        }
        break;
    case elv::key::Q:
        m_running = false;
        break;
    }
}

void Invaders::OnRoundStart()
{
    auto& scene = elv::GetScene();

    // hide start menu
    scene.DestroyEntity(m_gameName);
    scene.DestroyEntity(m_startMenuTextEntity);
    scene.DestroyEntity(m_quitMenuTextEntity);

    auto& gameOverText = scene.GetComponent<elv::TextComponent>(m_gameOverTextEntity);
    gameOverText.isVisible = false;

    auto& restartText = scene.GetComponent<elv::TextComponent>(m_restartMenuTextEntity);
    restartText.isVisible = false;

    //
    const auto& cameraBounds = scene.GetComponent<elv::CameraComponent>(m_orthoCameraEntity).camera.GetOrthographicsBounds();

    // enemies
    GenerateEnemiesGrid();

    // player
    const elv::ecs::Entity ship = scene.CreateEntity();

    m_playerEntity = ship;

    auto& shipSprite = scene.AddComponent<elv::SpriteComponent>(ship);
    shipSprite.SetTexture("greenShip", "ships/playerShip3_green.png");

    auto& shipTransform = scene.AddComponent<elv::TransformComponent>(ship);
    shipTransform.SetScale(kPlayerScale);
    shipTransform.SetPosition({ 0.0f, cameraBounds.bottom + 10.0f, 0.0f });

    scene.AddComponent<elv::BehaviorComponent>(ship).Bind<PlayerBehavior>();

    auto& player = scene.AddComponent<PlayerComponent>(ship);
    player.widthBounds = { cameraBounds.left + kPlayerSizeHalfX,
                           cameraBounds.right - kPlayerSizeHalfX };
    player.heightBounds = { cameraBounds.top - kPlayerSizeHalfY,
                            cameraBounds.bottom + kPlayerSizeHalfY };

    scene.AddComponent<elv::AABBComponent>(ship, kPlayerAabbSize);
    scene.AddComponent<elv::TagComponent>(ship, "player");

    scene.AddComponent<elv::UITransformComponent>(ship, lia::vec2(1.0f, 1.0f), lia::vec2(0.6f, 0.6f));
    scene.AddComponent<elv::TextComponent>(ship, "Score: 0", kTextColor);

    // player lifes
    const float startLifeX = cameraBounds.left + kPlayerLifeOffset.x;
    for (size_t i = 0; i < kPlayerInitHealth; ++i) {
        const elv::ecs::Entity shipLife = scene.CreateEntity();
        player.lifeEntities[i] = shipLife;

        auto& shipLifeSprite = scene.AddComponent<elv::SpriteComponent>(shipLife);
        shipLifeSprite.SetTexture("playerLife", "ships/playerLife3_green.png");

        auto& shipLifeTransform = scene.AddComponent<elv::TransformComponent>(shipLife);
        shipLifeTransform.SetScale(kPlayerLifeScale);
        shipLifeTransform.SetPosition({ startLifeX + kPlayerLifeOffset.x * i, cameraBounds.bottom + kPlayerLifeOffset.y, 1.0f });
    }

    m_gameState = GameState::Play;
}

void Invaders::OnGameOver(const GameOverEvent& e)
{
    m_gameState = GameState::GameOver;

    auto& scene = elv::GetScene();

    // Remove all game entities (player, player life, score, enemies)
    scene.DestroyEntity(m_playerEntity);

    const auto& enemiesPool = scene.GetComponentPool<EnemyComponent>();
    auto& entities = enemiesPool->GetEntities();
    for (const auto& entity : entities) {
        scene.DestroyEntity(entity);
    }

    const auto& powerupsPool = scene.GetComponentPool<PowerUpComponent>();
    auto& powerupsEntities = powerupsPool->GetEntities();
    for (const auto& entity : powerupsEntities) {
        scene.DestroyEntity(entity);
    }

    // Show messages
    auto& gameOverText = scene.GetComponent<elv::TextComponent>(m_gameOverTextEntity);
    gameOverText.isVisible = true;

    auto& restartText = scene.GetComponent<elv::TextComponent>(m_restartMenuTextEntity);
    restartText.isVisible = true;

    elv::gAudioManager.Play("game_over");
}

void Invaders::OnPlayerHitEvent(const PlayerHitEvent& e)
{
    if (e.isReceived == false) {
        elv::gAudioManager.Play("explosion");
        ++m_destroyedWaveEnemies;

        if (m_destroyedWaveEnemies == kEnemiesPerGrid && m_gameState == GameState::Play) {
            elv::gAudioManager.Play("win");

            auto& scene = elv::GetScene();
            auto& nextWaveText = scene.GetComponent<elv::TextComponent>(m_nextWaveTextEntity);
            nextWaveText.isVisible = true;

            isNextWaveWait = true;
            m_nextWaveTimer.Restart();
        }
    }
}
