#include "Helpers2d.h"

#include "Core/StringId.h"
#include "Events/EventManager.h"
#include "Events/TextureEvent.h"
#include "Renderer/TextureManager.h"

namespace Elven::helpers2d {

void LoadSprite(SpriteComponent& spriteComponent)
{
    auto texturePtr = textures::Get(spriteComponent.textureName);
    if (texturePtr) {
        spriteComponent.texture = texturePtr;
    } else {
        textures::Load(spriteComponent.textureName, spriteComponent.textureName + ".png");

        events::Subscribe<events::TextureLoadedEvent>([&spriteComponent](const events::TextureLoadedEvent& e) {
            if (e.textureName == spriteComponent.textureName) {
                spriteComponent.texture = textures::Get(spriteComponent.textureName);
            }
        },
                                                      string_id(spriteComponent.textureName));
    }
}
} // namespace Elven::helpers2d
