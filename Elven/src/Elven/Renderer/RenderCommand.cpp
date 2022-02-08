#include "elpch.h"
#include "Elven/Renderer/RenderCommand.h"

namespace Elven
{
    RendererAPI* RenderCommand::s_RendererAPI = RendererAPI::Create();
}
