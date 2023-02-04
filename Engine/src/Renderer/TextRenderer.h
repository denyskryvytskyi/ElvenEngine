#pragma once

#include "Camera.h"

namespace elv {

class TextRenderer {
public:
    static void Init();

    // Precompile a list of glyphs from the font
    static void Load(std::string_view font);

    // Setting up projection matrix for the shader
    static void PreRender(const Camera& camera);

    /**
     * Render the text using the precompiles list of glyphs
     *
     * @param pos Fixed position (RectTransform) on the screen according to camera bounds ([0;0] is a left top position of the screen and [100;100] is the right bottom position)
     *
     */
    static void RenderText(std::string_view text, const lia::vec2& pos, const lia::vec2& scale, lia::vec4 color = lia::vec4(1.0f));
};
} // namespace elv
