#pragma once

namespace elv::editor {

bool DrawVec3Control(const std::string& id, const std::string& label, lia::vec3& properties);

bool DrawSliderFloat(const std::string& label, const float min, const float max, float& property);

bool DrawRGBColorControl(const std::string& label, lia::vec3& colorProperty);
bool DrawRGBAColorControl(const std::string& label, lia::vec4& colorProperty);
} // namespace elv::editor
