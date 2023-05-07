#include <json.hpp>

#include "Scene.h"

#include "SceneSerializer.h"

#include "Components/SceneComponents.h"
#include "Core/FileSystem.h"

namespace elv {
using json = nlohmann::json;

void SceneSerializer::Init(Scene* scene)
{
    m_pScene = scene;
}

void SceneSerializer::LoadScene(std::string_view sceneName)
{
    EL_CORE_INFO("Scene loading...");
    json j;
    {
        std::ifstream in(fmt::format("{}{}{}", FileSystem::GetScenesPath(), sceneName, ".scene"));
        if (in.is_open()) {
            in >> j;
        } else {
            EL_CORE_ERROR("Failed to load scene. File - {0}.scene", sceneName.data());
            return;
        }
    }

    for (const auto& entityObj : j.at("entities")) {
        const ecs::Entity entity = m_pScene->CreateEntity();
        LoadEntity(entity, entityObj);
    }
    EL_CORE_INFO("Scene loaded");
}

void SceneSerializer::SaveScene(std::string_view sceneName)
{
    std::ofstream out(fmt::format("{}{}{}{}", FileSystem::GetScenesPath(), "new_", sceneName, ".scene"));
    json j;

    j["name"] = sceneName;

    auto entities = json::array();
    for (ecs::Entity entity : m_pScene->GetEntities()) {
        auto entityObj = json::object();
        SaveEntity(entity, entityObj);
        entities.push_back(entityObj);
    }
    j["entities"] = entities;
    out << std::setfill(' ') << std::setw(2) << j;
}

void SceneSerializer::LoadEntity(ecs::Entity entity, const nlohmann::json& jObj)
{
    auto transform = jObj.at("Transform").get<TransformComponent>();
    m_pScene->AddComponent(entity, std::move(transform));

    //
    auto sprite = jObj.at("Sprite").get<SpriteComponent>();
    m_pScene->AddComponent(entity, std::move(sprite));
    SpriteComponent& spriteComponentRef = m_pScene->GetComponent<SpriteComponent>(entity);
    spriteComponentRef.LoadTexture();

    //
    auto rectTransform = jObj.at("RectTransform").get<RectTransformComponent>();
    m_pScene->AddComponent(entity, std::move(rectTransform));

    //
    auto text = jObj.at("TExt").get<TextComponent>();
    m_pScene->AddComponent(entity, std::move(text));
}

void SceneSerializer::SaveEntity(ecs::Entity entity, nlohmann::json& jObj)
{
    if (m_pScene->HasComponent<TransformComponent>(entity)) {

        jObj["Transform"] = m_pScene->GetComponent<TransformComponent>(entity);
    }
    if (m_pScene->HasComponent<SpriteComponent>(entity)) {

        jObj["Sprite"] = m_pScene->GetComponent<SpriteComponent>(entity);
    }
    if (m_pScene->HasComponent<RectTransformComponent>(entity)) {

        jObj["RectTransform"] = m_pScene->GetComponent<RectTransformComponent>(entity);
    }
    if (m_pScene->HasComponent<TextComponent>(entity)) {

        jObj["Text"] = m_pScene->GetComponent<TextComponent>(entity);
    }
}

} // namespace elv
