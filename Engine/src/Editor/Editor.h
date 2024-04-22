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
};

} // namespace elv::editor
