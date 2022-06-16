#include "Renderer2D.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture2D.h"

#include "Core/FileSystem.h"

#include <glad/gl.h>

namespace Elven
{
    struct QuadVertex
    {
        lia::vec4 position;
        lia::vec4 color;
        lia::vec2 uv;
        float textureUnit = 0.0f;
    };

    struct Renderer2DData
    {
        lia::mat4 viewProjectionMat;
        Shader* shader;
        SharedPtr<Texture2D> whiteTexture;

        // quad batching
        static const uint32_t maxQuads = 20000;
        static const uint32_t maxQuadVertices = maxQuads * 4;
        static const uint32_t maxQuadIndices = maxQuads * 6;
        static const uint32_t maxTextureSlots = 32;
        static const uint8_t verticesPerQuad = 4;

        VertexArray* quadVAO = nullptr;
        VertexBuffer* quadVBO = nullptr;
        QuadVertex* quadVerticesBegin = nullptr;
        QuadVertex* quadVerticesCurrent = nullptr;
        uint32_t quadIndexCount = 0;
        std::vector<SharedPtr<Texture2D>> textures;
        float usedTextureSlots = 0.0f;

        const lia::vec4 quadPositions[4] =
        {
            { -0.5f, -0.5f, 0.0f, 1.0f },
            { -0.5f,  0.5f, 0.0f, 1.0f },
            {  0.5f,  0.5f, 0.0f, 1.0f },
            {  0.5f, -0.5f, 0.0f, 1.0f }
        };
        const lia::vec2 quadUV[4] = {
            { 0.0f, 0.0f },
            { 0.0f, 1.0f },
            { 1.0f, 1.0f },
            { 1.0f, 0.0f }
        };
    };

    Renderer2DData Renderer2D::s_data;
    Renderer2D::Telemetry Renderer2D::s_telemetry;

    void Renderer2D::Init()
    {
        RenderCommand::Init();
        RenderCommand::EnableDepthTesting(false);

        s_data.quadVAO = VertexArray::Create();

        s_data.quadVerticesBegin = new QuadVertex[s_data.maxQuadVertices];

        s_data.quadVBO = VertexBuffer::Create(s_data.maxQuadVertices * sizeof(QuadVertex));
        s_data.quadVBO->SetLayout({
            { BufferAttributeType::Float4 }, // pos
            { BufferAttributeType::Float4 }, // color
            { BufferAttributeType::Float2 }, // uv
            { BufferAttributeType::Float }   // texture unit
            });
        s_data.quadVAO->AddVertexBuffer(s_data.quadVBO);

        uint32_t* quadIndices = new uint32_t[s_data.maxQuadIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_data.maxQuadIndices; i += 6)
        {
            quadIndices[i] = offset;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 3;

            quadIndices[i + 3] = offset + 1;
            quadIndices[i + 4] = offset + 2;
            quadIndices[i + 5] = offset + 3;

            offset += 4;
        }
        IndexBuffer* quadEBO = IndexBuffer::Create(quadIndices, s_data.maxQuadIndices);
        s_data.quadVAO->SetIndexBuffer(quadEBO);
        delete[] quadIndices;

        s_data.shader = ShaderManager::Load("texture_shader", "texture_shader.vert", "texture_shader.frag");

        // textures
        s_data.whiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_data.whiteTexture->SetData(&whiteTextureData);
        s_data.whiteTexture->BindToUnit(0);
        s_data.textures.reserve(s_data.maxTextureSlots);
    }

    void Renderer2D::Shutdown()
    {
        delete s_data.quadVAO;
        delete s_data.shader;
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
        s_data.quadVBO->SetData(s_data.quadVerticesBegin, (uint32_t)((char*)s_data.quadVerticesCurrent - (char*)s_data.quadVerticesBegin));

        s_data.shader->Bind();
        s_data.shader->SetMatrix4("u_ViewProjection", s_data.viewProjectionMat);

        uint32_t textureUnitIndex = 1;
        for (auto& texture: s_data.textures)
        {
            texture->BindToUnit(textureUnitIndex);
            ++textureUnitIndex;
        }

        RenderCommand::DrawIndexed(s_data.quadVAO, s_data.quadIndexCount);

        s_telemetry.drawCalls++;
    }

    void Renderer2D::StartBatch()
    {
        s_data.quadIndexCount = 0;
        s_data.usedTextureSlots = 0;
        s_data.quadVerticesCurrent = s_data.quadVerticesBegin;
        s_data.textures.clear();
    }

    void Renderer2D::NextBatch()
    {
        Flush();
        StartBatch();
    }

    void Renderer2D::DrawQuad(lia::vec3 pos, lia::vec2 scale, lia::vec4 color)
    {
        DrawQuad(pos, scale, color, 0);
    }

    void Renderer2D::DrawQuad(lia::vec3 pos, lia::vec2 scale, const SharedPtr<Texture2D>& texture, lia::vec4 color)
    {
        if (s_data.usedTextureSlots >= s_data.maxTextureSlots)
        {
            NextBatch();
        }
        s_data.textures.push_back(texture);
        s_data.usedTextureSlots++;

        DrawQuad(pos, scale, color, s_data.usedTextureSlots);
    }

    void Renderer2D::DrawRotatedQuad(lia::vec3 pos, lia::vec2 scale, lia::vec3 rotation, lia::vec4 color)
    {
        DrawRotatedQuad(pos, scale, rotation, color, 0);
    }

    void Renderer2D::DrawRotatedQuad(lia::vec3 pos, lia::vec2 scale, lia::vec3 rotation, const SharedPtr<Texture2D>& texture, lia::vec4 color)
    {
        if (s_data.usedTextureSlots >= s_data.maxTextureSlots)
        {
            NextBatch();
        }
        s_data.textures.push_back(texture);
        s_data.usedTextureSlots++;

        DrawRotatedQuad(pos, scale, rotation, color, s_data.usedTextureSlots);
    }

    /// Draw functions only for internal usage ////////////////////

    void Renderer2D::DrawQuad(lia::vec3 pos, lia::vec2 scale, lia::vec4 color, float textureUnit)
    {
        lia::mat4 model = lia::scale(lia::mat4(), lia::vec3(scale.x, scale.y, 0.0f));
        model = lia::translate(model, pos);

        DrawQuad(model, color, textureUnit);
    }

    void Renderer2D::DrawRotatedQuad(lia::vec3 pos, lia::vec2 scale, lia::vec3 rotation, lia::vec4 color, float textureUnit)
    {
        lia::mat4 model = lia::scale(lia::mat4(), lia::vec3(scale.x, scale.y, 0.0f));
        model = lia::rotateX(model, lia::radians(rotation.x));
        model = lia::rotateY(model, lia::radians(rotation.y));
        model = lia::rotateZ(model, lia::radians(rotation.z));
        model = lia::translate(model, pos);

        DrawQuad(model, color, textureUnit);
    }

    void Renderer2D::DrawQuad(lia::mat4 model, lia::vec4 color, float textureUnit)
    {
        if (s_data.quadIndexCount >= s_data.maxQuadIndices)
        {
            NextBatch();
        }

        for (uint8_t i = 0; i < s_data.verticesPerQuad; ++i)
        {
            s_data.quadVerticesCurrent->position = s_data.quadPositions[i] * model;
            s_data.quadVerticesCurrent->color = color;
            s_data.quadVerticesCurrent->uv = s_data.quadUV[i];
            s_data.quadVerticesCurrent->textureUnit = textureUnit;
            s_data.quadVerticesCurrent++;
        }

        s_data.quadIndexCount += 6;
        ++s_telemetry.quadCount;
    }
}

