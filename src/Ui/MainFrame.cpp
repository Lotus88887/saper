#include "MainFrame.h"
#include "BoardUI.h"


wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_CLOSE(MainFrame::OnClose)

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

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Minesweeper", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxCLIP_CHILDREN)
{
    // Center on screen
    Centre();
    
    // Add minimal padding around grid
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->AddSpacer(10); // Top padding
    
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

    this->SetSizerAndFit(mainSizer);
}

void MainFrame::OnClose(wxCloseEvent& event) {
    if (event.CanVeto()) {
        if (wxMessageBox("Czy napewno chcesz wyjść?", "Proszę potwierdź", wxICON_QUESTION | wxYES_NO) != wxYES) {
            event.Veto();
            return;
        }
    }
    event.Skip();
}

void MainFrame::OnButtonClicked(wxCommandEvent& event) {
    wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
    if (!btn) return;
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
    }
}
