#ifndef EVALUATOR_H
#define EVALUATOR_H
#pragma once
#include <wx/wx.h>

#include "qm.h"

class Evaluator : public wxDialog {
public:
	Evaluator(wxWindow* pId, LOG* _log = nullptr);
	void CreateControls();
	void SizeControls();

private:
	wxDECLARE_EVENT_TABLE();
	wxPanel* mPanel = nullptr;
	
	wxTextCtrl *mInput = nullptr;
	wxListBox *mOutput = nullptr;

	void OnInputChange(wxCommandEvent& evt);
	void OnClose(wxCloseEvent& evt);

	bool CheckExpr(string str);
	Solver* solver = nullptr;
	LOG* log = nullptr;
};

enum {
	eval_panel,
	eval_input,
	eval_output,
};

#endif // !EVALUATOR_H
