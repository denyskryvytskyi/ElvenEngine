#include "PostProcessor.h"

#include "RHI/Shader.h"
#include "RHI/Texture.h"
#include "Renderer.h"

#include "Resources/TextureManager.h"

namespace elv {

constexpr std::uint16_t kDefaultMaskThreshold = 64;
constexpr std::uint16_t kMaskWidth = 128;
constexpr std::uint16_t kMaskHeight = 96;
constexpr std::uint16_t kMaskDataSize = kMaskWidth * kMaskHeight;

constexpr float kMaxIntensity = 255.0f;
constexpr std::uint16_t kCheckboardSquareSize = 8;
constexpr std::uint16_t kExpGradientRate = 3;

PostProcessor::PostProcessor()
    : m_renderTargetPostProcessor(RenderTarget::Create())
{
}

void PostProcessor::Init(const std::uint32_t width, const std::uint32_t height)
{
    m_blurShader = ShaderManager::Load("post_processing", "post_processing.vert", "post_processing.frag");
    m_renderTargetPostProcessor->Init(width, height);

    BlurMaskThreshold = kDefaultMaskThreshold;
    BuildBlurMask(BlurGradientMaskType::Linear);
}

void PostProcessor::BuildBlurMask(const BlurGradientMaskType type)
{
    GradientMaskType = type;

    std::uint8_t* data = new std::uint8_t[kMaskDataSize];

    switch (type) {
    case BlurGradientMaskType::Linear:
        CreateLinearGradientMask(data);
        break;
    case BlurGradientMaskType::Checkboard:
        CreateCheckerboarGradientMask(data);
        break;
    case BlurGradientMaskType::Triangular:
        CreateTriangularGradientMask(data);
        break;
    case BlurGradientMaskType::Radial:
        CreateRadialGradientMask(data);
        break;
    case BlurGradientMaskType::Exponential:
        CreateExponentialGradientMask(data);
        break;
    }

    m_blurMaskTexture = textures::Load("blur_mask", kMaskWidth, kMaskHeight, 1);
    m_blurMaskTexture->SetData(data, false);

    delete[] data;
}

SharedPtr<Texture> PostProcessor::Process(Renderer* renderer, const SharedPtr<Texture>& src)
{
    if (IsBlurEnabled) {
        m_renderTargetPostProcessor->Bind();
        renderer->EnableDepthTesting(false);
        renderer->ClearBufferBit(Renderer::BufferBitType::Color);

        m_blurShader->Bind();

        src->BindToSlot(0);
        m_blurMaskTexture->BindToSlot(1);

        renderer->RenderNDCQuad();

        return m_renderTargetPostProcessor->GetColorTextureAttachment();
    }

    return src;
}

void PostProcessor::OnWindowResized(const std::uint32_t width, const std::uint32_t height)
{
    m_renderTargetPostProcessor->Resize(width, height);
}

void PostProcessor::CreateLinearGradientMask(unsigned char* data)
{
    for (int y = 0; y < kMaskHeight; ++y) {
        memset(data + y * kMaskWidth, 0, BlurMaskThreshold);
        memset(data + y * kMaskWidth + BlurMaskThreshold, static_cast<std::uint8_t>(kMaxIntensity), kMaskWidth - BlurMaskThreshold);
    }
}

void PostProcessor::CreateCheckerboarGradientMask(unsigned char* data)
{
    for (int y = 0; y < kMaskHeight; ++y) {
        memset(data + y * kMaskWidth, 0, BlurMaskThreshold);
        for (int x = BlurMaskThreshold; x < kMaskWidth; ++x) {
            data[y * kMaskWidth + x] = ((x / kCheckboardSquareSize) + (y / kCheckboardSquareSize)) % 2 == 0 ? 0 : static_cast<std::uint8_t>(kMaxIntensity);
        }
    }
}

void PostProcessor::CreateTriangularGradientMask(unsigned char* data)
{
    for (int y = 0; y < kMaskHeight; ++y) {
        memset(data + y * kMaskWidth, 0, BlurMaskThreshold);
        for (int x = BlurMaskThreshold; x < kMaskWidth; ++x) {
            const float intensity = (x + y) / static_cast<float>(kMaskWidth + kMaskHeight) * kMaxIntensity;
            data[y * kMaskWidth + x] = static_cast<std::uint8_t>(intensity);
        }
    }
}

void PostProcessor::CreateRadialGradientMask(unsigned char* data)
{
    const int centerX = (kMaskWidth - BlurMaskThreshold) / 2;
    const int centerY = kMaskHeight / 2;
    const float maxDistance = sqrt(static_cast<float>((centerX) * (centerX) + (centerY) * (centerY)));

    for (int y = 0; y < kMaskHeight; ++y) {
        memset(data + y * kMaskWidth, 0, BlurMaskThreshold);
        for (int x = BlurMaskThreshold, i = 0; x < kMaskWidth; ++x, ++i) {
            const float distance = sqrt(static_cast<float>((i - centerX) * (i - centerX) + (y - centerY) * (y - centerY)));
            const float intensity = distance / maxDistance * kMaxIntensity;

            data[y * kMaskWidth + x] = static_cast<std::uint8_t>(intensity);
        }
    }
}

void PostProcessor::CreateExponentialGradientMask(unsigned char* data)
{
    for (int y = 0; y < kMaskHeight; ++y) {
        memset(data + y * kMaskWidth, 0, BlurMaskThreshold);
        for (int x = BlurMaskThreshold, i = 0; x < kMaskWidth; ++x, ++i) {
            const float intensity = expf(kExpGradientRate * i / static_cast<float>(kMaskWidth - BlurMaskThreshold)) * kMaxIntensity;
            data[y * kMaskWidth + x] = static_cast<std::uint8_t>(intensity);
        }
    }
}

int PostProcessor::GetMaskTextureId() const
{
    return m_blurMaskTexture->GetId();
}
} // namespace elv
