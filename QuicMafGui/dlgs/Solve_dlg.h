#ifndef SOLVE_DLG_H
#define SOLVE_DLG_H
#pragma once
#include <wx/wx.h>

#include "qm.h"

class Solve : public wxDialog {
public:
	Solve(wxWindow* pID, LOG* log = nullptr);
	void CreateControls();
	void SizeControls();

private:
	wxDECLARE_EVENT_TABLE();

	wxPanel *mPanel = nullptr;

	wxTextCtrl* mInput = nullptr;
	wxListBox* mOutput = nullptr;

	void OnInputChange(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);

	Solver* solver = nullptr;
	LOG* log = nullptr;
};

enum {
	solve_panel,
	solve_input,
	solve_output
};

#endif // !SOLVE_DLG
