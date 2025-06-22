#include "UiUtil.h"
#include <wx/font.h>
#include "MainFrame.h"

wxColour GetNumberColor(int number) {
    switch (number) {
    case 1: return wxColour(33, 150, 243);
    case 2: return wxColour(76, 175, 80);
    case 3: return wxColour(244, 67, 54);
    case 4: return wxColour(156, 39, 176);
    case 5: return wxColour(255, 152, 0);
    case 6: return wxColour(0, 188, 212);
    case 7: return wxColour(96, 125, 139);
    case 8: return wxColour(121, 85, 72);
    default: return wxColour(66, 66, 66);
    }
}

wxFont GetEmojiFont(int size, int weight) {
    wxFontInfo info(size);
    info.Family(wxFONTFAMILY_DEFAULT).Style(wxFONTSTYLE_NORMAL).Weight(weight);
    const char* fontNames[] = {
        "Segoe UI Emoji",
        "Noto Color Emoji",
        "Apple Color Emoji",
        "Arial Unicode MS",
        "Segoe UI Symbol",
        "Symbola"
    };
    for (const char* name : fontNames) {
        info.FaceName(name);
        wxFont font(info);
        if (font.IsOk()) {
            return font;
        }
    }
    return wxFont(info);
}

void MainFrame::ResetUI() {
    for (auto btn : buttons) {
        btn->SetLabel(L"");
        btn->SetBackgroundColour(wxColour(220, 220, 220));
        btn->SetForegroundColour(wxNullColour);
        wxFont font = GetEmojiFont(10);
        font.SetWeight(wxFONTWEIGHT_NORMAL);
        btn->SetFont(font);
        btn->Unbind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
        btn->Unbind(wxEVT_RIGHT_DOWN, &MainFrame::OnButtonRightClick, this);
        btn->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
        btn->Bind(wxEVT_RIGHT_DOWN, &MainFrame::OnButtonRightClick, this);
    }
    if (m_timer->IsRunning()) {
        m_timer->Stop();
    }
    m_seconds = 0;
    m_timeCounter->SetLabel(L"000");
    m_remainingMines = mines;
    m_mineCounter->SetLabel(std::to_wstring(m_remainingMines));
}

void MainFrame::UpdateMineCounter() {
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
