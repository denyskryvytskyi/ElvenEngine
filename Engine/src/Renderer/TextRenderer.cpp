#include "TextRenderer.h"

#include "RenderCommand.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"
#include "Resources/FontManager.h"
#include "Resources/TextureManager.h"
#include "Shader.h"
#include "Texture2D.h"

#include "Core/Application.h"
#include "Core/Window.h"
#include "Scene/Components/SceneComponents.h"
#include "Scene/SceneManager.h"

namespace elv {
namespace {

constexpr uint32_t quadIndexCount = 6;
constexpr uint8_t topGlyph = 'H'; // highest glyph for Y offset calculation

constexpr float uiRectMin = 0.0f;
constexpr float uiRectMax = 100.0f;
constexpr float uiRange = uiRectMax - uiRectMin;

struct Data {
    elv::SharedPtr<elv::Shader> shader;

    elv::SharedPtr<elv::VertexArray> vao;
    elv::SharedPtr<elv::VertexBuffer> vbo;

    elv::OrthoCameraBounds cameraBounds;

    lia::vec2 pixelToCamera;
    float topGlyphOffsetY { 0.0f };
} s_data;

} // namespace

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

    // disable byte-alignment restriction
    RenderCommand::DisableByteAlignment();
}

void TextRenderer::PreRender(const Camera& camera)
{
    s_data.shader->Bind();
    s_data.shader->SetMatrix4("projection", camera.GetProjectionMatrix());

    s_data.cameraBounds = GetScene().GetComponent<elv::CameraComponent>(Application::Get().GetOrthographicCameraEntity()).camera.GetOrthographicsBounds();
    s_data.pixelToCamera = GetPixelToCameraVec(s_data.cameraBounds);
}

void TextRenderer::RenderText(std::string_view text, const std::string& fontName, const lia::vec2& pos, const lia::vec2& scale, lia::vec4 color)
{
    const auto& glyphs = gFontManager.GetGlyphs(fontName);

    s_data.topGlyphOffsetY = glyphs.at(topGlyph).offset.y * s_data.pixelToCamera.y;
    s_data.shader->SetVector4f("textColor", color);
    s_data.shader->SetInteger("u_texture", 0);

    const lia::vec2 convertedPos = FromUiToCameraPos(pos, s_data.cameraBounds);
    float currentGlyphPosX = convertedPos.x;

    for (auto c : text) {

        auto it = glyphs.find(c);

        if (it == glyphs.end()) {
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

        const float delta = glyph.size.y - glyph.offset.y; // glyph height - bearing
        float ypos = convertedPos.y - (s_data.topGlyphOffsetY + delta) * scale.y;
        // clamp to the camera bottom
        ypos = std::max(s_data.cameraBounds.bottom - delta * scale.y, ypos);

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
