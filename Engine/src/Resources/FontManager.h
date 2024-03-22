#pragma once

namespace elv {

class Texture2D;

struct Glyph {
    SharedPtr<Texture2D> texture { nullptr };
    lia::vec2 size;   // size of glyph
    lia::vec2 offset; // offset from baseline to left/top glyph
    uint32_t advance; // horizontal offset to advance to the next glyph
};

class FontManager {
public:
    using GlyphsMap = std::unordered_map<char, Glyph>;
    using FontsGlyphsMap = std::unordered_map<std::string, GlyphsMap>;

public:
    FontManager() = default;
    FontManager(const FontManager&) = delete;
    const FontManager& operator=(const FontManager&) = delete;

    // Precompile a list of glyphs from the font
    void Load(const std::string& fontName, const std::string& fontPath);

    const GlyphsMap& GetGlyphs(const std::string& fontName) const
    {
        auto it = m_fonts.find(fontName);
        if (it != m_fonts.end()) {
            return it->second;
        }

        EL_CORE_ERROR("Glyphs for the font {0} isn't found.", fontName);

        static GlyphsMap placeholder;
        return placeholder;
    };

private:
    FontsGlyphsMap m_fonts;
};

extern FontManager gFontManager;
} // namespace elv
