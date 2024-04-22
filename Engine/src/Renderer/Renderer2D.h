#pragma once

#include "Renderer/Camera.h"

namespace elv {

class Renderer;
class VertexArray;
class Shader;
class Texture;
struct Renderer2DData;

class Renderer2D {
private:
    struct Telemetry {
        uint32_t drawCalls { 0 };
        uint32_t quadCount { 0 };
    };

public:
    static void Init(Renderer* renderer);
    static void Shutdown();

    static void BeginScene(const Camera& camera);
    static void EndScene();

    static void Flush();
    static void StartBatch();
    static void NextBatch();

    /**
     * @param angle Rotation angle around Z axis in degrees
     */
    static void DrawQuad(const lia::vec3& pos, const lia::vec3& scale, float rotation, const lia::vec4& color = lia::vec4(1.0f));
    static void DrawQuad(const SharedPtr<Texture>& texture, const lia::vec3& pos, const lia::vec3& scale, float rotation, const lia::vec4& color = lia::vec4(1.0f));

    static Telemetry GetTelemetry() { return s_telemetry; }

private:
    static void DrawQuad(const lia::vec3& pos, const lia::vec3& scale, float rotation, const lia::vec4& color, int textureUnit);
    static void DrawQuad(const lia::mat4& model, const lia::vec4& color, int textureUnit = 0);

private:
    static Renderer2DData s_data;
    static Renderer* s_rendererPtr;
    static Telemetry s_telemetry;
};

} // namespace elv
