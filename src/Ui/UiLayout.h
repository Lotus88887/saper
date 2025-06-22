#pragma once
#include <wx/wx.h>
#include <wx/sizer.h>
#include <vector>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/font.h>
#include "MainFrame.h"

wxPanel* CreateHeaderPanel(MainFrame* frame, wxFont& emojiFont);
wxBoxSizer* CreateMainSizer(MainFrame* frame, wxPanel* headerPanel);
wxBoxSizer* CreateHorizontalSizer(MainFrame* frame);
void BindGridButtons(MainFrame* frame);
