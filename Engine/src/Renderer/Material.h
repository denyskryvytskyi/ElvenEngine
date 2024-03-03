#pragma once

namespace elv {
struct Material {
    lia::vec3 ambient { 1.0f, 0.5f, 0.31f };
    lia::vec3 diffuse { 1.0f, 0.5f, 0.31f };
    lia::vec3 specular { 0.5f, 0.5f, 0.5f };
    float shininess { 32.0f };
};
} // namespace elv
