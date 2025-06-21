/**
 * @file Difficulty.h
 * @brief Definicje poziomów trudności oraz ich ustawień dla gry Saper.
 *
 * Plik zawiera enumerację poziomów trudności, strukturę przechowującą ustawienia
 * dla każdego poziomu oraz funkcje pomocnicze do pobierania tych ustawień.
 */

#pragma once

/**
 * @enum Difficulty
 * @brief Poziomy trudności gry.
 *
 * Określa dostępne poziomy trudności w grze Saper.
 */
enum class Difficulty {
    Easy,   /**< Łatwy poziom trudności */
    Medium, /**< Średni poziom trudności */
    Hard    /**< Trudny poziom trudności */
};

/**
 * @struct DifficultySettings
 * @brief Ustawienia dla różnych poziomów trudności.
 *
 * Przechowuje liczbę wierszy, kolumn oraz min dla danego poziomu trudności.
 */
struct DifficultySettings {
    int rows;   /**< Liczba wierszy planszy */
    int cols;   /**< Liczba kolumn planszy */
    int mines;  /**< Liczba min na planszy */
};

/**
 * @brief Ustawienia dla poziomu łatwego.
 */
inline const DifficultySettings EasySettings = { 9, 9, 10 };

/**
 * @brief Ustawienia dla poziomu średniego.
 */
inline const DifficultySettings MediumSettings = { 16, 16, 40 };

/**
 * @brief Ustawienia dla poziomu trudnego.
 */
inline const DifficultySettings HardSettings = { 16, 30, 99 };

/**
 * @brief Zwraca ustawienia dla danego poziomu trudności.
 *
 * @param difficulty Poziom trudności.
 * @return Stała referencja do ustawień odpowiadających wybranemu poziomowi trudności.
 */
inline const DifficultySettings& GetSettings(Difficulty difficulty) {
    switch (difficulty) {
        case Difficulty::Easy: return EasySettings;
        case Difficulty::Medium: return MediumSettings;
        case Difficulty::Hard: return HardSettings;
        default: return EasySettings; // Domyślnie zwraca ustawienia łatwe
    }
}
