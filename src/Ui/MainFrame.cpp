#include "MainFrame.h"
#include "BoardUI.h"


wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_CLOSE(MainFrame::OnClose)

wxEND_EVENT_TABLE()

wxColour GetNumberColor(int number) {
    switch (number) {
    case 1: return wxColour(0, 0, 255);      // Blue
    case 2: return wxColour(0, 128, 0);      // Green
    case 3: return wxColour(255, 0, 0);      // Red
    case 4: return wxColour(0, 0, 128);      // Dark Blue
    case 5: return wxColour(128, 0, 0);      // Dark Red
    case 6: return wxColour(0, 128, 128);    // Teal
    case 7: return wxColour(0, 0, 0);        // Black
    case 8: return wxColour(128, 128, 128);  // Gray
    default: return *wxBLACK;
    }
}

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Saper")
{
    Centre();

    wxGridSizer* gridSizer = CreateBoardUI(this, rows, cols, buttons);

    for (auto btn : buttons) {
        btn->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
        btn->Bind(wxEVT_RIGHT_DOWN, &MainFrame::OnButtonRightClick, this);
    }

    this->SetSizerAndFit(gridSizer);
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
        btn->SetLabel("X");

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
                    b->SetLabel("X");
                }
                else if (cell.adjacent > 0) {
                    b->SetLabel(std::to_string(cell.adjacent));
                    b->SetForegroundColour(GetNumberColor(cell.adjacent));
                    // Set font: bold, size 12, default family
                    wxFont font = b->GetFont();
                    font.SetWeight(wxFONTWEIGHT_BOLD);
                    font.SetPointSize(10);
                    b->SetFont(font);
                }
                else {
                    b->SetLabel("");
                    b->SetForegroundColour(wxNullColour);
                    wxFont font = b->GetFont();
                    font.SetWeight(wxFONTWEIGHT_NORMAL);
                    font.SetPointSize(10); // Default size
                    b->SetFont(font);
                }
                b->Enable(false);
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
        //wxMessageBox("Right button was clicked!");
        int idx = std::find(buttons.begin(), buttons.end(), btn) - buttons.begin();
        int row = idx / cols, col = idx % cols;
        board.ToggleFlag(row, col);

        const auto& cell = board.GetCell(row, col);

        if (cell.state == Board::CellState::Flagged) {
            wxFont font = btn->GetFont();
            font.SetWeight(wxFONTWEIGHT_BOLD);
            font.SetPointSize(12);
            btn->SetFont(font);
            btn->SetLabel("F");
        }
        else if (cell.state == Board::CellState::Hidden) {
            wxFont font = btn->GetFont();
            font.SetWeight(wxFONTWEIGHT_BOLD);
            font.SetPointSize(12);
            btn->SetFont(font);
            btn->SetLabel("");
        }
    }
}

void MainFrame::ResetUI() {
    for (auto btn : buttons) {
        btn->SetLabel("");
        btn->Enable(true);
        btn->SetForegroundColour(wxNullColour);
        wxFont font = btn->GetFont();
        font.SetWeight(wxFONTWEIGHT_NORMAL);
        font.SetPointSize(10);
        btn->SetFont(font);
    }
}
