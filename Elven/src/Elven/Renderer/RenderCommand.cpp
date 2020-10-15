#include "elpch.h"
#include "Elven/Renderer/RenderCommand.h"

namespace Elven
{
    Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
