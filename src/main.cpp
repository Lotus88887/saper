#include <wx/wx.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override;
};

class MyFrame : public wxFrame
{
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "Hello wxWidgets") {}
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}
