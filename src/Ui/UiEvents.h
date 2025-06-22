#pragma once
#include "MainFrame.h"

/**
 * @file UiEvents.h
 * @brief Deklaracje funkcji obsługujących zdarzenia interfejsu użytkownika w grze Saper.
 */

/**
 * @brief Obsługuje kliknięcie lewym przyciskiem myszy na przycisku siatki gry.
 *
 * Wywoływana po kliknięciu przycisku na planszy. Odpowiada za odkrywanie pól, 
 * aktualizację interfejsu oraz sprawdzenie warunków zakończenia gry.
 *
 * @param self Wskaźnik do głównego okna gry.
 * @param event Obiekt zdarzenia kliknięcia.
 */
void MainFrame_OnButtonClicked(MainFrame* self, wxCommandEvent& event);

/**
 * @brief Obsługuje kliknięcie prawym przyciskiem myszy na przycisku siatki gry.
 *
 * Pozwala na oznaczanie i odznaczanie pól jako miny (flagi).
 *
 * @param self Wskaźnik do głównego okna gry.
 * @param event Obiekt zdarzenia myszy.
 */
void MainFrame_OnButtonRightClick(MainFrame* self, wxMouseEvent& event);

/**
 * @brief Obsługuje zdarzenie zamknięcia głównego okna gry.
 *
 * Pozwala na potwierdzenie wyjścia oraz zatrzymuje timer.
 *
 * @param self Wskaźnik do głównego okna gry.
 * @param event Obiekt zdarzenia zamknięcia.
 */
void MainFrame_OnClose(MainFrame* self, wxCloseEvent& event);

/**
 * @brief Obsługuje zmianę poziomu trudności przez użytkownika.
 *
 * Aktualizuje parametry planszy i resetuje interfejs.
 *
 * @param self Wskaźnik do głównego okna gry.
 * @param event Obiekt zdarzenia wyboru z listy.
 */
void MainFrame_OnDifficultyChanged(MainFrame* self, wxCommandEvent& event);

/**
 * @brief Obsługuje kliknięcie przycisku informacji.
 *
 * Wyświetla okno z informacjami o grze i autorach.
 *
 * @param self Wskaźnik do głównego okna gry.
 * @param event Obiekt zdarzenia kliknięcia.
 */
void MainFrame_OnInfoButtonClicked(MainFrame* self, wxCommandEvent& event);

/**
 * @brief Obsługuje zdarzenie timera gry.
 *
 * Aktualizuje licznik czasu gry na interfejsie.
 *
 * @param self Wskaźnik do głównego okna gry.
 * @param event Obiekt zdarzenia timera.
 */
void MainFrame_OnTimer(MainFrame* self, wxTimerEvent& event);
