#include "Dashboard.h"

Dashboard::Dashboard(LOG* _log) :
	wxFrame(nullptr, wxID_ANY, "QuicMaf", wxDefaultPosition,
		wxSize(230, 200), wxDEFAULT_FRAME_STYLE & ~wxMAXIMIZE_BOX & ~wxMINIMIZE_BOX) {
	this->SetMinSize(this->GetSize());
	this->SetMaxSize(this->GetSize());
	CreateControls();
	SizeControls();
	Center();
	log = _log;
}

void Dashboard::CreateControls() {
	mPanel = new wxPanel(this, dsh_panel);

	mEvalPg = new wxButton(mPanel, dsh_eval_btn, "Evaluate");
	mSolverPg = new wxButton(mPanel, dsh_solver_btn, "Solver");
}

void Dashboard::SizeControls() {

	mEvalPg->SetSize(wxSize(
		GetClientSize().x-4,
		(GetClientSize().y / 2) - 4
	));

	mSolverPg->SetSize(wxSize(
		GetClientSize().x-4,
		(GetClientSize().y / 2) - 4
	));

	mEvalPg->SetPosition(
		wxPoint(
			(GetClientSize().x/2)-(mEvalPg->GetSize().x/2),
			0.15*GetClientSize().y
		)
	);

	mEvalPg->SetPosition(wxPoint(2, 0));

	mSolverPg->SetPosition(wxPoint(
		2,
		mEvalPg->GetSize().y + 4
	));
}

void Dashboard::OnEvalBtn(wxCommandEvent & evt) {
	this->Hide();
	Evaluator *eval = new Evaluator(this, log);
	eval->Show(true);
}

void Dashboard::OnSolverBtn(wxCommandEvent & evt) {
	this->Hide();
	Solve* solve = new Solve(this, log);
	solve->Show(true);
}


BEGIN_EVENT_TABLE(Dashboard, wxFrame)

EVT_BUTTON(dsh_eval_btn, Dashboard::OnEvalBtn)
EVT_BUTTON(dsh_solver_btn, Dashboard::OnSolverBtn)

END_EVENT_TABLE()