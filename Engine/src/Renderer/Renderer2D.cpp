#include "Renderer2D.h"

#include "Renderer/RHI/Buffer.h"
#include "Renderer/RHI/Shader.h"
#include "Renderer/RHI/VertexArray.h"
#include "Renderer/Renderer.h"

#include "Resources/TextureManager.h"

#include "Core/FileSystem.h"

#include "Events/TextureEvent.h"

namespace elv {
struct QuadVertex {
    lia::vec4 position;
    lia::vec4 color;
    lia::vec2 uv;
    int textureUnit { 0 };
};

namespace {
constexpr uint32_t MAX_QUADS = 20000;
constexpr uint32_t MAX_QUAD_VERTICES = MAX_QUADS * 4;
constexpr uint32_t MAX_QUAD_INDICES = MAX_QUADS * 6;
constexpr uint32_t MAX_TEXTURE_SLOTS = 32;
constexpr uint8_t VERTICES_PER_QUAD = 4;
} // namespace

struct Renderer2DData {
    lia::mat4 viewProjectionMat;
    SharedPtr<Shader> shader;

    // quad batching
    SharedPtr<VertexArray> quadVAO { nullptr };
    SharedPtr<VertexBuffer> quadVBO { nullptr };
    QuadVertex* quadVerticesBegin { nullptr };
    QuadVertex* quadVerticesCurrent { nullptr };
    uint32_t quadIndexCount = 0;
    std::array<SharedPtr<Texture>, MAX_TEXTURE_SLOTS> textures;
    uint32_t usedTextureSlots = 1; // white texture

    const lia::vec4 quadPositions[4] = {
        { -0.5f, -0.5f, 1.0f, 1.0f },
        { -0.5f, 0.5f, 1.0f, 1.0f },
        { 0.5f, 0.5f, 1.0f, 1.0f },
        { 0.5f, -0.5f, 1.0f, 1.0f }
    };
    const lia::vec2 quadUV[4] = {
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f }
    };
};

Renderer2DData Renderer2D::s_data;
Renderer* Renderer2D::s_rendererPtr = nullptr;
Renderer2D::Telemetry Renderer2D::s_telemetry;

void Renderer2D::Init(Renderer* renderer)
{
    s_rendererPtr = renderer;
    s_data.quadVAO = VertexArray::Create();

    s_data.quadVerticesBegin = new QuadVertex[MAX_QUAD_VERTICES];

    s_data.quadVBO = VertexBuffer::Create(MAX_QUAD_VERTICES * sizeof(QuadVertex));
    s_data.quadVBO->SetLayout({
        { BufferAttributeType::Float4 }, // pos
        { BufferAttributeType::Float4 }, // color
        { BufferAttributeType::Float2 }, // uv
        { BufferAttributeType::Int }     // texture unit
    });
    s_data.quadVAO->AddVertexBuffer(s_data.quadVBO);

    std::uint32_t* quadIndices = new std::uint32_t[MAX_QUAD_INDICES];
    std::uint32_t offset = 0;
    for (std::uint32_t i = 0; i < MAX_QUAD_INDICES; i += 6) {
        quadIndices[i] = offset;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 3;

        quadIndices[i + 3] = offset + 1;
        quadIndices[i + 4] = offset + 2;
        quadIndices[i + 5] = offset + 3;

        offset += 4;
    }
    SharedPtr<IndexBuffer> quadEBO = IndexBuffer::Create(quadIndices, MAX_QUAD_INDICES);
    s_data.quadVAO->SetIndexBuffer(quadEBO);
    delete[] quadIndices;

    s_data.shader = ShaderManager::Load("sprite", "sprite.vert", "sprite.frag");

    // Textures
    SharedPtr<Texture> whiteTexture = textures::Get("white");
    s_data.textures[0] = std::move(whiteTexture);
}

void Renderer2D::Shutdown()
{
    delete[] s_data.quadVerticesBegin;
}

void Renderer2D::BeginScene(const Camera& camera)
{
    s_telemetry = { 0 };

    s_data.viewProjectionMat = camera.GetViewProjectionMatrix();

    StartBatch();
}

void Renderer2D::EndScene()
{
    Flush();
}

void Renderer2D::Flush()
{
    s_data.quadVBO->SetData(s_data.quadVerticesBegin, (std::uint32_t)((std::uint8_t*)s_data.quadVerticesCurrent - (std::uint8_t*)s_data.quadVerticesBegin));

    s_data.shader->Bind();
    s_data.shader->SetMatrix4("u_ViewProjection", s_data.viewProjectionMat);

    for (std::uint32_t i = 0; i < s_data.usedTextureSlots; ++i) {
        s_data.textures[i]->BindToSlot(i);
    }

    s_rendererPtr->Submit(s_data.quadVAO, s_data.quadIndexCount);

    s_telemetry.drawCalls++;
}

void Renderer2D::StartBatch()
{
    s_data.quadIndexCount = 0;
    s_data.usedTextureSlots = 1;
    s_data.quadVerticesCurrent = s_data.quadVerticesBegin;
}

void Renderer2D::NextBatch()
{
    Flush();
    StartBatch();
}

void Renderer2D::DrawQuad(const lia::mat4& model, const lia::vec4& color)
{
    DrawQuad(model, color, 0);
}

void Renderer2D::DrawQuad(const SharedPtr<Texture>& texture, const lia::mat4& model, const lia::vec4& color)
{
    if (s_data.usedTextureSlots >= MAX_TEXTURE_SLOTS) {
        NextBatch();
    }

    uint32_t textureSlot = 0;
    // check whether we already register this texture
    for (uint32_t i = 0; i < s_data.usedTextureSlots; ++i) {
        if (s_data.textures[i] == texture) {
            textureSlot = i;
            break;
        }
    }

    // add new texture if it isn't registered
    if (textureSlot == 0) {
        if (s_data.usedTextureSlots >= MAX_TEXTURE_SLOTS) {
            NextBatch();
        }
        textureSlot = s_data.usedTextureSlots;
        s_data.textures[s_data.usedTextureSlots] = texture;
        ++s_data.usedTextureSlots;
    }

    DrawQuad(model, color, textureSlot);
}

void Renderer2D::DrawQuad(const lia::mat4& model, const lia::vec4& color, int textureUnit)
{
    lia::mat4 t = model;
    t(2, 2) = 0.0f;
    if (s_data.quadIndexCount >= MAX_QUAD_INDICES) {
        NextBatch();
    }

    for (std::uint8_t i = 0; i < VERTICES_PER_QUAD; ++i) {
        s_data.quadVerticesCurrent->position = s_data.quadPositions[i] * model;
        s_data.quadVerticesCurrent->color = color;
        s_data.quadVerticesCurrent->uv = s_data.quadUV[i];
        s_data.quadVerticesCurrent->textureUnit = textureUnit;
        ++s_data.quadVerticesCurrent;
    }

    s_data.quadIndexCount += 6;
    ++s_telemetry.quadCount;
}
} // namespace elv
