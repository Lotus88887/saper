#pragma once
#include <wx/wx.h>
#include <vector>

/**
 * @brief Tworzy siatkę przycisków dla planszy gry.
 *
 * @param parent Wskaźnik do rodzica (np. MainFrame).
 * @param rows Liczba wierszy.
 * @param cols Liczba kolumn.
 * @param buttons Wektor do przechowywania wskaźników do przycisków.
 * @return Wskaźnik do utworzonego wxGridSizer.
 */
inline wxGridSizer* CreateBoardUI(wxWindow* parent, int rows, int cols, std::vector<wxButton*>& buttons) {
    // Set a dark gray background for the main window
    parent->SetBackgroundColour(wxColour(50, 50, 50));
    
    // Use FlexGridSizer with small gaps for a cleaner look
    wxGridSizer* gridSizer = new wxGridSizer(rows, cols, 2, 2); // 2px gap between buttons
    buttons.resize(rows * cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Create button with no border style and modern flat appearance
            wxButton* myButton = new wxButton(parent, wxID_ANY, "", 
                                             wxDefaultPosition, 
                                             wxSize(36, 36), // Slightly larger buttons for better touch targets
                                             wxBORDER_NONE);
            
            // Set modern flat appearance with light gray background
            myButton->SetBackgroundColour(wxColour(220, 220, 220)); // Light gray
            
            // Set modern font
            wxFont font = myButton->GetFont();
            font.SetPointSize(10);
            font.SetFamily(wxFONTFAMILY_DEFAULT);
            font.SetWeight(wxFONTWEIGHT_BOLD);
            myButton->SetFont(font);
            
            // Add to grid with uniform spacing
            gridSizer->Add(myButton, 0, wxEXPAND);
            buttons[i * cols + j] = myButton;
        }
    }
    return gridSizer;
}
