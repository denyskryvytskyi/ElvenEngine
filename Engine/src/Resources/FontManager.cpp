#include "FontManager.h"

#include "Core/Profiler.h"
#include "Core/Timer.h"
#include "Resources/TextureManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace elv {

FontManager gFontManager;

constexpr uint32_t fontSize = 64;
constexpr uint16_t asciiCharNumToLoad = 128;

void FontManager::Load(const std::string& fontName, const std::string& fontPath)
{
    EL_CORE_INFO("Font {0} loading...", fontName);
    PROFILE("Font is loaded in");

    auto it = m_fonts.find(fontName);
    if (it != m_fonts.end()) {
        m_fonts[fontName].clear();
    }

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        EL_CORE_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        EL_CORE_ERROR("ERROR::FREETYPE: Failed to load font");
    }

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
        EL_CORE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
    }

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    auto& glyphsMap = m_fonts[fontName];
    for (uint8_t c = 32; c < asciiCharNumToLoad; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            EL_CORE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
            continue;
        }

        SharedPtr<Texture2D> texture { nullptr };
        // don't create texture for the Space symbol
        if (c != 32) {

            const std::string textureName = fmt::format("{}{}{}", fontName, "text2d_glyph_", c);
            texture = textures::Load(textureName, face->glyph->bitmap.width, face->glyph->bitmap.rows, 1);
            texture->SetData(face->glyph->bitmap.buffer);
        }

        Glyph glyph = {
            texture,
            lia::vec2(static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows)),
            lia::vec2(static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top)),
            static_cast<uint32_t>(face->glyph->advance.x)
        };
        glyphsMap.insert({ c, std::move(glyph) });
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
} // namespace elv
