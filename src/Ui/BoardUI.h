#pragma once
#include <wx/wx.h>
#include <vector>

/**
 * @file BoardUI.h
 * @brief Zawiera funkcję pomocniczą do tworzenia interfejsu planszy gry w Saperze.
 */

/**
 * @brief Tworzy siatkę przycisków reprezentujących planszę gry.
 *
 * Funkcja generuje siatkę przycisków o zadanej liczbie wierszy i kolumn, 
 * ustawia ich wygląd oraz dodaje do przekazanego wektora wskaźników. 
 * Każdy przycisk posiada nowoczesny wygląd oraz odpowiednią czcionkę.
 *
 * @param parent Wskaźnik do okna rodzica (np. MainFrame).
 * @param rows Liczba wierszy planszy.
 * @param cols Liczba kolumn planszy.
 * @param buttons Referencja do wektora, w którym zostaną zapisane wskaźniki do utworzonych przycisków.
 * @return Wskaźnik do utworzonego obiektu wxGridSizer, zawierającego przyciski planszy.
 */
inline wxGridSizer* CreateBoardUI(wxWindow* parent, int rows, int cols, std::vector<wxButton*>& buttons) {
    // Ustawienie ciemnoszarego tła dla głównego okna
    parent->SetBackgroundColour(wxColour(50, 50, 50));
    
    // Utworzenie siatki z odstępami 2px między przyciskami
    wxGridSizer* gridSizer = new wxGridSizer(rows, cols, 2, 2);
    buttons.resize(rows * cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Utworzenie przycisku bez obramowania, o nowoczesnym wyglądzie
            wxButton* myButton = new wxButton(parent, wxID_ANY, "", 
                                             wxDefaultPosition, 
                                             wxSize(36, 36),
                                             wxBORDER_NONE);
            
            // Ustawienie jasnoszarego tła przycisku
            myButton->SetBackgroundColour(wxColour(220, 220, 220));
            
            // Ustawienie nowoczesnej, pogrubionej czcionki
            wxFont font = myButton->GetFont();
            font.SetPointSize(10);
            font.SetFamily(wxFONTFAMILY_DEFAULT);
            font.SetWeight(wxFONTWEIGHT_BOLD);
            myButton->SetFont(font);
            
            // Dodanie przycisku do siatki
            gridSizer->Add(myButton, 0, wxEXPAND);
            buttons[i * cols + j] = myButton;
        }
    }
    return gridSizer;
}
