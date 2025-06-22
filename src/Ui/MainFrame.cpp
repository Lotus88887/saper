#include "MainFrame.h"
#include "BoardUI.h"
#include "UiUtil.h"
#include "UiEvents.h"
#include "UiLayout.h"
#include <algorithm>
#include "../Core/Difficulty.h"
#include <wx/strconv.h>
#include <wx/font.h>
#include <locale.h>
#include <locale>

const int ID_TIMER = 1001;
const int ID_INFO_BUTTON = 1002;

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_CLOSE(MainFrame::OnClose)
EVT_TIMER(ID_TIMER, MainFrame::OnTimer)
EVT_BUTTON(ID_INFO_BUTTON, MainFrame::OnInfoButtonClicked)
wxEND_EVENT_TABLE()

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, L"Minesweeper", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxCLIP_CHILDREN),
                         m_timer(nullptr), m_seconds(0), m_remainingMines(mines)
{
    setlocale(LC_ALL, "Polish_Poland.1250");
    std::locale::global(std::locale("Polish_Poland.1250"));
    static wxLocale* locale = nullptr;
    if (!locale) {
        locale = new wxLocale();
        if (!locale->Init(wxLANGUAGE_POLISH, wxLOCALE_LOAD_DEFAULT)) {
            locale->Init();
        }
    }
    Centre();
    wxFont emojiFont;
    wxPanel* headerPanel = CreateHeaderPanel(this, emojiFont);
    wxBoxSizer* mainSizer = CreateMainSizer(this, headerPanel);
    BindGridButtons(this);
    m_timer = new wxTimer(this, ID_TIMER);
    this->SetSizerAndFit(mainSizer);
}

void MainFrame::OnInfoButtonClicked(wxCommandEvent& event) { MainFrame_OnInfoButtonClicked(this, event); }
void MainFrame::OnTimer(wxTimerEvent& event) { MainFrame_OnTimer(this, event); }
void MainFrame::OnClose(wxCloseEvent& event) { MainFrame_OnClose(this, event); }
void MainFrame::OnButtonClicked(wxCommandEvent& event) { MainFrame_OnButtonClicked(this, event); }
void MainFrame::OnButtonRightClick(wxMouseEvent& event) { MainFrame_OnButtonRightClick(this, event); }
void MainFrame::OnDifficultyChanged(wxCommandEvent& event) { MainFrame_OnDifficultyChanged(this, event); }
