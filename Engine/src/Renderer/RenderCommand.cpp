#include "Renderer/RenderCommand.h"

namespace Elven {
UniquePtr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
