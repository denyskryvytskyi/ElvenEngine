#pragma once

#include "Renderer/OrthographicCamera.h"

namespace Elven
{
    class VertexArray;
    class Shader;

    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        static void DrawQuad(lia::vec2 pos, lia::vec2 size, lia::vec4 color);

        /** 
        * @param angle Rotation angle in degrees
        */
        static void DrawRotatedQuad(lia::vec2 pos, lia::vec2 size, lia::vec4 color, float angle);

    private:
        static void DrawQuad(lia::mat4 model, lia::vec4 color);

    private:
        struct Data
        {
            VertexArray* quadVAO;
            lia::mat4 viewProjectionMat;
            Shader* shader;
        };

    private:
        static Data data;
    };
}