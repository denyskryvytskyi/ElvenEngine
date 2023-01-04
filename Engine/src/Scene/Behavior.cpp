#include "Behavior.h"
#include "Scene.h"
#include "SceneManager.h"

namespace Elven::ecs {
IBehavior::IBehavior()
{
    p_Scene = &GetScene();
}

} // namespace Elven::ecs
