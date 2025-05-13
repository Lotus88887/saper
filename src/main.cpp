#include <wx/wx.h>
#include <vector>

class MainApp : public wxApp
{
public:
    virtual bool OnInit();
};



wxIMPLEMENT_APP(MainApp);

class MainFrame : public wxFrame
{
public:
    MainFrame();
private:
    void OnClose(wxCloseEvent& event);
    void OnButtonClicked(wxCommandEvent& event);
    void OnButtonRightClick(wxMouseEvent& event);

    std::vector<wxButton*> buttons;

wxDECLARE_EVENT_TABLE();
    
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_CLOSE(MainFrame::OnClose)
wxEND_EVENT_TABLE()

bool MainApp::OnInit() {
    MainFrame* frame = new MainFrame();
    frame->Show(true);
    
    return true;
}

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Minesweeper")
{
    Centre();
    
    wxGridSizer* gridSizer = new wxGridSizer(9, 9, 2, 2);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            wxButton* myButton = new wxButton(this, wxID_ANY, "", wxDefaultPosition, wxSize(30, 30));
            myButton->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
            myButton->Bind(wxEVT_RIGHT_UP, &MainFrame::OnButtonRightClick, this);
            gridSizer->Add(myButton, 0, wxEXPAND);
        }
    }

    this->SetSizerAndFit(gridSizer);
}

void MainFrame::OnClose(wxCloseEvent& event) {
    if (event.CanVeto()) {
        if (wxMessageBox("Do you want to exit?", "Please confirm", wxICON_QUESTION | wxYES_NO) != wxYES) {
            event.Veto();
            return;
        }
    }
    event.Skip();
}

void MainFrame::OnButtonClicked(wxCommandEvent& event) {
    wxMessageBox("Button was clicked!");
}

void MainFrame::OnButtonRightClick(wxMouseEvent& event) {
    wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
    if (btn) {
        wxMessageBox("Right button was clicked!");
        // You can set a flag, change label, etc. here
    }
}


