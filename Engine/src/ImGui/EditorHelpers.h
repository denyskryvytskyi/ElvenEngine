#pragma once

namespace elv::editor {

void DrawVec3Control(const std::string& id, const std::string& label, lia::vec3& properties);

void DrawSliderFloat(const std::string& label, const float min, const float max, float& property);

void DrawRGBColorControl(const std::string& label, lia::vec3& colorProperty);
} // namespace elv::editor
