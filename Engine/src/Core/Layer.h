#pragma once

#include "Events/Event.h"

namespace Elven {

class Layer {
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer() = default;

    virtual void OnAttach() { }
    virtual void OnDetach() { }
    virtual void OnUpdate(float dt) { }
    virtual void OnImGuiRender() { }

    const std::string& GetName() const { return m_debugName; }

protected:
    std::string m_debugName;
};

} // namespace Elven
