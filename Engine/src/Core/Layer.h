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
    virtual void OnEvent(Events::Event& event) { }

    const std::string& GetName() const { return m_DebugName; }

protected:
    std::string m_DebugName;
};

} // namespace Elven
