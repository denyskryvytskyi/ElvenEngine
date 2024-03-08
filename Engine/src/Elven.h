#pragma once

// For use by Elven Engine applications

/// Core ////////////////////////////////////////

#include "Core/Core.h"

#include "Core/Application.h"
#include "Core/EntryPoint.h"
#include "Core/Input.h"
#include "Core/Log.h"
#include "Core/SettingsConfig.h"
#include "Core/StringId.h"
#include "Core/TimeHelpers.h"
#include "Core/Timer.h"

#include "Core/Window.h"

#include "Events/EventManager.h"

#include "ImGui/ImGuiOverlay.h"

/// Renderer ////////////////////////////////////

#include "Renderer/CameraController.h"
#include "Renderer/EditorCameraController.h"
#include "Renderer/OrthographicCameraController.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include "Renderer/Light.h"
#include "Renderer/Material.h"

#include "Resources/TextureManager.h"

/// Scene ////////////////////////////////////

#include "Scene/Behavior.h"
#include "Scene/Components/SceneComponents.h"
#include "Scene/Components/TransformComponent.h"
#include "Scene/SceneManager.h"

///////////////////////////////////////////////////
