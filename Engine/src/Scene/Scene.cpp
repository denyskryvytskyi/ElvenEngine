#include "Scene.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Texture2D.h"

#include "Core/Timer.h"

#include "Components/ComponentsDef.h"

namespace Elven {

Scene::Scene()
    : m_cameraController(1280.0f / 720.0f)
{
}

void Scene::OnInit()
{
}

void Scene::OnShutdown()
{
    m_componentPools.clear();
    m_entities.clear();
}

void Scene::OnUpdate(float dt)
{
    m_cameraController.OnUpdate(dt);
}

void Scene::OnRender(float dt)
{
    Elven::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
    Elven::RenderCommand::Clear();

    Elven::Renderer2D::BeginScene(m_cameraController.GetCamera());

    if (HasComponentPool<TransformComponent>()) {
        auto& transformComponents = GetComponents<TransformComponent>();
        for (std::uint32_t index = 0; index < transformComponents.size(); ++index) {
            const ecs::Entity entity = GetEntity<TransformComponent>(index);

            auto& transformComponent = transformComponents[index];
            if (HasComponent<SpriteComponent>(entity)) {
                auto& spriteComponent = GetComponent<SpriteComponent>(entity);
                Elven::Renderer2D::DrawQuad(spriteComponent.m_texture, transformComponent.pos, transformComponent.scale, 45.0f);
            } else {
                Elven::Renderer2D::DrawQuad(transformComponent.pos, transformComponent.scale, 0.0f, { 0.5f, 0.5f, 0.2f, 1.0f });
            }
        }
    }

    Elven::Renderer2D::EndScene();
}

} // namespace Elven
