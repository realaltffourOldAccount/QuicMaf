#include <wx/wx.h>
#include "Dashboard.h"

class MyApp : public wxApp {
public:
	virtual bool OnInit();
};

bool MyApp::OnInit() {
	LOG* log = new LOG();
	Dashboard *dash = new Dashboard(log);
	dash->Show(true);

	return true;
}

wxIMPLEMENT_APP(MyApp);
