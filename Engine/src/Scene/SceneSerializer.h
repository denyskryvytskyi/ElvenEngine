#pragma once

#include <nlohmann/json_fwd.hpp>

#include "Entity.h"

namespace elv {
class Scene;
class SceneSerializer {
public:
    void Init(Scene* scene);

    void LoadScene(std::string_view sceneName);
    void SaveScene(std::string_view sceneName);

private:
    void LoadEntity(ecs::Entity entity, const nlohmann::json& jObj);
    bool SaveEntity(std::pair<ecs::Entity, ecs::ComponentMask> entityInfo, nlohmann::json& jObj);

private:
    Scene* m_pScene { nullptr };
};
} // namespace elv
