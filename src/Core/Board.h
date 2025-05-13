#pragma once
#include <vector>
#include <random>

/**
 * @brief Logika planszy gry Saper.
 */
class Board {
public:
    /**
     * @enum CellState
     * @brief Stan pojedynczego pola na planszy.
     */
    enum CellState { Hidden,    ///< Pole ukryte
                     Revealed,  ///< Pole odkryte
                     Flagged    ///< Pole oznaczone flagą
    };

    /**
     * @struct Cell
     * @brief Reprezentuje pojedyncze pole na planszy.
     */
    struct Cell {
        bool mine = false;         ///< Czy pole zawiera minę
        int adjacent = 0;         ///< Liczba sąsiadujących min
        CellState state = Hidden; ///< Aktualny stan pola
    };

    /**
     * @brief Konstruktor planszy gry Saper.
     * @param rows Liczba wierszy.
     * @param cols Liczba kolumn.
     * @param mines Liczba min.
     */
    Board(int rows, int cols, int mines);

    /**
     * @brief Resetuje planszę do stanu początkowego.
     */
    void Reset();

    /**
     * @brief Odkrywa pole na planszy.
     * @param row Wiersz pola.
     * @param col Kolumna pola.
     * @return true jeśli trafiono minę, false w przeciwnym razie.
     */
    bool Reveal(int row, int col);

    /**
     * @brief Przełącza flagę na polu (oznacza/odznacza jako mina).
     * @param row Wiersz pola.
     * @param col Kolumna pola.
     */
    void ToggleFlag(int row, int col);

    /**
     * @brief Zwraca referencję do komórki planszy.
     * @param row Wiersz.
     * @param col Kolumna.
     * @return Stała referencja do komórki.
     */
    const Cell& GetCell(int row, int col) const;

    /**
     * @brief Zwraca liczbę wierszy planszy.
     * @return Liczba wierszy.
     */
    int GetRows() const { return rows; }

    /**
     * @brief Zwraca liczbę kolumn planszy.
     * @return Liczba kolumn.
     */
    int GetCols() const { return cols; }

    /**
     * @brief Sprawdza, czy gracz wygrał grę.
     * @return true jeśli wszystkie nie-minowe pola są odkryte, false w przeciwnym razie.
     */
    bool IsWin() const;

private:
    int rows;   ///< Liczba wierszy planszy
    int cols;   ///< Liczba kolumn planszy
    int mines;  ///< Liczba min na planszy
    std::vector<std::vector<Cell>> grid; ///< Dwuwymiarowa siatka pól
    bool firstMove = true; ///< Czy to pierwszy ruch gracza

    /**
     * @brief Rozstawia miny na planszy, omijając pierwsze kliknięte pole.
     * @param safeRow Wiersz bezpiecznego pola.
     * @param safeCol Kolumna bezpiecznego pola.
     */
    void PlaceMines(int safeRow, int safeCol);

    /**
     * @brief Oblicza liczbę sąsiadujących min dla każdego pola.
     */
    void CountAdjacents();

    /**
     * @brief Rekurencyjnie odkrywa puste pola sąsiadujące z danym polem.
     * @param row Wiersz pola.
     * @param col Kolumna pola.
     */
    void RevealEmpty(int row, int col);

    /**
     * @brief Sprawdza, czy podane współrzędne znajdują się w granicach planszy.
     * @param row Wiersz.
     * @param col Kolumna.
     * @return true jeśli pole jest w granicach planszy, false w przeciwnym razie.
     */
    bool InBounds(int row, int col) const;
};
