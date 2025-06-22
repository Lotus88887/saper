#pragma once
#include <wx/wx.h>

class MainApp : public wxApp
{
public:
    /**
     * @brief Inicjalizuje aplikację główną.
     *
     * Metoda wywoływana przy starcie aplikacji. Tworzy główne okno gry.
     * @return true jeśli inicjalizacja powiodła się, false w przeciwnym razie.
     */
    virtual bool OnInit() override;
};
