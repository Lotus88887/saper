#pragma once
#include <wx/wx.h>
#include <wx/sizer.h>
#include <vector>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/font.h>
#include "MainFrame.h"

/**
 * @file UiLayout.h
 * @brief Deklaracje funkcji pomocniczych do budowy i obsługi układu interfejsu gry Saper.
 */

/**
 * @brief Tworzy panel nagłówka z licznikami i wyborem poziomu trudności.
 *
 * @param frame Wskaźnik do głównego okna gry.
 * @param emojiFont Referencja do czcionki emoji używanej w licznikach.
 * @return Wskaźnik do utworzonego panelu nagłówka.
 */
wxPanel* CreateHeaderPanel(MainFrame* frame, wxFont& emojiFont);

/**
 * @brief Tworzy główny sizer (układ pionowy) dla okna gry.
 *
 * @param frame Wskaźnik do głównego okna gry.
 * @param headerPanel Wskaźnik do panelu nagłówka.
 * @return Wskaźnik do utworzonego głównego sizer'a.
 */
wxBoxSizer* CreateMainSizer(MainFrame* frame, wxPanel* headerPanel);

/**
 * @brief Tworzy poziomy sizer na planszę gry i elementy pomocnicze.
 *
 * @param frame Wskaźnik do głównego okna gry.
 * @return Wskaźnik do utworzonego poziomego sizer'a.
 */
wxBoxSizer* CreateHorizontalSizer(MainFrame* frame);

/**
 * @brief Podpina obsługę zdarzeń do wszystkich przycisków siatki gry.
 *
 * @param frame Wskaźnik do głównego okna gry.
 */
void BindGridButtons(MainFrame* frame);
