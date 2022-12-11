#pragma once

namespace Elven::ecs {
class IBehavior {
public:
    virtual ~IBehavior() = default;

    virtual void OnUpdate(float dt) { }
    virtual void OnRender(float dt) { }
};
} // namespace Elven::ecs
