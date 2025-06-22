#include "UiLayout.h"
#include "UiUtil.h"
#include "BoardUI.h"
#include <wx/arrstr.h>

/**
 * @file UiLayout.cpp
 * @brief Implementacja funkcji pomocniczych do budowy i obsługi układu interfejsu gry Saper.
 */

/** @name Nagłówek interfejsu */
///@{
/**
 * @brief Tworzy panel nagłówka z licznikami, wyborem poziomu trudności i przyciskiem informacji.
 *
 * @param frame Wskaźnik do głównego okna gry.
 * @param emojiFont Referencja do czcionki emoji używanej w licznikach.
 * @return Wskaźnik do utworzonego panelu nagłówka.
 */
wxPanel* CreateHeaderPanel(MainFrame* frame, wxFont& emojiFont) {
    wxPanel* headerPanel = new wxPanel(frame, wxID_ANY);
    headerPanel->SetBackgroundColour(wxColour(50, 50, 50));
    wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);
    emojiFont = GetEmojiFont(12);
    wxStaticText* mineLabel = new wxStaticText(headerPanel, wxID_ANY, L"\U0001F4A3");
    mineLabel->SetForegroundColour(wxColour(255, 255, 255));
    mineLabel->SetFont(emojiFont);
    headerSizer->Add(mineLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    frame->m_mineCounter = new wxStaticText(headerPanel, wxID_ANY, std::to_wstring(frame->mines));
    frame->m_mineCounter->SetForegroundColour(wxColour(255, 255, 255));
    frame->m_mineCounter->SetFont(emojiFont);
    headerSizer->Add(frame->m_mineCounter, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
    wxArrayString diffChoices;
    diffChoices.Add("\u0141atwy");
    diffChoices.Add("\u015aredni");
    diffChoices.Add("Trudny");
    frame->m_difficultyCombo = new wxComboBox(headerPanel, wxID_ANY, diffChoices[0], wxDefaultPosition, wxDefaultSize, diffChoices, wxCB_READONLY);
    headerSizer->Add(frame->m_difficultyCombo, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
    frame->m_difficultyCombo->SetSelection(0);
    frame->m_difficultyCombo->Bind(wxEVT_COMBOBOX, &MainFrame::OnDifficultyChanged, frame);
    headerSizer->AddStretchSpacer();
    wxButton* infoButton = new wxButton(headerPanel, 1002, "i", wxDefaultPosition, wxSize(24, 24), wxBORDER_NONE);
    infoButton->SetForegroundColour(wxColour(255, 255, 255));
    infoButton->SetBackgroundColour(wxColour(80, 80, 80));
    wxFont infoFont = infoButton->GetFont();
    infoFont.SetWeight(wxFONTWEIGHT_BOLD);
    infoFont.SetPointSize(12);
    infoButton->SetFont(infoFont);
    headerSizer->Add(infoButton, 0, wxALIGN_CENTER_VERTICAL);
    headerSizer->AddStretchSpacer();
    wxStaticText* timerLabel = new wxStaticText(headerPanel, wxID_ANY, L"\u23f1\ufe0f ");
    timerLabel->SetForegroundColour(wxColour(255, 255, 255));
    timerLabel->SetFont(emojiFont);
    headerSizer->Add(timerLabel, 0, wxALIGN_CENTER_VERTICAL);
    frame->m_timeCounter = new wxStaticText(headerPanel, wxID_ANY, L"000");
    frame->m_timeCounter->SetForegroundColour(wxColour(255, 255, 255));
    frame->m_timeCounter->SetFont(emojiFont);
    headerSizer->Add(frame->m_timeCounter, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 10);
    headerPanel->SetSizer(headerSizer);
    return headerPanel;
}
///@}

/** @name Plansza gry */
///@{
/**
 * @brief Tworzy poziomy sizer na planszę gry i elementy pomocnicze.
 *
 * @param frame Wskaźnik do głównego okna gry.
 * @return Wskaźnik do utworzonego poziomego sizer'a.
 */
wxBoxSizer* CreateHorizontalSizer(MainFrame* frame) {
    wxBoxSizer* horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
    horizontalSizer->AddSpacer(10);
    frame->m_gridSizer = CreateBoardUI(frame, frame->rows, frame->cols, frame->buttons);
    horizontalSizer->Add(frame->m_gridSizer, 1, wxEXPAND);
    horizontalSizer->AddSpacer(10);
    return horizontalSizer;
}
///@}

/** @name Główny układ okna */
///@{
/**
 * @brief Tworzy główny sizer (układ pionowy) dla okna gry.
 *
 * @param frame Wskaźnik do głównego okna gry.
 * @param headerPanel Wskaźnik do panelu nagłówka.
 * @return Wskaźnik do utworzonego głównego sizer'a.
 */
wxBoxSizer* CreateMainSizer(MainFrame* frame, wxPanel* headerPanel) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->AddSpacer(10);
    mainSizer->Add(headerPanel, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
    mainSizer->AddSpacer(5);
    frame->m_horizontalSizer = CreateHorizontalSizer(frame);
    mainSizer->Add(frame->m_horizontalSizer, 1, wxEXPAND);
    mainSizer->AddSpacer(10);
    return mainSizer;
}
///@}

/** @name Bindowanie przycisków planszy */
///@{
/**
 * @brief Podpina obsługę zdarzeń do wszystkich przycisków siatki gry.
 *
 * @param frame Wskaźnik do głównego okna gry.
 */
void BindGridButtons(MainFrame* frame) {
    for (auto btn : frame->buttons) {
        btn->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, frame);
        btn->Bind(wxEVT_RIGHT_DOWN, &MainFrame::OnButtonRightClick, frame);
    }
}
///@}
