#include <nlohmann/json.hpp>

#include "Scene.h"

#include "SceneSerializer.h"

#include "Components/SceneComponents.h"
#include "Components/TransformComponent.h"
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
        std::ifstream in(fmt::format("{}{}{}", fileSystem::SCENES_PATH, sceneName, ".scene"));
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
    if (!fileSystem::IsDirectoryExists(fileSystem::SCENES_PATH)) {
        fileSystem::CreateDir(fileSystem::SCENES_PATH);
    }

    std::ofstream out(fmt::format("{}{}{}{}", fileSystem::SCENES_PATH, "new_", sceneName, ".scene"));
    json j;

    j["name"] = sceneName;

    auto entities = json::array();
    for (const auto& entityInfo : m_pScene->GetEntities()) {
        auto entityObj = json::object();
        if (SaveEntity(entityInfo, entityObj))
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
    auto quad = jObj.at("Quad").get<QuadComponent>();
    m_pScene->AddComponent(entity, std::move(quad));

    //
    auto sprite = jObj.at("Sprite").get<SpriteComponent>();
    m_pScene->AddComponent(entity, std::move(sprite));
    SpriteComponent& spriteComponentRef = m_pScene->GetComponent<SpriteComponent>(entity);
    spriteComponentRef.LoadTexture();

    //
    auto uiTransform = jObj.at("UITransform").get<UITransformComponent>();
    m_pScene->AddComponent(entity, std::move(uiTransform));

    //
    auto text = jObj.at("Text").get<TextComponent>();
    m_pScene->AddComponent(entity, std::move(text));
}

bool SceneSerializer::SaveEntity(std::pair<ecs::Entity, ecs::ComponentMask> entityInfo, nlohmann::json& jObj)
{
    bool isEntityHasComponents = false;
    if (entityInfo.second.test(ecs::GetComponentTypeId<TransformComponent>())) {

        jObj["Transform"] = m_pScene->GetComponent<TransformComponent>(entityInfo.first);
        isEntityHasComponents = true;
    }
    if (entityInfo.second.test(ecs::GetComponentTypeId<QuadComponent>())) {

        jObj["Quad"] = m_pScene->GetComponent<QuadComponent>(entityInfo.first);
        isEntityHasComponents = true;
    }
    if (entityInfo.second.test(ecs::GetComponentTypeId<SpriteComponent>())) {

        jObj["Sprite"] = m_pScene->GetComponent<SpriteComponent>(entityInfo.first);
        isEntityHasComponents = true;
    }
    if (entityInfo.second.test(ecs::GetComponentTypeId<UITransformComponent>())) {

        jObj["UITransform"] = m_pScene->GetComponent<UITransformComponent>(entityInfo.first);
        isEntityHasComponents = true;
    }
    if (entityInfo.second.test(ecs::GetComponentTypeId<TextComponent>())) {
        jObj["Text"] = m_pScene->GetComponent<TextComponent>(entityInfo.first);
        isEntityHasComponents = true;
    }

    return isEntityHasComponents;
}

} // namespace elv
