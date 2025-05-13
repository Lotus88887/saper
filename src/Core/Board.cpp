#include "Board.h"

Board::Board(int r, int c, int m) : rows(r), cols(c), mines(m) {
    Reset();
}

void Board::Reset() {
    grid.assign(rows, std::vector<Cell>(cols));
    firstMove = true;
}

bool Board::Reveal(int row, int col) {
    if (!InBounds(row, col)) return false;
    if (grid[row][col].state == Revealed || grid[row][col].state == Flagged) return false;

    if (firstMove) {
        PlaceMines(row, col);
        CountAdjacents();
        firstMove = false;
    }

    grid[row][col].state = Revealed;
    if (grid[row][col].mine) return true; // trafiono minę

    if (grid[row][col].adjacent == 0)
        RevealEmpty(row, col);

    return false;
}


void Board::ToggleFlag(int row, int col) {
    if (!InBounds(row, col)) return;
    if (grid[row][col].state == Revealed) return;
    grid[row][col].state = (grid[row][col].state == Flagged) ? Hidden : Flagged;
}

const Board::Cell& Board::GetCell(int row, int col) const {
    return grid[row][col];
}

bool Board::IsWin() const {
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            if (!grid[r][c].mine && grid[r][c].state != Revealed)
                return false;
    return true;
}

void Board::PlaceMines(int safeRow, int safeCol) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<std::pair<int, int>> positions;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            if (!(r == safeRow && c == safeCol))
                positions.emplace_back(r, c);

    std::shuffle(positions.begin(), positions.end(), gen);
    for (int i = 0; i < mines && i < (int)positions.size(); ++i)
        grid[positions[i].first][positions[i].second].mine = true;
}

void Board::CountAdjacents() {
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c].mine) continue;
            int count = 0;
            for (int dr = -1; dr <= 1; ++dr)
                for (int dc = -1; dc <= 1; ++dc) {
                    int nr = r + dr, nc = c + dc;
                    if (InBounds(nr, nc) && grid[nr][nc].mine)
                        ++count;
                }
            grid[r][c].adjacent = count;
        }
}

void Board::RevealEmpty(int row, int col) {
    for (int dr = -1; dr <= 1; ++dr)
        for (int dc = -1; dc <= 1; ++dc) {
            int nr = row + dr, nc = col + dc;
            if (InBounds(nr, nc) && grid[nr][nc].state == Hidden && !grid[nr][nc].mine) {
                grid[nr][nc].state = Revealed;
                if (grid[nr][nc].adjacent == 0)
                    RevealEmpty(nr, nc);
            }
        }
}

bool Board::InBounds(int row, int col) const {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}
