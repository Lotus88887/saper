#pragma once
#include <wx/wx.h>

/**
 * @class MainApp
 * @brief Główna klasa aplikacji gry Saper.
 *
 * Dziedziczy po wxApp i inicjalizuje główne okno.
 */
class MainApp : public wxApp
{
public:
    /**
     * @brief Inicjalizuje aplikację.
     * @return true jeśli inicjalizacja zakończyła się sukcesem, w przeciwnym razie false.
     */
    virtual bool OnInit();
};
