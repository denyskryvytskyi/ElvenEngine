#pragma once

namespace Elven {
struct TransformComponent {
    lia::vec3 pos { 0.0f, 0.0f, 0.0f };
    lia::vec2 scale { 1.0f, 1.0f };
    lia::vec3 rotation { 0.0f, 0.0f, 0.0f };
};

class Texture2D;
struct SpriteComponent {
    Elven::SharedPtr<Elven::Texture2D> m_texture { nullptr };
};

} // namespace Elven
