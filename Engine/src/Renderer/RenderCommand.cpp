#include "Renderer/RenderCommand.h"

namespace elv {
UniquePtr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
