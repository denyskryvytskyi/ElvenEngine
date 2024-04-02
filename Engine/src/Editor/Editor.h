#pragma once

#include "Panels/ProfileTelemetryPanel.h"
#include "Panels/SceneHierarchyPanel.h"

namespace elv::editor {

class Editor {
public:
    void OnInit();
    void OnImGuiRender();

private:
    // panels
    SceneHierarchyPanel m_sceneHierarchyPanel;
    ProfileTelemetryPanel m_profileTelemetry;

    // settings
    bool m_isVSync { false };
    bool m_isFullScreen { false };
    bool m_isMSAAEnabled { true };
};

} // namespace elv::editor
