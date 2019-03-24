#include "Solve_dlg.h"

Solve::Solve(wxWindow* pID, LOG* _log)
	: wxDialog(pID, wxID_ANY,
		"QuicMaf | Solver", wxDefaultPosition,
		wxSize(400, 200), wxDEFAULT_FRAME_STYLE & ~wxMAXIMIZE_BOX & ~wxMINIMIZE_BOX) {
	CreateControls();
	SizeControls();
	Center();
	log = _log;
	solver = new Solver(log);
}

void Solve::CreateControls() {
	mPanel = new wxPanel(this, solve_panel);
	mInput = new wxTextCtrl(mPanel, solve_input);
	mOutput = new wxListBox(mPanel, solve_output);
	mOutput->Clear();
	mOutput->AppendString("Enter an equation to solve!");
}

void Solve::SizeControls() {
	mInput->SetSize(wxSize(
		GetClientSize().x - 4,
		(GetClientSize().y / 2) - 4
	));
	mOutput->SetSize(wxSize(
		GetClientSize().x - 4,
		(GetClientSize().y / 2) - 4
	));

	mInput->SetPosition(wxPoint(2, 0));
	mOutput->SetPosition(wxPoint(2, mInput->GetPosition().y + mInput->GetSize().y + 4));
}

void Solve::OnInputChange(wxCommandEvent & evt) {
	// Check for emptyness
	if (mInput->GetValue().empty()) {
		mOutput->Clear();
		mOutput->AppendString("Enter an equation to solve!");
		return;
	}
	// Check for blank
	if (mInput->GetValue() == " ") {
		mOutput->Clear();
		mOutput->AppendString("I wonder what a blank evaluates to!");
		return;
	}
	// Check for equal only
	if (mInput->GetValue() == "=") {
		mOutput->Clear();
		mOutput->AppendString("Equal is an Equal.");
		return;
	}
	// Check if there isnt an equal
	if (!isContain(mInput->GetValue().ToStdString(), '=')) {
		mOutput->Clear();
		mOutput->AppendString("There must be an equal sign!");
		return;
	}
	// Check for illegal chars
	for (auto c : mInput->GetValue().ToStdString()) {
		switch (c) {
		case '!':
		case '@':
		case '#':
		case '$':
		case '%':
		case '&':
		case '|':
		case '{':
		case '}':
		case '[':
		case ']':
		case '>':
		case '<':
		case '~':
		case '?':
			mOutput->Clear();
			mOutput->AppendString("Illegal character!");
			return;
			break;

		default:
			break;
		}
	}

	solver->Parse(mInput->GetValue().ToStdString());
	auto equ = solver->Solve();

	if (equ == nullptr) {
		// error occured
		mOutput->Clear();
		for (auto item : *log->qm_log)
			mOutput->AppendString(item);
		log->qm_log->clear();
		return;
	}

	if (equ->lwing.empty()) {
		mOutput->Clear();
		mOutput->AppendString("Left wing must not be empty!");
	}
	if (equ->rwing.empty()) {
		mOutput->Clear();
		mOutput->AppendString("Right wing must not be empty!");
	}


	string str;

	for (auto lwing_term : equ->lwing)
		str.append(lwing_term->to_str());
	
	str.append("=");

	for (auto rwing_term : equ->rwing)
		str.append(rwing_term->to_str());

	mOutput->Clear();
	mOutput->AppendString(str);
}

void Solve::OnClose(wxCloseEvent & evt) {
	this->GetParent()->Show(true);
	Destroy();
}

BEGIN_EVENT_TABLE(Solve, wxDialog)
EVT_TEXT(solve_input, Solve::OnInputChange)
EVT_CLOSE(Solve::OnClose)
END_EVENT_TABLE()