#include "App.h"
#include "../Ui/MainFrame.h" // For MainFrame declaration

wxIMPLEMENT_APP(MainApp);


bool MainApp::OnInit()
{
    MainFrame* frame = new MainFrame();
    frame->Show(true);
    return true;
}
