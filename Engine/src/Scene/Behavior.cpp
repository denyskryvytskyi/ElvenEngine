#include "Behavior.h"
#include "SceneManager.h"

namespace elv::ecs {
IBehavior::IBehavior()
{
    p_Scene = &GetScene();
}

} // namespace elv::ecs
