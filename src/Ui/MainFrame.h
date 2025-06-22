#pragma once
#include <wx/wx.h>
#include <vector>
#include "../Core/Board.h"
#include "../Core/Difficulty.h"

/**
 * @file MainFrame.h
 * @brief Deklaracja klasy głównego okna gry Saper oraz jej metod i pól.
 */

/**
 * @class MainFrame
 * @brief Główne okno gry Saper.
 *
 * Odpowiada za utworzenie siatki gry, obsługę interakcji użytkownika oraz zarządzanie elementami interfejsu.
 */
class MainFrame : public wxFrame
{
public:
    /**
     * @brief Konstruktor głównego okna i inicjalizacja siatki gry.
     */
    MainFrame();
    
    /**
     * @brief Liczba wierszy planszy.
     */
    int rows = 9;
    /**
     * @brief Liczba kolumn planszy.
     */
    int cols = 9;
    /**
     * @brief Liczba min na planszy.
     */
    int mines = 10;

    /**
     * @brief Resetuje interfejs planszy do stanu początkowego.
     */
    void ResetUI();
    
    /**
     * @brief Aktualizuje licznik min.
     */
    void UpdateMineCounter();
    
    /**
     * @brief Obsługuje zdarzenie timera.
     * @param event Obiekt zdarzenia timera.
     */
    void OnTimer(wxTimerEvent& event);
    
    /**
     * @brief Obsługuje kliknięcie przycisku informacji.
     * @param event Obiekt zdarzenia kliknięcia.
     */
    void OnInfoButtonClicked(wxCommandEvent& event);

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
     * @brief Obsługuje zmianę poziomu trudności przez użytkownika.
     * @param event Obiekt zdarzenia wyboru z listy.
     */
    void OnDifficultyChanged(wxCommandEvent& event);

    /**
     * @brief Przechowuje wskaźniki do wszystkich przycisków siatki.
     */
    std::vector<wxButton*> buttons;
    
    /**
     * @brief Logika planszy gry Saper.
     */
    Board board{ rows, cols, mines };
    
    /**
     * @brief Timer odliczający czas gry.
     */
    wxTimer* m_timer;
    
    /**
     * @brief Licznik upłyniętych sekund.
     */
    int m_seconds;
    
    /**
     * @brief Liczba pozostałych min (całkowita liczba min - liczba flag).
     */
    int m_remainingMines;
    
    /**
     * @brief Wyświetlacz liczby pozostałych min.
     */
    wxStaticText* m_mineCounter;
    
    /**
     * @brief Wyświetlacz czasu gry.
     */
    wxStaticText* m_timeCounter;

    /**
     * @brief Lista wyboru poziomu trudności.
     */
    wxComboBox* m_difficultyCombo;

    /**
     * @brief Aktualny poziom trudności.
     */
    Difficulty m_difficulty = Difficulty::Easy;

    /**
     * @brief Układ siatki dla przycisków.
     */
    wxGridSizer* m_gridSizer = nullptr;

    /**
     * @brief Układ poziomy dla elementów interfejsu.
     */
    wxBoxSizer* m_horizontalSizer = nullptr;

    wxDECLARE_EVENT_TABLE();
};

