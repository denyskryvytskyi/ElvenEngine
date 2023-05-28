#include "ComponentSystem.h"
#include "SceneManager.h"

namespace elv::ecs {
IComponentSystem::IComponentSystem()
{
    m_pScene = &GetScene();
    OnInit();
}
} // namespace elv::ecs
