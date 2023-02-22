#pragma once

namespace elv {

class Texture2D;

struct Glyph {
    elv::SharedPtr<elv::Texture2D> texture { nullptr };
    lia::vec2 size;   // size of glyph
    lia::vec2 offset; // offset from baseline to left/top glyph
    uint32_t advance; // horizontal offset to advance to the next glyph
};

class FontManager {
public:
    FontManager() = default;
    FontManager(const FontManager&) = delete;
    const FontManager& operator=(const FontManager&) = delete;

    // Precompile a list of glyphs from the font
    void Load(std::string_view font);

    std::unordered_map<char, Glyph>& GetGlyphs() { return m_glyphs; };

private:
    std::unordered_map<char, Glyph> m_glyphs;
};

extern FontManager gFontManager;
} // namespace elv
