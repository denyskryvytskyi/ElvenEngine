#include "Renderer2D.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture2D.h"

#include "Core/FileSystem.h"

namespace Elven
{
    Renderer2D::Data Renderer2D::s_data;
    Renderer2D::Telemetry Renderer2D::s_telemetry;

    void Renderer2D::Init()
    {
        s_data.quadVAO = VertexArray::Create();

        float quadVertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f,  0.0f, 1.0f, // top left
            0.5f, 0.5f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f // bottom right
        };

        VertexBuffer* quadVBO = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
        quadVBO->SetLayout({
            { BufferAttributeType::Float3, "a_Position" },
            { BufferAttributeType::Float2, "a_TextureCoords" }
            });

        uint32_t quadIndices[] = {
            0, 1, 3,
            1, 2, 3
        };

        IndexBuffer* quadEBO = IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));

        s_data.quadVAO->AddVertexBuffer(quadVBO);
        s_data.quadVAO->SetIndexBuffer(quadEBO);
        
        s_data.shader = ShaderManager::Load("texture_shader", "texture_shader.vert", "texture_shader.frag");

        SharedPtr<Texture2D> texture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        texture->SetData(&whiteTextureData);
        s_data.whiteTexture = texture;
    }

    void Renderer2D::Shutdown()
    {
        delete s_data.quadVAO;
        delete s_data.shader;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        s_telemetry.drawCalls = 0;
        s_data.viewProjectionMat = camera.GetViewProjectionMatrix();
    }

    void Renderer2D::EndScene()
    {
        
    }

    void Renderer2D::DrawQuad(lia::mat4 model, lia::vec4 color)
    {
        if (s_data.texture.get() == nullptr)
        {
            s_data.whiteTexture->Bind();
        }
        else
        {
            s_data.texture->Bind();
        }

        s_data.shader->Bind();
        s_data.shader->SetMatrix4("u_ViewProjection", s_data.viewProjectionMat);
        s_data.shader->SetMatrix4("u_Model", model);
        s_data.shader->SetVector4f("u_Color", color);
        s_data.shader->SetInteger("u_Texture", 0);

        s_data.quadVAO->Bind();

        RenderCommand::DrawIndexed(s_data.quadVAO);
        s_telemetry.drawCalls++;

        s_data.texture.reset();
    }

    void Renderer2D::DrawQuad(lia::vec2 pos, lia::vec2 size, lia::vec4 color)
    {
        lia::mat4 model = lia::scale(lia::mat4(), lia::vec3(size.x, size.y, 0.0f));
        model = lia::translate(model, lia::vec3(pos.x, pos.y, 0.0f));

        DrawQuad(model, color);
    }

    void Renderer2D::DrawQuad(lia::vec2 pos, lia::vec2 size, SharedPtr<Texture2D> texture, lia::vec4 color)
    {
        s_data.texture = texture;
        DrawQuad(pos, size, color);
    }

    void Renderer2D::DrawRotatedQuad(lia::vec2 pos, lia::vec2 size, lia::vec4 color, float angle)
    {
        lia::mat4 model = lia::scale(lia::mat4(), lia::vec3(size.x, size.y, 0.0f));
        model = lia::rotate(model, lia::radians(angle), { 0.0f, 0.0f, 1.0f });
        model = lia::translate(model, lia::vec3(pos.x, pos.y, 0.0f));

        DrawQuad(model, color);
    }
}

