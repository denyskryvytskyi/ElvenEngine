#pragma once

#include "Renderer/OrthographicCamera.h"

namespace Elven
{
    class VertexArray;
    class Shader;
    class Texture2D;

    class Renderer2D
    {
    private:
        struct Data
        {
            VertexArray* quadVAO;
            lia::mat4 viewProjectionMat;
            Shader* shader;
            SharedPtr<Texture2D> whiteTexture;
            SharedPtr<Texture2D> texture;
        };

        struct Telemetry
        {
            uint32_t drawCalls{ 0 };
        };

    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        static void DrawQuad(lia::vec2 pos, lia::vec2 size, lia::vec4 color);
        static void DrawQuad(lia::vec2 pos, lia::vec2 size, SharedPtr<Texture2D> texture, lia::vec4 color = lia::vec4(1.0f));

        /** 
        * @param angle Rotation angle in degrees
        */
        static void DrawRotatedQuad(lia::vec2 pos, lia::vec2 size, lia::vec4 color, float angle);

        static Telemetry GetTelemetry() { return s_telemetry; }

    private:
        static void DrawQuad(lia::mat4 model, lia::vec4 color);



    private:
        static Data s_data;
        static Telemetry s_telemetry;
    };
}