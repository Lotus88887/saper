#pragma once
#include "MainFrame.h"

void MainFrame_OnButtonClicked(MainFrame* self, wxCommandEvent& event);
void MainFrame_OnButtonRightClick(MainFrame* self, wxMouseEvent& event);
void MainFrame_OnClose(MainFrame* self, wxCloseEvent& event);
void MainFrame_OnDifficultyChanged(MainFrame* self, wxCommandEvent& event);
void MainFrame_OnInfoButtonClicked(MainFrame* self, wxCommandEvent& event);
void MainFrame_OnTimer(MainFrame* self, wxTimerEvent& event);
