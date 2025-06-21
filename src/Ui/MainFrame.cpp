#include "MainFrame.h"
#include "BoardUI.h"
#include <algorithm> // for std::min
#include "../Core/Difficulty.h"
#include <wx/strconv.h>
#include <wx/font.h>
#include <locale.h>
#include <locale>

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

// Helper function to set a Unicode/emoji font with fallback using wxFontInfo
wxFont GetEmojiFont(int size, int weight = wxFONTWEIGHT_BOLD) {
    wxFontInfo info(size);
    info.Family(wxFONTFAMILY_DEFAULT).Style(wxFONTSTYLE_NORMAL).Weight(weight);
    // Try common emoji/Unicode fonts in order
    const char* fontNames[] = {
        "Segoe UI Emoji",        // Windows
        "Noto Color Emoji",     // Linux
        "Apple Color Emoji",    // macOS
        "Arial Unicode MS",     // Windows fallback
        "Segoe UI Symbol",      // Windows fallback
        "Symbola"               // Linux/Windows fallback
    };
    for (const char* name : fontNames) {
        info.FaceName(name);
        wxFont font(info);
        if (font.IsOk()) {
            return font;
        }
    }
    // Fallback to default
    return wxFont(info);
}

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, L"Minesweeper", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxCLIP_CHILDREN),
                         m_timer(nullptr), m_seconds(0), m_remainingMines(mines)
{
    // Always use Polish formatting for C and C++ locale
    setlocale(LC_ALL, "pl_PL.UTF-8");
    std::locale::global(std::locale("pl_PL.UTF-8"));

    // Set Polish UTF-8 locale for proper Unicode and Polish support
    static wxLocale* locale = nullptr;
    if (!locale) {
        locale = new wxLocale();
        if (!locale->Init(wxLANGUAGE_POLISH, wxLOCALE_LOAD_DEFAULT)) {
            locale->Init();
        }
    }

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
    wxFont emojiFont = GetEmojiFont(12);
    wxStaticText* mineLabel = new wxStaticText(headerPanel, wxID_ANY, L"💣 ");
    mineLabel->SetForegroundColour(wxColour(255, 255, 255));
    mineLabel->SetFont(emojiFont);
    headerSizer->Add(mineLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    
    m_mineCounter = new wxStaticText(headerPanel, wxID_ANY, std::to_wstring(mines));
    m_mineCounter->SetForegroundColour(wxColour(255, 255, 255));
    m_mineCounter->SetFont(emojiFont);
    headerSizer->Add(m_mineCounter, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
    
    // Dodaj ComboBox do wyboru poziomu trudności
    wxArrayString diffChoices;
    diffChoices.Add("Łatwy");
    diffChoices.Add("Średni");
    diffChoices.Add("Trudny");
    m_difficultyCombo = new wxComboBox(headerPanel, wxID_ANY, diffChoices[0], wxDefaultPosition, wxDefaultSize, diffChoices, wxCB_READONLY);
    headerSizer->Add(m_difficultyCombo, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    m_difficultyCombo->SetSelection(0);
    m_difficultyCombo->Bind(wxEVT_COMBOBOX, &MainFrame::OnDifficultyChanged, this);

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
    wxStaticText* timerLabel = new wxStaticText(headerPanel, wxID_ANY, L"⏱️ ");
    timerLabel->SetForegroundColour(wxColour(255, 255, 255));
    timerLabel->SetFont(emojiFont);
    headerSizer->Add(timerLabel, 0, wxALIGN_CENTER_VERTICAL);
    
    m_timeCounter = new wxStaticText(headerPanel, wxID_ANY, L"000");
    m_timeCounter->SetForegroundColour(wxColour(255, 255, 255));
    m_timeCounter->SetFont(emojiFont);
    headerSizer->Add(m_timeCounter, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 10);
    
    headerPanel->SetSizer(headerSizer);
    mainSizer->Add(headerPanel, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    mainSizer->AddSpacer(5);
    
    // Continue with existing grid setup
    m_horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
    m_horizontalSizer->AddSpacer(10); // Left padding
    
    m_gridSizer = CreateBoardUI(this, rows, cols, buttons);
    m_horizontalSizer->Add(m_gridSizer, 1, wxEXPAND);
    m_horizontalSizer->AddSpacer(10); // Right padding
    
    mainSizer->Add(m_horizontalSizer, 1, wxEXPAND);
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
    wxString message = L"Minesweeper\n\n"
                      L"Created by: Kacper Kałuża, Przemysław Błaszczyk, Mateusz Biskup\n"
                      L"Version: 1.0\n\n"
                      L"A modern implementation of the classic Minesweeper game using wxWidgets.\n\n"
                      L"© 2025 All Rights Reserved";
    
    wxMessageDialog dialog(this, message, L"About Minesweeper", wxOK | wxICON_INFORMATION);
    dialog.ShowModal();
}

void MainFrame::OnTimer(wxTimerEvent& WXUNUSED(event)) {
    m_seconds++;
    m_timeCounter->SetLabel(std::to_wstring(std::min(m_seconds, 999)));
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
    m_mineCounter->SetLabel(std::to_wstring(m_remainingMines));
}

void MainFrame::OnClose(wxCloseEvent& event) {
    if (event.CanVeto()) {
        if (wxMessageBox(L"Czy napewno chcesz wyjść?", L"Proszę potwierdź", wxICON_QUESTION | wxYES_NO) != wxYES) {
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
        wxFont font = GetEmojiFont(12);
        btn->SetFont(font);
        btn->SetLabel(L"💣");
        btn->SetForegroundColour(wxColour(244, 67, 54)); // Modern Red for mine
        
        // Use darker background color for revealed mine
        btn->SetBackgroundColour(wxColour(200, 200, 200)); // Slightly darker gray

        // Stop timer when game is over
        if (m_timer->IsRunning()) {
            m_timer->Stop();
        }

        wxMessageBox(L"Przegrałeś!", L"Koniec gry");
        
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
                    wxFont font = GetEmojiFont(12);
                    b->SetFont(font);
                    b->SetLabel(L"💣");
                    b->SetForegroundColour(wxColour(244, 67, 54)); // Modern Red for mine
                }
                else if (cell.adjacent > 0) {
                    b->SetLabel(std::to_wstring(cell.adjacent));
                    b->SetForegroundColour(GetNumberColor(cell.adjacent));
                    // Set font: bold, size 11, default family for better readability
                    wxFont font = GetEmojiFont(11);
                    font.SetWeight(wxFONTWEIGHT_BOLD);
                    b->SetFont(font);
                }
                else {
                    b->SetLabel(L"");
                    b->SetForegroundColour(wxNullColour);
                    wxFont font = GetEmojiFont(10);
                    font.SetWeight(wxFONTWEIGHT_NORMAL);
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
        
        wxMessageBox(L"Wygrałeś!", L"Koniec gry");
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
            wxFont font = GetEmojiFont(12);
            btn->SetFont(font);
            btn->SetLabel(L"🚩");
            btn->SetForegroundColour(wxColour(244, 67, 54)); // Modern Red for flag
            // Use slightly darker background for flagged cells
            btn->SetBackgroundColour(wxColour(240, 240, 240));
        }
        else if (cell.state == Board::CellState::Hidden) {
            wxFont font = GetEmojiFont(12);
            font.SetWeight(wxFONTWEIGHT_BOLD);
            btn->SetFont(font);
            btn->SetLabel(L"");
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
        btn->SetLabel(L"");
        // Reset to light gray buttons
        btn->SetBackgroundColour(wxColour(220, 220, 220));
        btn->SetForegroundColour(wxNullColour);
        // Modern font
        wxFont font = GetEmojiFont(10);
        font.SetWeight(wxFONTWEIGHT_NORMAL);
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
    m_timeCounter->SetLabel(L"000");
    
    // Reset mine counter
    m_remainingMines = mines;
    m_mineCounter->SetLabel(std::to_wstring(m_remainingMines));
}

void MainFrame::OnDifficultyChanged(wxCommandEvent& event) {
    int sel = m_difficultyCombo->GetSelection();
    switch (sel) {
    case 0: m_difficulty = Difficulty::Easy; break;
    case 1: m_difficulty = Difficulty::Medium; break;
    case 2: m_difficulty = Difficulty::Hard; break;
    default: m_difficulty = Difficulty::Easy; break;
    }
    auto settings = GetSettings(m_difficulty);
    rows = settings.rows;
    cols = settings.cols;
    mines = settings.mines;
    board = Board(rows, cols, mines);

    // Remove old buttons from UI
    for (auto btn : buttons) {
        btn->Destroy();
    }
    buttons.clear();

    // Remove old sizer from the layout
    if (m_gridSizer) {
        m_gridSizer->Clear(true); // true = delete windows
        // Do not set m_gridSizer to nullptr yet, we will reuse the pointer
    }

    // Create new board UI
    m_gridSizer = CreateBoardUI(this, rows, cols, buttons);

    // If m_horizontalSizer is not set, find and store it
    if (!m_horizontalSizer) {
        wxBoxSizer* mainSizer = static_cast<wxBoxSizer*>(GetSizer());
        if (mainSizer) {
            wxSizerItemList& items = mainSizer->GetChildren();
            for (auto item : items) {
                m_horizontalSizer = dynamic_cast<wxBoxSizer*>(item->GetSizer());
                if (m_horizontalSizer) break;
            }
        }
    }

    // Remove all items from horizontal sizer and re-add paddings and grid
    if (m_horizontalSizer) {
        m_horizontalSizer->Clear(false); // remove old grid sizer, don't delete children (already destroyed)
        m_horizontalSizer->AddSpacer(10); // Left padding
        m_horizontalSizer->Add(m_gridSizer, 1, wxEXPAND);
        m_horizontalSizer->AddSpacer(10); // Right padding
    }

    // Re-bind events for new buttons
    for (auto btn : buttons) {
        btn->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
        btn->Bind(wxEVT_RIGHT_DOWN, &MainFrame::OnButtonRightClick, this);
    }

    Layout();
    Fit();
    ResetUI();
}
