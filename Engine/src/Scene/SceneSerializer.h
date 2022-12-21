#pragma once

#include <json_fwd.hpp>

#include "Entity.h"

namespace Elven {
class Scene;
class SceneSerializer {
public:
    void Init(Scene* scene);

    void LoadScene(const std::string& sceneName);
    void SaveScene(const std::string& sceneName);

private:
    void LoadEntity(ecs::Entity entity, const nlohmann::json& jObj);
    void SaveEntity(ecs::Entity entity, nlohmann::json& jObj);

private:
    Scene* m_pScene { nullptr };
};
} // namespace Elven
