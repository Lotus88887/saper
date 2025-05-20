#include <wx/wx.h>
#include <vector>

inline wxGridSizer* CreateBoardUI(wxWindow* parent, int rows, int cols, std::vector<wxButton*>& buttons) {
    wxGridSizer* gridSizer = new wxGridSizer(rows, cols, 2, 2);
    buttons.resize(rows * cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            wxButton* myButton = new wxButton(parent, wxID_ANY, "", wxDefaultPosition, wxSize(30, 30));
            gridSizer->Add(myButton, 0, wxEXPAND);
            buttons[i * cols + j] = myButton;
        }
    }
    return gridSizer;
}