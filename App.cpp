#include "App.h"

bool App::OnInit()
{
	MainFrame* homepage = new MainFrame("Home");

	return true;
}


wxIMPLEMENT_APP(App);

