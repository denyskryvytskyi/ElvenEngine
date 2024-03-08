#pragma once

#include <json_fwd.hpp>

#include "TransformComponent.h"

namespace lia {
using json = nlohmann::json;

void to_json(json& j, const lia::vec4& vec);

void from_json(const nlohmann::json& j, lia::vec4& vec);

void to_json(json& j, const lia::vec3& vec);

void from_json(const json& j, lia::vec3& vec);

void to_json(json& j, const lia::vec2& vec);

void from_json(const json& j, lia::vec2& vec);

} // namespace lia
