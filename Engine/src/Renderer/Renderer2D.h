#pragma once

#include "Renderer/Camera.h"

namespace Elven
{
    class VertexArray;
    class Shader;
    class Texture2D;
    struct Renderer2DData;

    class Renderer2D
    {
    private:
        struct Telemetry
        {
            uint32_t drawCalls { 0 };
            uint32_t quadCount { 0 };
        };

    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Camera& camera);
        static void EndScene();

        static void Flush();
        static void StartBatch();
        static void NextBatch();

        static void DrawQuad(lia::vec2 pos, lia::vec2 size, lia::vec4 color);
        static void DrawQuad(lia::vec2 pos, lia::vec2 size, const SharedPtr<Texture2D>& texture, lia::vec4 color = lia::vec4(1.0f));

        /** 
        * @param angle Rotation angle in degrees
        */
        static void DrawRotatedQuad(lia::vec2 pos, lia::vec2 size, float angle, lia::vec4 color);
        static void DrawRotatedQuad(lia::vec2 pos, lia::vec2 size, const SharedPtr<Texture2D>& texture, float angle, lia::vec4 color = lia::vec4(1.0f));

        static Telemetry GetTelemetry() { return s_telemetry; }

    private:
        static void DrawQuad(lia::vec2 pos, lia::vec2 size, lia::vec4 color, float textureUnit);
        static void DrawRotatedQuad(lia::vec2 pos, lia::vec2 size, float angle, lia::vec4 color, float textureUnit);
        static void DrawQuad(lia::mat4 model, lia::vec4 color, float textureUnit = 0.0f);

    private:
        static Renderer2DData s_data;
        static Telemetry s_telemetry;
    };
}