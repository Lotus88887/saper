#include "MainFrame.h"
#include "BoardUI.h"
#include <algorithm> // for std::min

// Timer event ID and Info Button ID
const int ID_TIMER = 1001;
const int ID_INFO_BUTTON = 1002;

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_CLOSE(MainFrame::OnClose)
EVT_TIMER(ID_TIMER, MainFrame::OnTimer)
EVT_BUTTON(ID_INFO_BUTTON, MainFrame::OnInfoButtonClicked)
wxEND_EVENT_TABLE()

wxColour GetNumberColor(int number) {
    switch (number) {
    case 1: return wxColour(33, 150, 243);   // Modern Blue
    case 2: return wxColour(76, 175, 80);    // Modern Green
    case 3: return wxColour(244, 67, 54);    // Modern Red
    case 4: return wxColour(156, 39, 176);   // Modern Purple
    case 5: return wxColour(255, 152, 0);    // Modern Orange
    case 6: return wxColour(0, 188, 212);    // Modern Teal
    case 7: return wxColour(96, 125, 139);   // Modern Blue Gray
    case 8: return wxColour(121, 85, 72);    // Modern Brown
    default: return wxColour(66, 66, 66);    // Dark Gray
    }
}

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Minesweeper", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxCLIP_CHILDREN),
                         m_timer(nullptr), m_seconds(0), m_remainingMines(mines)
{
    // Center on screen
    Centre();
    
    // Add minimal padding around grid
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->AddSpacer(10); // Top padding
    
    // Create header panel for timer and mine counter
    wxPanel* headerPanel = new wxPanel(this, wxID_ANY);
    headerPanel->SetBackgroundColour(wxColour(50, 50, 50));
    wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);
    
    // Mine counter
    wxStaticText* mineLabel = new wxStaticText(headerPanel, wxID_ANY, "💣 ");
    mineLabel->SetForegroundColour(wxColour(255, 255, 255));
    mineLabel->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    headerSizer->Add(mineLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    
    m_mineCounter = new wxStaticText(headerPanel, wxID_ANY, wxString::Format("%d", mines));
    m_mineCounter->SetForegroundColour(wxColour(255, 255, 255));
    m_mineCounter->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    headerSizer->Add(m_mineCounter, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
    
    // Add spacer to push info button to the center
    headerSizer->AddStretchSpacer();
    
    // Info button
    wxButton* infoButton = new wxButton(headerPanel, ID_INFO_BUTTON, "i", wxDefaultPosition, wxSize(24, 24), wxBORDER_NONE);
    infoButton->SetForegroundColour(wxColour(255, 255, 255));
    infoButton->SetBackgroundColour(wxColour(80, 80, 80));
    wxFont infoFont = infoButton->GetFont();
    infoFont.SetWeight(wxFONTWEIGHT_BOLD);
    infoFont.SetPointSize(12);
    infoButton->SetFont(infoFont);
    headerSizer->Add(infoButton, 0, wxALIGN_CENTER_VERTICAL);
    
    // Add spacer to push timer to the right
    headerSizer->AddStretchSpacer();
    
    // Timer display
    wxStaticText* timerLabel = new wxStaticText(headerPanel, wxID_ANY, "⏱️ ");
    timerLabel->SetForegroundColour(wxColour(255, 255, 255));
    timerLabel->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    headerSizer->Add(timerLabel, 0, wxALIGN_CENTER_VERTICAL);
    
    m_timeCounter = new wxStaticText(headerPanel, wxID_ANY, "000");
    m_timeCounter->SetForegroundColour(wxColour(255, 255, 255));
    m_timeCounter->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    headerSizer->Add(m_timeCounter, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 10);
    
    headerPanel->SetSizer(headerSizer);
    mainSizer->Add(headerPanel, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    mainSizer->AddSpacer(5);
    
    // Continue with existing grid setup
    wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
    horizontalSizer->AddSpacer(10); // Left padding
    
    wxGridSizer* gridSizer = CreateBoardUI(this, rows, cols, buttons);
    horizontalSizer->Add(gridSizer, 1, wxEXPAND);
    horizontalSizer->AddSpacer(10); // Right padding
    
    mainSizer->Add(horizontalSizer, 1, wxEXPAND);
    mainSizer->AddSpacer(10); // Bottom padding

    for (auto btn : buttons) {
        btn->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
        btn->Bind(wxEVT_RIGHT_DOWN, &MainFrame::OnButtonRightClick, this);
    }

    // Create timer
    m_timer = new wxTimer(this, ID_TIMER);
    
    this->SetSizerAndFit(mainSizer);
}

void MainFrame::OnInfoButtonClicked(wxCommandEvent& WXUNUSED(event)) {
    wxString message = "Minesweeper\n\n"
                      "Created by: Kacper Kałuża, Przemysław Błaszczyk, Mateusz Biskup\n"
                      "Version: 1.0\n\n"
                      "A modern implementation of the classic Minesweeper game using wxWidgets.\n\n"
                      "© 2025 All Rights Reserved";
    
    wxMessageDialog dialog(this, message, "About Minesweeper", wxOK | wxICON_INFORMATION);
    dialog.ShowModal();
}

void MainFrame::OnTimer(wxTimerEvent& WXUNUSED(event)) {
    m_seconds++;
    m_timeCounter->SetLabel(wxString::Format("%03d", std::min(m_seconds, 999)));
}

void MainFrame::UpdateMineCounter() {
    // Count flagged cells
    int flaggedCells = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            const auto& cell = board.GetCell(r, c);
            if (cell.state == Board::CellState::Flagged) {
                flaggedCells++;
            }
        }
    }
    
    m_remainingMines = mines - flaggedCells;
    m_mineCounter->SetLabel(wxString::Format("%d", m_remainingMines));
}

void MainFrame::OnClose(wxCloseEvent& event) {
    if (event.CanVeto()) {
        if (wxMessageBox("Czy napewno chcesz wyjść?", "Proszę potwierdź", wxICON_QUESTION | wxYES_NO) != wxYES) {
            event.Veto();
            return;
        }
    }
    
    // Stop timer if running
    if (m_timer && m_timer->IsRunning()) {
        m_timer->Stop();
    }
    
    event.Skip();
}

void MainFrame::OnButtonClicked(wxCommandEvent& event) {
    wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
    if (!btn) return;
    
    // Start the timer on first click if not already running
    if (!m_timer->IsRunning()) {
        m_timer->Start(1000); // 1 second interval
    }
    
    int idx = std::find(buttons.begin(), buttons.end(), btn) - buttons.begin();
    int row = idx / cols, col = idx % cols;

    if (board.Reveal(row, col)) {
        wxFont font = btn->GetFont();
        font.SetWeight(wxFONTWEIGHT_BOLD);
        font.SetPointSize(12);
        btn->SetFont(font);
        btn->SetLabel("💣");
        btn->SetForegroundColour(wxColour(244, 67, 54)); // Modern Red for mine
        
        // Use darker background color for revealed mine
        btn->SetBackgroundColour(wxColour(200, 200, 200)); // Slightly darker gray

        // Stop timer when game is over
        if (m_timer->IsRunning()) {
            m_timer->Stop();
        }

        wxMessageBox("Przegrałeś!", "Koniec gry");
        
        board.Reset();
        ResetUI();
        return;
    }
    
    // Update UI for all cells (or just changed ones)
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) {
            const auto& cell = board.GetCell(r, c);
            wxButton* b = buttons[r * cols + c];
            if (cell.state == Board::Revealed) {
                if (cell.mine) {
                    b->SetLabel("💣");
                    b->SetForegroundColour(wxColour(244, 67, 54)); // Modern Red for mine
                }
                else if (cell.adjacent > 0) {
                    b->SetLabel(std::to_string(cell.adjacent));
                    b->SetForegroundColour(GetNumberColor(cell.adjacent));
                    // Set font: bold, size 11, default family for better readability
                    wxFont font = b->GetFont();
                    font.SetWeight(wxFONTWEIGHT_BOLD);
                    font.SetPointSize(11);
                    b->SetFont(font);
                }
                else {
                    b->SetLabel("");
                    b->SetForegroundColour(wxNullColour);
                    wxFont font = b->GetFont();
                    font.SetWeight(wxFONTWEIGHT_NORMAL);
                    font.SetPointSize(10);
                    b->SetFont(font);
                }
                
                // Use white background color for revealed cells - clean modern look
                b->SetBackgroundColour(wxColour(255, 255, 255)); 
                b->Unbind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
                b->Unbind(wxEVT_RIGHT_DOWN, &MainFrame::OnButtonRightClick, this);
            }
        }

    if (board.IsWin()) {
        // Stop timer when game is won
        if (m_timer->IsRunning()) {
            m_timer->Stop();
        }
        
        wxMessageBox("Wygrałeś!", "Koniec gry");
        board.Reset();
        ResetUI();
    }
}

void MainFrame::OnButtonRightClick(wxMouseEvent& event) {
    wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
    if (btn) {
        int idx = std::find(buttons.begin(), buttons.end(), btn) - buttons.begin();
        int row = idx / cols, col = idx % cols;
        board.ToggleFlag(row, col);

        const auto& cell = board.GetCell(row, col);

        if (cell.state == Board::CellState::Flagged) {
            wxFont font = btn->GetFont();
            font.SetWeight(wxFONTWEIGHT_BOLD);
            font.SetPointSize(12);
            btn->SetFont(font);
            btn->SetLabel("🚩");
            btn->SetForegroundColour(wxColour(244, 67, 54)); // Modern Red for flag
            // Use slightly darker background for flagged cells
            btn->SetBackgroundColour(wxColour(240, 240, 240));
        }
        else if (cell.state == Board::CellState::Hidden) {
            wxFont font = btn->GetFont();
            font.SetWeight(wxFONTWEIGHT_BOLD);
            font.SetPointSize(12);
            btn->SetFont(font);
            btn->SetLabel("");
            btn->SetForegroundColour(wxNullColour);
            // Reset to default light gray button color
            btn->SetBackgroundColour(wxColour(220, 220, 220));
        }
        
        // Update mine counter after flagging/unflagging
        UpdateMineCounter();
    }
}

void MainFrame::ResetUI() {
    for (auto btn : buttons) {
        btn->SetLabel("");
        // Reset to light gray buttons
        btn->SetBackgroundColour(wxColour(220, 220, 220));
        btn->SetForegroundColour(wxNullColour);
        // Modern font
        wxFont font = btn->GetFont();
        font.SetWeight(wxFONTWEIGHT_NORMAL);
        font.SetPointSize(10);
        font.SetFamily(wxFONTFAMILY_DEFAULT);
        btn->SetFont(font);
        
        // Re-bind events
        btn->Unbind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
        btn->Unbind(wxEVT_RIGHT_DOWN, &MainFrame::OnButtonRightClick, this);
        btn->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
        btn->Bind(wxEVT_RIGHT_DOWN, &MainFrame::OnButtonRightClick, this);
    }
    
    // Reset timer and mine counter
    if (m_timer->IsRunning()) {
        m_timer->Stop();
    }
    m_seconds = 0;
    m_timeCounter->SetLabel("000");
    
    // Reset mine counter
    m_remainingMines = mines;
    m_mineCounter->SetLabel(wxString::Format("%d", m_remainingMines));
}
