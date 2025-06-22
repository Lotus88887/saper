#pragma once
#include <wx/colour.h>
#include <wx/font.h>

/**
 * @file UiUtil.h
 * @brief Deklaracje funkcji narzędziowych do obsługi kolorów i czcionek w interfejsie gry Saper.
 */

/**
 * @brief Zwraca kolor tekstu odpowiadający liczbie sąsiadujących min.
 *
 * Funkcja dobiera kolor na podstawie liczby, aby ułatwić rozróżnianie ilości min wokół pola.
 *
 * @param number Liczba sąsiadujących min (1-8).
 * @return Kolor tekstu dla danej liczby.
 */
wxColour GetNumberColor(int number);

/**
 * @brief Zwraca czcionkę przystosowaną do wyświetlania emoji.
 *
 * Funkcja próbuje wybrać czcionkę obsługującą emoji, odpowiednią do danego rozmiaru i wagi.
 *
 * @param size Rozmiar czcionki w punktach.
 * @param weight Waga czcionki (domyślnie pogrubiona).
 * @return Czcionka do wyświetlania emoji i tekstu na planszy.
 */
wxFont GetEmojiFont(int size, int weight = wxFONTWEIGHT_BOLD);
