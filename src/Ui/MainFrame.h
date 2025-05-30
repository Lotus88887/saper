#pragma once
#include <wx/wx.h>
#include <vector>
#include "../Core/Board.h"

/**
 * @class MainFrame
 * @brief Główne okno gry Saper.
 *
 * Odpowiada za utworzenie siatki gry oraz obsługę interakcji użytkownika.
 */
class MainFrame : public wxFrame
{
public:
    /**
     * @brief Konstruktor głównego okna i inicjalizacja siatki gry.
     */
    MainFrame();
    

    int rows = 9;
    int cols = 9;
    int mines = 10;

private:
    void ResetUI();

    /**
     * @brief Obsługuje zdarzenie zamknięcia głównego okna.
     * @param event Obiekt zdarzenia zamknięcia.
     */
    void OnClose(wxCloseEvent& event);

    /**
     * @brief Obsługuje kliknięcie lewym przyciskiem myszy na przycisku siatki.
     * @param event Obiekt zdarzenia polecenia.
     */
    void OnButtonClicked(wxCommandEvent& event);

    /**
     * @brief Obsługuje kliknięcie prawym przyciskiem myszy na przycisku siatki.
     * @param event Obiekt zdarzenia myszy.
     */
    void OnButtonRightClick(wxMouseEvent& event);

    /**
     * @brief Przechowuje wskaźniki do wszystkich przycisków siatki.
     */
    std::vector<wxButton*> buttons;
    
    /**
     * @brief Logika planszy gry Saper.
     */
    Board board{ rows, cols, mines };
    
    wxDECLARE_EVENT_TABLE();
};

