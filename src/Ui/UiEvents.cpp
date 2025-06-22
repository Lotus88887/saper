#include "UiEvents.h"
#include "UiUtil.h"
#include "BoardUI.h"
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <algorithm>

void MainFrame_OnInfoButtonClicked(MainFrame* self, wxCommandEvent& WXUNUSED(event)) {
    wxString message =  L"Saper\n\n"
                        L"Stworzone przez:\n"
                        L"(Autor - Zainteresowania)\n"
                        L"Kacper Ka\u0142u\u017ca - design interfejs\u00f3w\n"
                        L"Przemys\u0142aw B\u0142aszczyk - gry wideo i fikcja naukowa\n"
                        L"Mateusz Biskup - \n\n"
                        L"Studenci Politechniki \u015awi\u0119tokrzyskiej - Informatyka I Stopnia\n\n"
                        L"\u00a9 2025 All Rights Reserved";
    wxMessageDialog dialog(self, message, L"About Minesweeper", wxOK | wxICON_INFORMATION);
    dialog.ShowModal();
}

void MainFrame_OnTimer(MainFrame* self, wxTimerEvent& WXUNUSED(event)) {
    self->m_seconds++;
    self->m_timeCounter->SetLabel(wxString::Format(L"%03d", std::min(self->m_seconds, 999)));
}

void MainFrame_OnClose(MainFrame* self, wxCloseEvent& event) {
    if (event.CanVeto()) {
        if (wxMessageBox(L"Czy napewno chcesz wyj\u015b\u0107?", L"Prosz\u0119 potwierd\u017a", wxICON_QUESTION | wxYES_NO) != wxYES) {
            event.Veto();
            return;
        }
    }
    if (self->m_timer && self->m_timer->IsRunning()) {
        self->m_timer->Stop();
    }
    event.Skip();
}

void MainFrame_OnButtonClicked(MainFrame* self, wxCommandEvent& event) {
    wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
    if (!btn) return;
    if (!self->m_timer->IsRunning()) {
        self->m_timer->Start(1000);
    }
    int idx = std::find(self->buttons.begin(), self->buttons.end(), btn) - self->buttons.begin();
    int row = idx / self->cols, col = idx % self->cols;
    if (self->board.Reveal(row, col)) {
        wxFont font = GetEmojiFont(12);
        btn->SetFont(font);
        btn->SetLabel(L"\U0001F4A3");
        btn->SetForegroundColour(wxColour(244, 67, 54));
        btn->SetBackgroundColour(wxColour(200, 200, 200));
        if (self->m_timer->IsRunning()) {
            self->m_timer->Stop();
        }
        wxMessageBox(L"Przegr\u0142e\u015b!", L"Koniec gry");
        self->board.Reset();
        self->ResetUI();
        return;
    }
    for (int r = 0; r < self->rows; ++r)
        for (int c = 0; c < self->cols; ++c) {
            const auto& cell = self->board.GetCell(r, c);
            wxButton* b = self->buttons[r * self->cols + c];
            if (cell.state == Board::Revealed) {
                if (cell.mine) {
                    wxFont font = GetEmojiFont(12);
                    b->SetFont(font);
                    b->SetLabel(L"\U0001F4A3");
                    b->SetForegroundColour(wxColour(244, 67, 54));
                }
                else if (cell.adjacent > 0) {
                    b->SetLabel(std::to_wstring(cell.adjacent));
                    b->SetForegroundColour(GetNumberColor(cell.adjacent));
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
                b->SetBackgroundColour(wxColour(255, 255, 255));
                b->Unbind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, self);
                b->Unbind(wxEVT_RIGHT_DOWN, &MainFrame::OnButtonRightClick, self);
            }
        }
    if (self->board.IsWin()) {
        if (self->m_timer->IsRunning()) {
            self->m_timer->Stop();
        }
        wxMessageBox(L"Wygra\u0142e\u015b!", L"Koniec gry");
        self->board.Reset();
        self->ResetUI();
    }
}

void MainFrame_OnButtonRightClick(MainFrame* self, wxMouseEvent& event) {
    wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
    if (btn) {
        int idx = std::find(self->buttons.begin(), self->buttons.end(), btn) - self->buttons.begin();
        int row = idx / self->cols, col = idx % self->cols;
        self->board.ToggleFlag(row, col);
        const auto& cell = self->board.GetCell(row, col);
        if (cell.state == Board::CellState::Flagged) {
            wxFont font = GetEmojiFont(12);
            btn->SetFont(font);
            btn->SetLabel(L"\U0001F6A9");
            btn->SetForegroundColour(wxColour(244, 67, 54));
            btn->SetBackgroundColour(wxColour(240, 240, 240));
        }
        else if (cell.state == Board::CellState::Hidden) {
            wxFont font = GetEmojiFont(12);
            font.SetWeight(wxFONTWEIGHT_BOLD);
            btn->SetFont(font);
            btn->SetLabel(L"");
            btn->SetForegroundColour(wxNullColour);
            btn->SetBackgroundColour(wxColour(220, 220, 220));
        }
        self->UpdateMineCounter();
    }
}

void MainFrame_OnDifficultyChanged(MainFrame* self, wxCommandEvent& event) {
    int sel = self->m_difficultyCombo->GetSelection();
    switch (sel) {
    case 0: self->m_difficulty = Difficulty::Easy; break;
    case 1: self->m_difficulty = Difficulty::Medium; break;
    case 2: self->m_difficulty = Difficulty::Hard; break;
    default: self->m_difficulty = Difficulty::Easy; break;
    }
    auto settings = GetSettings(self->m_difficulty);
    self->rows = settings.rows;
    self->cols = settings.cols;
    self->mines = settings.mines;
    self->board = Board(self->rows, self->cols, self->mines);
    for (auto btn : self->buttons) {
        btn->Destroy();
    }
    self->buttons.clear();
    if (self->m_gridSizer) {
        self->m_gridSizer->Clear(true);
    }
    self->m_gridSizer = CreateBoardUI(self, self->rows, self->cols, self->buttons);
    if (!self->m_horizontalSizer) {
        wxBoxSizer* mainSizer = static_cast<wxBoxSizer*>(self->GetSizer());
        if (mainSizer) {
            wxSizerItemList& items = mainSizer->GetChildren();
            for (auto item : items) {
                self->m_horizontalSizer = dynamic_cast<wxBoxSizer*>(item->GetSizer());
                if (self->m_horizontalSizer) break;
            }
        }
    }
    if (self->m_horizontalSizer) {
        self->m_horizontalSizer->Clear(false);
        self->m_horizontalSizer->AddSpacer(10);
        self->m_horizontalSizer->Add(self->m_gridSizer, 1, wxEXPAND);
        self->m_horizontalSizer->AddSpacer(10);
    }
    for (auto btn : self->buttons) {
        btn->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, self);
        btn->Bind(wxEVT_RIGHT_DOWN, &MainFrame::OnButtonRightClick, self);
    }
    self->Layout();
    self->Fit();
    self->ResetUI();
}
