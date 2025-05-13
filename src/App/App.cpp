#include "App.h"
#include "../Ui/MainFrame.h"

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
    MainFrame* frame = new MainFrame();
    frame->Show(true);
    return true;
}
