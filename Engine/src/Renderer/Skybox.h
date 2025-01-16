#pragma once

#include "RHI/Cubemap.h"

namespace elv {

class VertexArray;
class Renderer;
class Shader;

class Skybox {

public:
    using CubemapFaceFilepaths = std::array<std::string, 6>; // order: left, right, top, bottom, front, back

public:
    void Init();
    void Draw(Renderer* renderer, const lia::mat4& viewMatrix, const lia::mat4& projectionMatrix);
    void SetFaces(const CubemapFaceFilepaths& filepathes);

private:
    void InitCubemap(const CubemapFaceFilepaths& filepathes);

private:
    UniquePtr<Cubemap> m_cubemap { nullptr };
    SharedPtr<VertexArray> m_vao { nullptr };
    SharedPtr<Shader> m_shader { nullptr };
};
} // namespace elv
