#pragma once

#include "RHI/RenderTarget.h"

namespace elv {

class Shader;
class Texture;
class Renderer;

class PostProcessor {

public:
    enum class BlurGradientMaskType {
        Linear,
        Checkboard,
        Triangular,
        Radial,
        Exponential
    };

public:
    PostProcessor();

    void Init(const std::uint32_t width, const std::uint32_t height);
    void BuildBlurMask(const BlurGradientMaskType type);
    SharedPtr<Texture> Process(Renderer* renderer, const SharedPtr<Texture>& src);

    void OnWindowResized(const std::uint32_t width, const std::uint32_t height);

    int GetMaskTextureId() const;

private:
    void CreateLinearGradientMask(unsigned char* data);
    void CreateCheckerboarGradientMask(unsigned char* data);
    void CreateRadialGradientMask(unsigned char* data);
    void CreateTriangularGradientMask(unsigned char* data);
    void CreateExponentialGradientMask(unsigned char* data);

public:
    bool IsBlurEnabled { false };
    int BlurMaskThreshold { 0 };
    BlurGradientMaskType GradientMaskType { BlurGradientMaskType::Linear };

private:
    UniquePtr<RenderTarget> m_renderTargetPostProcessor { nullptr };

    SharedPtr<Shader> m_blurShader { nullptr };
    SharedPtr<Texture> m_blurMaskTexture { nullptr };
};
} // namespace elv
