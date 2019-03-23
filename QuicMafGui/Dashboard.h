#ifndef DASHBOARD_H
#define DASHBOARD_H
#pragma once
#include <wx/wx.h>

#include "dlgs/evaluator.h"
#include "dlgs/Solve_dlg.h"

class Dashboard : public wxFrame {
public:
	Dashboard(LOG* log = nullptr);
	void CreateControls();
	void SizeControls();


private:
	wxDECLARE_EVENT_TABLE();

	wxPanel* mPanel = nullptr;
	wxButton* mEvalPg = nullptr;
	wxButton* mSolverPg = nullptr;

	void OnEvalBtn(wxCommandEvent& evt);
	void OnSolverBtn(wxCommandEvent& evt);
	LOG* log = nullptr;
};

enum {
	dsh_panel,
	dsh_eval_btn,
	dsh_solver_btn
};

#endif // !DASHBOARD_H
