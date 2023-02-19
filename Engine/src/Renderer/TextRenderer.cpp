#include "TextRenderer.h"

#include "RenderCommand.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Shader.h"
#include "Texture2D.h"
#include "TextureManager.h"

#include "Core/Application.h"
#include "Core/Window.h"
#include "Scene/Components/SceneComponents.h"
#include "Scene/SceneManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace {
constexpr uint32_t fontSize = 64;
constexpr uint16_t asciiCharNumToLoad = 128;
constexpr uint32_t quadIndexCount = 6;
constexpr uint8_t topGlyph = 'H'; // highest glyph for Y offset calculation

constexpr float uiRectMin = 0.0f;
constexpr float uiRectMax = 100.0f;
constexpr float uiRange = uiRectMax - uiRectMin;

struct Glyph {
    elv::SharedPtr<elv::Texture2D> texture { nullptr };
    lia::vec2 size;   // size of glyph
    lia::vec2 offset; // offset from baseline to left/top glyph
    uint32_t advance; // horizontal offset to advance to the next glyph
};

struct Data {
    std::unordered_map<char, Glyph> glyphs;
    elv::SharedPtr<elv::Shader> shader;

    elv::SharedPtr<elv::VertexArray> vao;
    elv::SharedPtr<elv::VertexBuffer> vbo;

    elv::OrthoCameraBounds cameraBounds;

    lia::vec2 pixelToCamera;
    float topGlyphOffsetY { 0.0f };
} s_data;

} // namespace

namespace elv {

// Convert from pixel size to camera space size
static lia::vec2 GetPixelToCameraVec(const elv::OrthoCameraBounds& cameraBounds)
{
    return {
        (cameraBounds.right - cameraBounds.left) / static_cast<float>(Application::Get().GetWindow()->GetWidth()),
        (cameraBounds.top - cameraBounds.bottom) / static_cast<float>(Application::Get().GetWindow()->GetHeight())
    };
}

static lia::vec2 FromUiToCameraPos(const lia::vec2& pos, const elv::OrthoCameraBounds cameraBounds)
{
    const float cameraRangeX = cameraBounds.right - cameraBounds.left;
    const float cameraRangeY = cameraBounds.top - cameraBounds.bottom;

    return { ((pos.x - uiRectMin) * cameraRangeX / uiRange) + cameraBounds.left,
             cameraBounds.top - ((pos.y - uiRectMin) * cameraRangeY / uiRange) };
}

void TextRenderer::Init()
{
    s_data.shader = ShaderManager::Load("text2d", "text2d.vert", "text2d.frag");

    s_data.vao = VertexArray::Create();

    s_data.vbo = VertexBuffer::Create(sizeof(float) * 6 * 4);
    s_data.vbo->SetLayout({
        { BufferAttributeType::Float2 }, // pos
        { BufferAttributeType::Float2 }, // uv
    });
    s_data.vao->AddVertexBuffer(s_data.vbo);

    uint32_t quadIndices[quadIndexCount] = {
        0, 1, 3,
        1, 2, 3

    };
    SharedPtr<IndexBuffer> ebo = IndexBuffer::Create(quadIndices, quadIndexCount);
    s_data.vao->SetIndexBuffer(ebo);
}

void TextRenderer::Load(std::string_view font)
{
    s_data.glyphs.clear();

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        EL_CORE_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
    }

    FT_Face face;
    if (FT_New_Face(ft, "assets/fonts/arial.ttf", 0, &face)) {
        EL_CORE_ERROR("ERROR::FREETYPE: Failed to load font");
    }

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
        EL_CORE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
    }

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // disable byte-alignment restriction
    RenderCommand::DisableByteAlignment();

    for (uint8_t c = 32; c < asciiCharNumToLoad; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            EL_CORE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
            continue;
        }

        SharedPtr<Texture2D> texture { nullptr };
        // don't create texture for the Space symbol
        if (c != 32) {

            const std::string textureName = std::format("{}{}", "text2d_glyph_", c);
            texture = textures::Load(textureName, face->glyph->bitmap.width, face->glyph->bitmap.rows, 1);
            texture->SetData(face->glyph->bitmap.buffer);
        }

        Glyph glyph = {
            texture,
            lia::vec2(static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows)),
            lia::vec2(static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top)),
            static_cast<uint32_t>(face->glyph->advance.x)
        };
        s_data.glyphs.insert({ c, std::move(glyph) });
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::PreRender(const Camera& camera)
{
    s_data.shader->Bind();
    s_data.shader->SetMatrix4("projection", camera.GetProjectionMatrix());

    s_data.cameraBounds = GetScene().GetComponent<elv::CameraComponent>(Application::Get().GetOrthographicCameraEntity()).camera.GetOrthographicsBounds();
    s_data.pixelToCamera = GetPixelToCameraVec(s_data.cameraBounds);
    s_data.topGlyphOffsetY = s_data.glyphs[topGlyph].offset.y * s_data.pixelToCamera.y;
}

void TextRenderer::RenderText(std::string_view text, const lia::vec2& pos, const lia::vec2& scale, lia::vec4 color)
{
    s_data.shader->Bind();
    s_data.shader->SetVector4f("textColor", color);

    const lia::vec2 convertedPos = FromUiToCameraPos(pos, s_data.cameraBounds);
    float currentGlyphPosX = convertedPos.x;

    for (auto c : text) {

        auto it = s_data.glyphs.find(c);

        if (it == s_data.glyphs.end()) {
            EL_ERROR("Glyph {0} isn't supported by loaded font.", c);
            continue;
        }

        Glyph glyph = it->second;
        if (c == ' ') {
            // now advance cursors for next glyph
            currentGlyphPosX += s_data.pixelToCamera.x * scale.x * static_cast<float>(glyph.advance >> 6); // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
            continue;
        }

        glyph.offset = { glyph.offset.x * s_data.pixelToCamera.x, glyph.offset.y * s_data.pixelToCamera.y };
        glyph.size = { glyph.size.x * s_data.pixelToCamera.x, glyph.size.y * s_data.pixelToCamera.y };

        const float w = glyph.size.x * scale.x;
        const float h = glyph.size.y * scale.y;

        const float xpos = currentGlyphPosX + glyph.offset.x * scale.x;

        float ypos = convertedPos.y - (s_data.topGlyphOffsetY + glyph.size.y - glyph.offset.y) * scale.y;
        ypos = std::max(s_data.cameraBounds.bottom, ypos);

        const float vertices[4][4] = {
            { xpos, ypos + h, 0.0f, 0.0f },
            { xpos, ypos, 0.0f, 1.0f },
            { xpos + w, ypos, 1.0f, 1.0f },
            { xpos + w, ypos + h, 1.0f, 0.0f },
        };

        s_data.vbo->SetData(vertices, sizeof(vertices));

        glyph.texture->BindToUnit(0);

        RenderCommand::DrawIndexed(s_data.vao, quadIndexCount);

        // now advance cursors for next glyph
        currentGlyphPosX += s_data.pixelToCamera.x * scale.x * static_cast<float>(glyph.advance >> 6); // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    }
}
} // namespace elv
