#pragma once

#include "Renderer/RHI/Cubemap.h"
#include "Resources/TextureManager.h"

namespace elv {

class OpenGLCubemap : public Cubemap {
public:
    OpenGLCubemap(const std::vector<TextureManager::LoadedTextureInfo>& texturesInfo);

    virtual void Bind() override;

private:
    uint32_t m_id { 0 };
    uint32_t m_slot { 0 };
};

} // namespace elv
