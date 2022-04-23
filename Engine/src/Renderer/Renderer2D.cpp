#include "Renderer2D.h"

#include "Renderer/RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Elven
{
    Renderer2D::Data Renderer2D::data;

    void Renderer2D::Init()
    {
        data.quadVAO = VertexArray::Create();

        float quadVertices[] = {
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f, 0.5f, 0.0f,   // top left
            0.5f, 0.5f, 0.0f,    // top right
            0.5f, -0.5f, 0.0f,   // bottom right
        };

        VertexBuffer* quadVBO = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
        quadVBO->SetLayout({
            { BufferAttributeType::Float3, "a_Position" }
            });

        uint32_t quadIndices[] = {
            0, 1, 3,
            1, 2, 3
        };

        IndexBuffer* quadEBO = IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));

        data.quadVAO->AddVertexBuffer(quadVBO);
        data.quadVAO->SetIndexBuffer(quadEBO);

        data.shader = ShaderManager::Load("example_shader", "Debug/res/shaders/shader.vert", "Debug/res/shaders/shader.frag");
    }

    void Renderer2D::Shutdown()
    {
        delete data.quadVAO;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        data.viewProjectionMat = camera.GetViewProjectionMatrix();
    }

    void Renderer2D::EndScene()
    {
    }

    void Renderer2D::DrawQuad(lia::mat4 model, lia::vec4 color)
    {
        data.shader->Bind();
        data.shader->SetMatrix4("u_ViewProjection", data.viewProjectionMat);
        data.shader->SetMatrix4("u_Model", model);
        data.shader->SetVector4f("u_Color", color);

        data.quadVAO->Bind();

        RenderCommand::DrawIndexed(data.quadVAO);
    }

    void Renderer2D::DrawQuad(lia::vec2 pos, lia::vec2 size, lia::vec4 color)
    {
        lia::mat4 model = lia::scale(lia::mat4(), lia::vec3(size.x, size.y, 0.0f));
        model = lia::translate(model, lia::vec3(pos.x, pos.y, 0.0f));

        DrawQuad(model, color);
    }

    void Renderer2D::DrawRotatedQuad(lia::vec2 pos, lia::vec2 size, lia::vec4 color, float angle)
    {
        lia::mat4 model = lia::scale(lia::mat4(), lia::vec3(size.x, size.y, 0.0f));
        model = lia::rotate(model, lia::radians(angle), { 0.0f, 0.0f, 1.0f });
        model = lia::translate(model, lia::vec3(pos.x, pos.y, 0.0f));

        DrawQuad(model, color);
    }
}

