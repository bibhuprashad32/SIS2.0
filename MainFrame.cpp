#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	if (title == "Login")
	{
		login();
	}
	else if (title == "Homepage")
	{
		homepage();
		popup();
	}
	else if (title == "Home")
	{
		homepage();
	}
	else if (title == "SignUp")
	{
		signup();
	}
}



void MainFrame::updateDataInDatabase(const string& name, const string& fname, const string& mname, const string& gender, const string& blood, const string& DOB, const string& ageStr, const string& contact, const string& aadhaar, const string& address, const string& email, const string& pwd, const string& token, int rollno)
{
	//function will be available soon for logged in users
}


void MainFrame::login()
{
	wxPanel* panel = new wxPanel(this);
	this->SetClientSize(500, 700);

	wxStaticText* loginLabel = new wxStaticText(panel, wxID_ANY, "Login", wxPoint(200, 50), wxSize(100, 30));
	loginLabel->SetForegroundColour(*wxRED);
	loginLabel->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	wxStaticText* rollnoLabel = new wxStaticText(panel, wxID_ANY, "Roll No:", wxPoint(100, 100), wxSize(70, 30));
	wxTextCtrl* rollnoField = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(180, 100), wxSize(200, 30));

	wxStaticText* passwordLabel = new wxStaticText(panel, wxID_ANY, "Password:", wxPoint(100, 150), wxSize(70, 30));
	wxTextCtrl* passwordField = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(180, 150), wxSize(200, 30), wxTE_PASSWORD);

	wxButton* loginButton = new wxButton(panel, wxID_ANY, "Login", wxPoint(200, 200), wxSize(100, 30));
	wxButton* forgetPasswordButton = new wxButton(panel, wxID_ANY, "Forget Password?", wxPoint(200, 250), wxSize(150, 30));
	wxButton* goBackButton = new wxButton(panel, wxID_ANY, "Go Back", wxPoint(200, 250), wxSize(80, 30));
	
	wxStaticText* tokenLabel = new wxStaticText(panel, wxID_ANY, "Token:", wxPoint(100, 150), wxSize(70, 30));
	wxTextCtrl* tokenField = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(180, 150), wxSize(200, 30));
	wxButton* resetButton = new wxButton(panel, wxID_ANY, "Reset", wxPoint(200, 200), wxSize(100, 30));

	wxStaticText* newPasswordLabel = new wxStaticText(panel, wxID_ANY, "New Password:", wxPoint(100, 150), wxSize(100, 30));
	wxTextCtrl* newPasswordField = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxPoint(210, 150), wxSize(200, 30), wxTE_PASSWORD);
	wxButton* updateButton = new wxButton(panel, wxID_ANY, "Update", wxPoint(200, 200), wxSize(100, 30));

	tokenLabel->Hide();
	tokenField->Hide();
	resetButton->Hide();
	newPasswordLabel->Hide();
	newPasswordField->Hide();
	updateButton->Hide();
	goBackButton->Hide();

	loginButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
		if (verifyLogin(rollnoField->GetValue().ToStdString(), passwordField->GetValue().ToStdString()))
		{
			wxMessageBox("Login Successful", "Thanks for using my tool", wxOK | wxICON_INFORMATION);
			this->Close();
		}
		else
		{
			wxMessageBox("Invalid Credintials", "Invalid", wxOK | wxICON_ERROR);
		}
		});

	forgetPasswordButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
		passwordLabel->Hide();
		passwordField->Hide();
		loginButton->Hide();
		tokenLabel->Show();
		tokenField->Show();
		resetButton->Show();
		forgetPasswordButton->Hide();
		goBackButton->Show();
		panel->Layout();
		});

	goBackButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
		passwordLabel->Show();
		passwordField->Show();
		loginButton->Show();
		//tokenLabel->Show();
		//tokenField->Show();
		//resetButton->Show();
		forgetPasswordButton->Show();
		goBackButton->Hide();

		tokenLabel->Hide();
		tokenField->Hide();
		resetButton->Hide();
		newPasswordLabel->Hide();
		newPasswordField->Hide();
		updateButton->Hide();
		goBackButton->Hide();

		panel->Layout();
		});

	resetButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
		if (verifyToken(rollnoField->GetValue().ToStdString(), tokenField->GetValue().ToStdString()))
		{
			tokenLabel->Hide();
			tokenField->Hide();
			resetButton->Hide();
			newPasswordLabel->Show();
			newPasswordField->Show();
			updateButton->Show();
			passwordField->Hide();
			passwordLabel->Hide();
			loginButton->Hide();
			tokenField->Clear();
			goBackButton->Hide();
			forgetPasswordButton->Show();
		}
		else
		{
			tokenLabel->Show();
			tokenField->Show();
			resetButton->Show();
			newPasswordLabel->Hide();
			newPasswordField->Hide();
			updateButton->Hide();
			passwordField->Hide();
			passwordLabel->Hide();
			loginButton->Hide();
			goBackButton->Show();
			forgetPasswordButton->Hide();
			wxMessageBox("Invalid Credintials", "Invalid", wxOK | wxICON_ERROR);
		}

		panel->Layout();
		});

	updateButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) {
		updatePassword(rollnoField->GetValue().ToStdString(), newPasswordField->GetValue().ToStdString());
		tokenLabel->Hide();
		tokenField->Hide();
		resetButton->Hide();
		newPasswordLabel->Hide();
		newPasswordField->Hide();
		updateButton->Hide();
		passwordField->Show();
		passwordLabel->Show();
		loginButton->Show();
		wxMessageBox("Password updated successfully. Kindly Relogin", "Success", wxOK | wxICON_INFORMATION);
		});

	this->Show(true);
	this->Centre();
}

bool MainFrame::verifyLogin(const string& rollno, const string& password)
{
	string pswd = hashPassword(password);

	sqlite3* db;
	sqlite3_open("database.db", &db);
	string sql = "SELECT pwd FROM studdata WHERE rollno=" + rollno + ";";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
	string dbPassword;

	if (sqlite3_step(stmt) == SQLITE_ROW) {
		dbPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);

	if (pswd == dbPassword) {
		return true;
	}
	else {
		return false;
	}
}

bool MainFrame::verifyToken(const string& rollno, const string& token)
{
	string hashedtoken = hashPassword(token);
	sqlite3* db;
	sqlite3_open("database.db", &db);
	string sql = "SELECT token FROM studdata WHERE rollno=" + rollno + ";";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
	string dbToken;

	if (sqlite3_step(stmt) == SQLITE_ROW) {
		dbToken = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);

	if (hashedtoken == dbToken) {
		return true;
	}
	else {
		return false;
	}
}

void MainFrame::updatePassword(const string& rollno, const string& newPassword)
{

	string pswd = hashPassword(newPassword);
	string newtoken = fetchToken();
	string hashedtoken = hashPassword(newtoken);
	sqlite3* db;
	sqlite3_open("database.db", &db);
	string sqlUpdate = "UPDATE studdata SET pwd='" + pswd + "', token='" + hashedtoken + "' WHERE rollno=" + rollno + ";";

	char* zErrMsg = 0;
	sqlite3_exec(db, sqlUpdate.c_str(), 0, 0, &zErrMsg);

	if (zErrMsg) {
		wxMessageBox("Password update failed", "Error", wxOK | wxICON_ERROR);
		sqlite3_free(zErrMsg);
	}
	else {

		wxDialog dialog(this, wxID_ANY, "Success", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

		wxStaticText* wlcmText = new wxStaticText(&dialog, wxID_ANY, "Password updated", wxDefaultPosition, wxDefaultSize);
		wxFont font(13, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		wlcmText->SetFont(font);
		sizer->Add(wlcmText, 0, wxALL, 10);

		wxStaticText* messageText = new wxStaticText(&dialog, wxID_ANY, "Your new token is " + newtoken);
		sizer->Add(messageText, 0, wxALL, 10);

		wxStaticText* warningText = new wxStaticText(&dialog, wxID_ANY, "Remember it can be used for only once and not visible again, so please copy it and keep it safe.", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
		warningText->SetFont(font);
		warningText->SetForegroundColour(wxColour(255, 0, 0));
		sizer->Add(warningText, 0, wxALL, 10);

		wxButton* copyButton = new wxButton(&dialog, wxID_ANY, "Copy Token");
		sizer->Add(copyButton, 0, wxALL | wxALIGN_CENTER, 10);

		copyButton->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event) {
			if (wxTheClipboard->Open()) {
				wxTheClipboard->SetData(new wxTextDataObject(newtoken));
				wxTheClipboard->Close();
			}
			wxMessageBox("Token copied to clipboard", "Info", wxOK | wxICON_INFORMATION, this);
			});

		wxButton* okButton = new wxButton(&dialog, wxID_OK, "OK");
		sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 10);

		dialog.SetSizerAndFit(sizer);
		dialog.ShowModal();
	}
	sqlite3_close(db);
}

string MainFrame::hashPassword(const std::string& password)
{
	return picosha2::hash256_hex_string(password);
}


void MainFrame::popup()
{
	wxDialog dialog(this, wxID_ANY, "Support", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* supportText = new wxStaticText(&dialog, wxID_ANY, "Developed by Bibhu Prashad Sahu", wxDefaultPosition, wxDefaultSize);
	wxFont font(13, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	supportText->SetFont(font);
	supportText->SetForegroundColour(*wxRED);
	sizer->Add(supportText, 0, wxALL, 10);

	wxStaticText* messageText = new wxStaticText(&dialog, wxID_ANY, "Please visit my github profile for more: @bibhuprashad32");
	sizer->Add(messageText, 0, wxALL, 10);

	wxButton* copyButton = new wxButton(&dialog, wxID_ANY, "Copy Link");
	sizer->Add(copyButton, 0, wxALL | wxALIGN_CENTER, 10);


	copyButton->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event) {
		if (wxTheClipboard->Open()) {
			wxTheClipboard->SetData(new wxTextDataObject("https://github.com/bibhuprashad32/"));
			wxTheClipboard->Close();
		}
		wxMessageBox("Url copied to clipboard", "Info", wxOK | wxICON_INFORMATION, this);
		});


	wxButton* okButton = new wxButton(&dialog, wxID_OK, "OK");
	sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 10);

	dialog.SetSizerAndFit(sizer);
	dialog.ShowModal();
}

void MainFrame::homepage()
{
	wxPanel* panel = new wxPanel(this);
	SetClientSize(250, 300);
	wxStaticText* text = new wxStaticText(panel, wxID_ANY, "Student Information System", wxPoint(25, 50), wxSize(80, 20));
	wxFont font(13, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	text->SetFont(font);
	text->SetForegroundColour(wxColour(255, 0, 0));

	wxButton* loginButton = new wxButton(panel, wxID_ANY, "Login", wxPoint(85, 90), wxSize(80, 50));
	wxButton* signupButton = new wxButton(panel, wxID_ANY, "SignUp", wxPoint(85, 150), wxSize(80, 50));
	wxGauge* gauge = new wxGauge(this, wxID_ANY, 100, wxPoint(55, 220), wxSize(140, 20));
	gauge->Hide();

	Centre();

	Show(true);
	loginButton->Bind(wxEVT_BUTTON, [this, gauge](wxCommandEvent& event) { switchMainframe(event, "Login", gauge); }); 
	signupButton->Bind(wxEVT_BUTTON, [this, gauge](wxCommandEvent& event) { switchMainframe(event, "SignUp", gauge); });

}



void MainFrame::switchMainframe(wxCommandEvent& evt, wxString title, wxGauge* gauge)
{
	this->fastloading(gauge);
	MainFrame* m = new MainFrame(title);

	this->Close();
}

void MainFrame::fastloading(wxGauge* gauge)
{
	gauge->Show();
	for (int i = 0; i <= 10; i++)
	{
		gauge->SetValue(i * 10);
		wxMilliSleep(20);
	}
}


int MainFrame::calculateAge(int day, int month, int year)
{
	std::time_t t = std::time(nullptr);
	std::tm* now = std::localtime(&t);

	int currentYear = now->tm_year + 1900;
	int currentMonth = now->tm_mon + 1;
	int currentDay = now->tm_mday;

	int age = currentYear - year;
	if (currentMonth < month || (currentMonth == month && currentDay < day)) {
		age--;
	}
	return age;
}



void MainFrame::signup()
{
	wxPanel* panel = new wxPanel(this);
	this->SetClientSize(600, 700);

	wxString* name = new wxString("");
	wxString* fname = new wxString("");
	wxString* mname = new wxString("");
	wxString* gender = new wxString("");
	wxString* blood = new wxString("");
	wxString* DOB = new wxString("");
	wxString* ageStr = new wxString("");
	wxString* contact = new wxString("");
	wxString* aadhaar = new wxString("");
	wxString* address = new wxString("");
	wxString* email = new wxString("");
	bool* isValidateEmailClicked = new bool(false);

	this->signupForm(panel, name, fname, mname, gender, blood, DOB, ageStr, contact, aadhaar, address, email, isValidateEmailClicked);


	wxButton* gobackButton = new wxButton(panel, wxID_ANY, "Go Back", wxPoint(150, 480), wxSize(100, 50));
	wxButton* enrollButton = new wxButton(panel, wxID_ANY, "Enroll", wxPoint(350, 480), wxSize(100, 50));

	wxFont buttonFont(15, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Helvetica");

	gobackButton->SetFont(buttonFont);
	enrollButton->SetFont(buttonFont);

	gobackButton->SetForegroundColour(*wxRED);
	enrollButton->SetForegroundColour(*wxRED);

	wxGauge* waitingGauge = new wxGauge(panel, wxID_ANY, 100, wxPoint(50, 580), wxSize(500, 20));
	waitingGauge->Hide();


	gobackButton->Bind(wxEVT_BUTTON, [this, waitingGauge](wxCommandEvent& event) { switchMainframe(event, "Home", waitingGauge); });
	enrollButton->Bind(wxEVT_BUTTON, [this, waitingGauge, name, fname, mname, gender, blood, DOB, ageStr, contact, aadhaar, address, email, isValidateEmailClicked]
	(wxCommandEvent& event)
		{

			if (*isValidateEmailClicked)
			{
				saveValues(name, fname, mname, gender, blood, DOB, ageStr, contact, aadhaar, address, email);
				*isValidateEmailClicked = false;
			}
			else
			{
				*email = "";
				wxMessageBox(
					"Please click on validate email button, before submitting the enrollment form.",
					"Invalid Email",
					wxOK | wxICON_WARNING
				);

			}


		});

	this->Show(true);
	this->Centre();


}

void MainFrame::saveValues(wxString* name, wxString* fname, wxString* mname, wxString* gender, wxString* blood, wxString* DOB, wxString* ageStr, wxString* contact, wxString* aadhaar, wxString* address, wxString* email) {
	wxString message;
	int rollno = getStudentRollNo();
	wxString token = fetchToken();


	wxString wlcmmsg = "Thanks ! " + *name + " for enrolling.";
	message << "Your Roll No. is " << wxString::Format(wxT("%d"), rollno) << "\n";
	message << "Your Password Reset Token: " << token << "\n";
	message << "Keep this token safe, use it for password recovery\n";
	message << "Remember it can be used for only once and not visible again, so please copy it and keep it safe.";

	uploadToDatabase(string(name->ToStdString()), string(fname->ToStdString()), string(mname->ToStdString()), string(gender->ToStdString()), string(blood->ToStdString()), string(DOB->ToStdString()), string(ageStr->ToStdString()), string(contact->ToStdString()), string(aadhaar->ToStdString()), string(address->ToStdString()), string(email->ToStdString()), string(token.ToStdString()), rollno);

	wxDialog dialog(this, wxID_ANY, "Success", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE);
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* wlcmText = new wxStaticText(&dialog, wxID_ANY, wlcmmsg, wxDefaultPosition, wxDefaultSize);
	wxFont font(13, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	wlcmText->SetFont(font);
	sizer->Add(wlcmText, 0, wxALL, 10);

	wxStaticText* messageText = new wxStaticText(&dialog, wxID_ANY, message);
	sizer->Add(messageText, 0, wxALL, 10);

	wxStaticText* warningText = new wxStaticText(&dialog, wxID_ANY, "Remember it can be used for only once and not visible again, so please copy it and keep it safe.", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	warningText->SetFont(font);
	warningText->SetForegroundColour(wxColour(255, 0, 0));
	sizer->Add(warningText, 0, wxALL, 10);

	wxButton* copyButton = new wxButton(&dialog, wxID_ANY, "Copy Token");
	sizer->Add(copyButton, 0, wxALL | wxALIGN_CENTER, 10);

	copyButton->Bind(wxEVT_BUTTON, [&](wxCommandEvent& event) {
		if (wxTheClipboard->Open()) {
			wxTheClipboard->SetData(new wxTextDataObject(token));
			wxTheClipboard->Close();
		}
		wxMessageBox("Token copied to clipboard", "Info", wxOK | wxICON_INFORMATION, this);
		});

	wxButton* okButton = new wxButton(&dialog, wxID_OK, "OK");
	sizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 10);

	dialog.SetSizerAndFit(sizer);
	dialog.ShowModal();
}


string MainFrame::fetchToken()
{
	const string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	string randomString;
	srand(time(0));

	for (size_t i = 0; i < 32; ++i)
	{
		randomString += characters[rand() % characters.size()];
	}

	return randomString;
}

int MainFrame::getStudentRollNo() {
	sqlite3* db;
	sqlite3_stmt* stmt;
	char* zErrMsg = 0;
	int rc;
	int rollNo = 1;

	rc = sqlite3_open("database.db", &db);

	if (rc) {
		wxString wxErrMsg = wxString::FromUTF8(sqlite3_errmsg(db));
		wxString errMsg = "Can't open database: " + wxErrMsg;
		wxMessageBox(errMsg, "Error", wxOK | wxICON_ERROR);
		return rollNo;
	}

	const char* sqlCreateTable = "CREATE TABLE IF NOT EXISTS studroll(id INTEGER PRIMARY KEY, rollno INTEGER);";
	rc = sqlite3_exec(db, sqlCreateTable, 0, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		wxString wxStr = wxString::FromUTF8(zErrMsg);
		wxString zerr = "SQL error: " + wxStr;
		wxMessageBox(zerr, "Error", wxOK | wxICON_ERROR);
		sqlite3_free(zErrMsg);
		sqlite3_close(db);
		return rollNo;
	}

	const char* sqlSelect = "SELECT rollno FROM studroll WHERE id = 1;";
	rc = sqlite3_prepare_v2(db, sqlSelect, -1, &stmt, 0);

	if (rc == SQLITE_OK) {
		if (sqlite3_step(stmt) == SQLITE_ROW) {
			rollNo = sqlite3_column_int(stmt, 0);
		}
		else {
			const char* sqlInsert = "INSERT INTO studroll (id, rollno) VALUES (1, 1);";
			rc = sqlite3_exec(db, sqlInsert, 0, 0, &zErrMsg);
			if (rc != SQLITE_OK) {
				wxString wxStr = wxString::FromUTF8(zErrMsg);
				wxString zerr = "Insert error: " + wxStr;
				wxMessageBox(zerr, "Error", wxOK | wxICON_ERROR);
				sqlite3_free(zErrMsg);
			}
		}
	}
	else {
		wxString wxStr = wxString::FromUTF8(sqlite3_errmsg(db));
		wxString err = "SQL error: " + wxStr;
		wxMessageBox(err, "Error", wxOK | wxICON_ERROR);
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);

	return rollNo;
}

void MainFrame::fetchDataFromDatabase(int rollno)
{
	sqlite3* db;
	sqlite3_stmt* stmt;
	int rc;

	rc = sqlite3_open("database.db", &db);

	if (rc) {
		wxString wxErrMsg = wxString::FromUTF8(sqlite3_errmsg(db));
		wxString errMsg = "Can't open database: " + wxErrMsg;
		wxMessageBox(errMsg, "Error", wxOK | wxICON_ERROR);
		return;
	}

	string sqlSelect = "SELECT * FROM studdata WHERE rollno = " + to_string(rollno) + ";";

	rc = sqlite3_prepare_v2(db, sqlSelect.c_str(), -1, &stmt, 0);

	if (rc != SQLITE_OK) {
		wxMessageBox("No student database found", "Error", wxOK | wxICON_ERROR);
		sqlite3_close(db);
		return;
	}

	if (sqlite3_step(stmt) == SQLITE_ROW) {
		string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
		string fname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		string mname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		string gender = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		string blood = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
		string DOB = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
		string ageStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
		string contact = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
		string aadhaar = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
		string address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
		string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
		string pwd = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
		string token = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));


		string msg = "Student user99 found: " + name + ", " + std::to_string(rollno);
		wxString wxmsg(msg.c_str(), wxConvUTF8);
		wxMessageBox(wxmsg, "Information", wxOK | wxICON_INFORMATION);
	}
	else {
		wxMessageBox("User not found", "Error", wxOK | wxICON_ERROR);
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);
}


void MainFrame::uploadToDatabase(const string& name, const string& fname, const string& mname, const string& gender, const string& blood, const string& DOB, const string& ageStr, const string& contact, const string& aadhaar, const string& address, const string& email, const string& token, int rollno) {
	const string randompswd = fetchToken();
	const string hashedtoken = hashPassword(token);
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	rc = sqlite3_open("database.db", &db);

	if (rc) {
		wxString wxErrMsg = wxString::FromUTF8(sqlite3_errmsg(db));
		wxString errMsg = "Can't open database: " + wxErrMsg;
		wxMessageBox(errMsg, "Error", wxOK | wxICON_ERROR);
		return;
	}

	const char* sql = "CREATE TABLE IF NOT EXISTS studdata("
		"name TEXT, fname TEXT, mname TEXT, gender TEXT, blood TEXT, DOB TEXT, "
		"ageStr TEXT, contact TEXT, aadhaar TEXT, address TEXT, email TEXT, pwd TEXT, token TEXT, "
		"rollno INTEGER PRIMARY KEY);";
	rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

	string sqlInsert = "INSERT INTO studdata (name, fname, mname, gender, blood, DOB, ageStr, contact, aadhaar, address, email, pwd, token, rollno) "
		"VALUES ('" + name + "','" + fname + "','" + mname + "','" + gender + "','" + blood + "','" + DOB + "','" + ageStr + "','" + contact + "','" + aadhaar + "','" + address + "','" + email + "','" + randompswd + "','" + hashedtoken + "','" + to_string(rollno) + "');";
	rc = sqlite3_exec(db, sqlInsert.c_str(), 0, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		wxString wxStr = wxString::FromUTF8(zErrMsg);
		wxString zerr = "SQL error: " + wxStr;
		wxMessageBox(zerr, "Error", wxOK | wxICON_ERROR);
		sqlite3_free(zErrMsg);
	}
	else {
		wxMessageBox("Records created successfully", "Success", wxOK | wxICON_INFORMATION);

		const char* sqlUpdate = "UPDATE studroll SET rollno = rollno + 1 WHERE id = 1;";
		rc = sqlite3_exec(db, sqlUpdate, 0, 0, &zErrMsg);

		if (rc != SQLITE_OK) {
			wxString wxStr = wxString::FromUTF8(zErrMsg);
			wxString zerr = "SQL error: " + wxStr;
			wxMessageBox(zerr, "Error", wxOK | wxICON_ERROR);
			sqlite3_free(zErrMsg);
		}
		else {
			wxMessageBox("Roll number updated successfully", "Success", wxOK | wxICON_INFORMATION);
		}
	}




	sqlite3_close(db);
}




void MainFrame::signupForm(wxPanel* panel, wxString* name, wxString* fname, wxString* mname, wxString* gender, wxString* blood, wxString* DOB, wxString* ageStr, wxString* contact, wxString* aadhaar, wxString* address, wxString* email, bool* isValidateEmailClicked)
{
	{
		wxStaticText* nameLabel = new wxStaticText(panel, wxID_ANY, "Student Name: Enter name in the box", wxPoint(30, 18), wxSize(50, 10));
		wxTextCtrl* nameTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(300, 15), wxSize(150, 25));
		nameTextCtrl->Bind(wxEVT_TEXT, [this, name, nameLabel, nameTextCtrl]
		(wxCommandEvent& event)
			{

				*name = nameTextCtrl->GetValue();
				nameLabel->SetLabel("Student Name: " + *name);
			});
		wxStaticText* fnameLabel = new wxStaticText(panel, wxID_ANY, "Father's Name: Enter name in the box", wxPoint(30, 49), wxSize(50, 10));
		wxTextCtrl* fnameTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(300, 45), wxSize(150, 25));
		fnameTextCtrl->Bind(wxEVT_TEXT, [this, fname, fnameLabel, fnameTextCtrl]
		(wxCommandEvent& event)
			{

				*fname = fnameTextCtrl->GetValue();
				fnameLabel->SetLabel("Father's Name: " + *fname);
			});
		wxStaticText* mnameLabel = new wxStaticText(panel, wxID_ANY, "Mother's Name: Enter name in the box", wxPoint(30, 80), wxSize(50, 10));
		wxTextCtrl* mnameTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(300, 75), wxSize(150, 25));
		mnameTextCtrl->Bind(wxEVT_TEXT, [this, mname, mnameLabel, mnameTextCtrl]
		(wxCommandEvent& event)
			{

				*mname = mnameTextCtrl->GetValue();
				mnameLabel->SetLabel("Mother's Name: " + *mname);
			});

		wxCheckBox* maleCheckbox = new wxCheckBox(panel, wxID_ANY, "Male", wxPoint(300, 110), wxSize(70, 25));
		wxCheckBox* femaleCheckbox = new wxCheckBox(panel, wxID_ANY, "Female", wxPoint(380, 110), wxSize(70, 25));
		wxCheckBox* otherCheckbox = new wxCheckBox(panel, wxID_ANY, "Other", wxPoint(450, 110), wxSize(70, 25));

		wxStaticText* genderLabel = new wxStaticText(panel, wxID_ANY, "Gender: ", wxPoint(30, 115), wxSize(50, 10));

		maleCheckbox->Bind(wxEVT_CHECKBOX, [this, gender, maleCheckbox, femaleCheckbox, otherCheckbox, genderLabel]
		(wxCommandEvent& event)
			{
				if (maleCheckbox->GetValue())
				{
					*gender = "Male";
					genderLabel->SetLabel("Gender: " + *gender);
				}
				else
				{
					*gender = "";
					genderLabel->SetLabel("Gender: Please Select Your Gender");
				}

				femaleCheckbox->SetValue(false);
				otherCheckbox->SetValue(false);
			});
		femaleCheckbox->Bind(wxEVT_CHECKBOX, [this, gender, maleCheckbox, femaleCheckbox, otherCheckbox, genderLabel]
		(wxCommandEvent& event)
			{
				if (femaleCheckbox->GetValue())
				{
					*gender = "Female";
					genderLabel->SetLabel("Gender: " + *gender);
				}
				else
				{
					*gender = "";
					genderLabel->SetLabel("Gender: Please Select Your Gender");
				}
				maleCheckbox->SetValue(false);
				otherCheckbox->SetValue(false);
			});
		otherCheckbox->Bind(wxEVT_CHECKBOX, [this, gender, maleCheckbox, femaleCheckbox, otherCheckbox, genderLabel]
		(wxCommandEvent& event)
			{
				if (otherCheckbox->GetValue())
				{
					*gender = "Other";
					genderLabel->SetLabel("Gender: " + *gender);
				}
				else
				{
					*gender = "";
					genderLabel->SetLabel("Gender: Please Select Your Gender");
				}
				maleCheckbox->SetValue(false);
				femaleCheckbox->SetValue(false);
			});


		wxArrayString choices;
		choices.Add("A+");
		choices.Add("A-");
		choices.Add("B+");
		choices.Add("B-");
		choices.Add("AB+");
		choices.Add("AB-");
		choices.Add("O+");
		choices.Add("O-");
		choices.Add("Unknown");

		wxStaticText* bloodLabel = new wxStaticText(panel, wxID_ANY, "Blood Group: Select Blood", wxPoint(30, 150), wxSize(50, 10));
		wxChoice* bloodChoice = new wxChoice(panel, wxID_ANY, wxPoint(300, 145), wxSize(100, 50), choices);
		bloodChoice->Bind(wxEVT_CHOICE, [this, blood, bloodChoice, bloodLabel]
		(wxCommandEvent& event)
			{
				*blood = bloodChoice->GetString(bloodChoice->GetSelection());
				bloodLabel->SetLabel("Blood Group: " + *blood);
			});


		int* day = new int(-1);
		int* month = new int(-1);
		int* year = new int(-1);



		wxString* tempDOB = new wxString("");
		wxStaticText* DOBLabel = new wxStaticText(panel, wxID_ANY, "Enter DOB: Enter your Date Of Birth", wxPoint(30, 195), wxSize(50, 10));
		wxSpinCtrl* dd = new wxSpinCtrl(panel, wxID_ANY, "Day", wxPoint(420, 190), wxSize(50, -1));
		dd->SetRange(1, 31);
		dd->Hide();
		wxSpinCtrl* mm = new wxSpinCtrl(panel, wxID_ANY, "Month", wxPoint(360, 190), wxSize(50, -1));
		mm->SetRange(1, 12);
		mm->Hide();
		wxSpinCtrl* yyyy = new wxSpinCtrl(panel, wxID_ANY, "Year", wxPoint(300, 190), wxSize(50, -1));
		yyyy->SetRange(1900, 2100);

		yyyy->Bind(wxEVT_SPINCTRL, [this, yyyy, mm, dd, day, month, year, tempDOB, DOB, DOBLabel]
		(wxCommandEvent& event)
			{
				*year = yyyy->GetValue();
				mm->Show();
				mm->SetValue(1);
				dd->SetValue(1);
				*day = -1;
				*month = -1;
				dd->Hide();
				*tempDOB = wxString("dd/mm/") + wxString::Format("%d", *year);
				DOBLabel->SetLabel("Date Of Birth: " + *tempDOB);
				if (*day != -1 && *month != -1 && *year != -1) *DOB = wxString::Format("%d/", *day) + wxString::Format("%d/", *month) + wxString::Format("%d", *year);
				else *DOB = "";
			});
		mm->Bind(wxEVT_SPINCTRL, [this, mm, dd, year, month, day, tempDOB, DOB, DOBLabel]
		(wxCommandEvent& event)
			{
				*month = mm->GetValue();
				//dd->Show();
				dd->SetValue(1);
				*day = -1;
				*tempDOB = wxString("dd/") + wxString::Format("%d/", *month) + wxString::Format("%d", *year);
				if (*month == 2)
				{

					if ((*year % 4 == 0 && *year % 100 != 0) || (*year % 400 == 0))
					{
						dd->SetRange(1, 29);
					}
					else
					{
						dd->SetRange(1, 28);
					}
				}
				else if (*month == 4 || *month == 6 || *month == 9 || *month == 11)
				{
					dd->SetRange(1, 30);
				}
				else
				{
					dd->SetRange(1, 31);
				}
				dd->Show();
				DOBLabel->SetLabel("Date Of Birth: " + *tempDOB);
				if (*day != -1 && *month != -1 && *year != -1) *DOB = wxString::Format("%d/", *day) + wxString::Format("%d/", *month) + wxString::Format("%d", *year);
				else *DOB = "";
			});
		dd->Bind(wxEVT_SPINCTRL, [this, day, dd, month, year, tempDOB, DOB, DOBLabel]
		(wxCommandEvent& event)
			{
				if (*month == 2)
				{

					if ((*year % 4 == 0 && *year % 100 != 0) || (*year % 400 == 0))
					{
						dd->SetRange(1, 29);
					}
					else
					{
						dd->SetRange(1, 28);
					}
				}
				else if (*month == 4 || *month == 6 || *month == 9 || *month == 11)
				{
					dd->SetRange(1, 30);
				}
				else
				{
					dd->SetRange(1, 31);
				}
				*day = dd->GetValue();
				*tempDOB = wxString::Format("%d/", *day) + wxString::Format("%d/", *month) + wxString::Format("%d", *year);
				DOBLabel->SetLabel("Date Of Birth: " + *tempDOB);
				if (*day != -1 && *month != -1 && *year != -1) *DOB = wxString::Format("%d/", *day) + wxString::Format("%d/", *month) + wxString::Format("%d", *year);
				else *DOB = "";

			});

		wxStaticText* ageLabel = new wxStaticText(panel, wxID_ANY, *ageStr, wxPoint(30, 240), wxSize(50, 10));
		ageLabel->Hide();
		wxButton* getAge = new wxButton(panel, wxID_ANY, "Get Age", wxPoint(300, 240), wxSize(90, 30));
		getAge->Bind(wxEVT_BUTTON, [this, ageStr, ageLabel, day, month, year](wxCommandEvent& event)
			{
				*ageStr = wxString::Format("%d", calculateAge(*day, *month, *year));
				ageLabel->SetLabel("Student Age: " + *ageStr);
				ageLabel->Show();
			});


		wxStaticText* contactLabel = new wxStaticText(panel, wxID_ANY, "Contact Number: Enter Name in the Box", wxPoint(30, 280), wxSize(50, 10));
		wxTextCtrl* contactTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(300, 280), wxSize(150, 25), 0, wxTextValidator(wxFILTER_DIGITS));
		contactTextCtrl->SetMaxLength(10);
		contactTextCtrl->Bind(wxEVT_TEXT, [this, contact, contactLabel, contactTextCtrl]
		(wxCommandEvent& event)
			{
				*contact = contactTextCtrl->GetValue();
				contactLabel->SetLabel("Contact Number: " + *contact);
			});

		wxStaticText* aadhaarLabel = new wxStaticText(panel, wxID_ANY, "Aadhaar Number: Enter Name in the Box", wxPoint(30, 320), wxSize(50, 10));
		wxTextCtrl* aadhaarTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(300, 320), wxSize(150, 25), 0, wxTextValidator(wxFILTER_DIGITS));
		aadhaarTextCtrl->SetMaxLength(12);
		aadhaarTextCtrl->Bind(wxEVT_TEXT, [this, aadhaar, aadhaarLabel, aadhaarTextCtrl]
		(wxCommandEvent& event)
			{
				*aadhaar = aadhaarTextCtrl->GetValue();
				aadhaarLabel->SetLabel("Aadhar Number: " + *aadhaar);
			});

		wxStaticText* addressLabel = new wxStaticText(panel, wxID_ANY, "Mailling Address: Enter address in the box", wxPoint(30, 350), wxSize(50, 10));
		wxTextCtrl* addressTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(300, 350), wxSize(150, 25));
		addressTextCtrl->Bind(wxEVT_TEXT, [this, address, addressLabel, addressTextCtrl]
		(wxCommandEvent& event)
			{

				*address = addressTextCtrl->GetValue();
				addressLabel->SetLabel("Mailling Address: " + *address);
			});

		wxStaticText* emailLabel = new wxStaticText(panel, wxID_ANY, "E-mail: Enter email in the box", wxPoint(30, 380), wxSize(50, 10));
		wxTextCtrl* emailTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(300, 380), wxSize(150, 25));
		emailTextCtrl->Bind(wxEVT_TEXT, [this, isValidateEmailClicked]
		(wxCommandEvent& event)
			{
				*isValidateEmailClicked = false;
			});
		wxButton* validateEmailButton = new wxButton(panel, wxID_ANY, "Validate Email", wxPoint(460, 380), wxSize(100, 25));
		validateEmailButton->Bind(wxEVT_BUTTON, [email, emailLabel, emailTextCtrl, isValidateEmailClicked]
		(wxCommandEvent& event)
			{
				string emailstr = (emailTextCtrl->GetValue()).ToStdString();
				size_t atPos = emailstr.find('@');
				size_t dotPos = emailstr.find('.', atPos);
				bool isValid = (atPos != string::npos) && (dotPos != string::npos) && (atPos < dotPos) && (atPos + 1 != dotPos) && (dotPos != emailstr.length() - 1) && (emailstr.back() != '.');

				if (isValid)
				{
					*isValidateEmailClicked = true;
					*email = emailTextCtrl->GetValue();
					emailLabel->SetLabel("E-mail: " + *email);
				}
				else
				{
					*isValidateEmailClicked = false;
					*email = "";
					emailLabel->SetLabel("E-mail: Enter a valid email: ");
					wxMessageBox(
						"Your email must contain a '@' symbol and a domain name (e.g. user@domain.com).\nPlease enter a valid email address.",
						"Invalid Email",
						wxOK | wxICON_WARNING
					);

				}

			});


		wxFont font(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		nameLabel->SetFont(font);
		nameTextCtrl->SetFont(font);
		fnameLabel->SetFont(font);
		fnameTextCtrl->SetFont(font);
		mnameLabel->SetFont(font);
		mnameTextCtrl->SetFont(font);
		genderLabel->SetFont(font);
		bloodLabel->SetFont(font);
		DOBLabel->SetFont(font);
		ageLabel->SetFont(font);
		contactLabel->SetFont(font);
		contactTextCtrl->SetFont(font);
		aadhaarLabel->SetFont(font);
		aadhaarTextCtrl->SetFont(font);
		addressLabel->SetFont(font);
		addressTextCtrl->SetFont(font);
		emailLabel->SetFont(font);
		emailTextCtrl->SetFont(font);
		Centre();
	}
}
