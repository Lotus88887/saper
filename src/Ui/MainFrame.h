#pragma once
#include <wx/wx.h>
#include <vector>

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
private:
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

    wxDECLARE_EVENT_TABLE();
};
