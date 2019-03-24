#include "evaluator.h"

Evaluator::Evaluator(wxWindow* pId, LOG* _log) :
	wxDialog(pId, wxID_ANY, "QuicMaf | Evaluator", 
		wxDefaultPosition, wxSize(400, 200), wxDEFAULT_FRAME_STYLE & ~wxMAXIMIZE_BOX & ~wxMINIMIZE_BOX) {
	CreateControls();
	SizeControls();
	Center();
	log = _log;
	solver = new Solver(log);
}

void Evaluator::CreateControls() {
	mPanel = new wxPanel(this, eval_panel);

	mInput = new wxTextCtrl(mPanel, eval_input);
	mOutput = new wxListBox(mPanel, eval_output);
	mOutput->Clear();
	mOutput->AppendString("Enter an expression to evaluate!");
}

void Evaluator::SizeControls() {
	mInput->SetSize(wxSize(
		GetClientSize().x-4,
		(GetClientSize().y/2)-4
	));
	mOutput->SetSize(wxSize(
		GetClientSize().x-4,
		(GetClientSize().y / 2) - 4
	));

	mInput->SetPosition(wxPoint(2, 0));
	mOutput->SetPosition(wxPoint(2, mInput->GetPosition().y+mInput->GetSize().y+4));
}

void Evaluator::OnInputChange(wxCommandEvent & evt) {
	// Check for emptyness
	if (mInput->GetValue().empty()) {
		mOutput->Clear();
		mOutput->AppendString("Enter an expression to evaluate!");
		return;
	}
	// Check for blank
	if (mInput->GetValue() == " ") {
		mOutput->Clear();
		mOutput->AppendString("I wonder what a blank evaluates to!");
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

	if (CheckExpr(mInput->GetValue().ToStdString())) {
		vector<Term*> terms;
		string input = mInput->GetValue().ToStdString();
		lexertk::generator gen;
		gen.process(input);
		tok_log = log;
		terms = tokenize(gen);
		if (terms.empty()) {
			// error occured
			mOutput->Clear();
			for (auto item : *log->qm_log)
				mOutput->AppendString(item);
			log->qm_log->clear();
			return;
		}
		if (!solver->CheckValidOps(terms)) {
			mOutput->Clear();
			mOutput->AppendString("Operators are not correct!");
		}
		else {
			// check for ending operators
			if (terms[terms.size() - 1]->mType == TermTypes::Op) {
				// there can't be ending operators
				mOutput->Clear();
				mOutput->AppendString("Operators can't be at the end!");
			}
			else {
				// Break brackets
				terms = solver->RemoveBrackets(terms);

				// Remove Exponents
				for (int i = 0; i < terms.size(); i++) {
					if (terms[i]->mType == TermTypes::Const) {
						terms[i] = QMEvalHelper::ReducePower(terms[i])[0];
					}
				}

				terms = evaluate(terms);
				terms = solver->AddNonEssientalOps(terms);
				if (terms.empty()) {
					// error occured
					mOutput->Clear();
					for (auto item : *log->qm_log)
						mOutput->AppendString(item);
					log->qm_log->clear();
					return;
				}

				string output = "";
				for (int i = 0; i < terms.size(); i++)
					output.append(terms[i]->to_str());
				mOutput->Clear();
				mOutput->AppendString(output);
			}
		}
	}
	else {
		mOutput->Clear();
		mOutput->AppendString("There can't be an equal sign!");
	}
}

void Evaluator::OnClose(wxCloseEvent & evt) {
	this->GetParent()->Show(true);
	Destroy();
}

bool Evaluator::CheckExpr(string str) {	
	if (isContain(str, '=')) return false;
}

BEGIN_EVENT_TABLE(Evaluator, wxDialog)

EVT_TEXT(eval_input, Evaluator::OnInputChange)
EVT_CLOSE(Evaluator::OnClose)

END_EVENT_TABLE()