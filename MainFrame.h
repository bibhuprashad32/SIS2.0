#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/clipbrd.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "sqlite3.h"
#include "picosha2.h"


using namespace std;


class MainFrame : public wxFrame
{
private:
    void login();
    void signup();
    void switchMainframe(wxCommandEvent& evt, wxString title, wxGauge* gauge);
    void fastloading(wxGauge* gauge);
    int calculateAge(int day, int month, int year);
    void signupForm(wxPanel* panel, wxString* name, wxString* fname, wxString* mname, wxString* gender, wxString* blood, wxString* DOB, wxString* ageStr, wxString* contact, wxString* aadhaar, wxString* address, wxString* email, bool* isValidateEmailClicked);
    void uploadToDatabase(const string& name, const string& fname, const string& mname, const string& gender, const string& blood, const string& DOB, const string& ageStr, const string& contact, const string& aadhaar, const string& address, const string& email, const string& token, int rollno);
    void fetchDataFromDatabase(int rollno);
    void updateDataInDatabase(const string& name, const string& fname, const string& mname, const string& gender, const string& blood, const string& DOB, const string& ageStr, const string& contact, const string& aadhaar, const string& address, const string& email, const string& pwd, const string& token, int rollno);
    void saveValues(wxString* name, wxString* fname, wxString* mname, wxString* gender, wxString* blood, wxString* DOB, wxString* ageStr, wxString* contact, wxString* aadhaar, wxString* address, wxString* email);
    int getStudentRollNo();
    string fetchToken();
    bool verifyLogin(const string& rollno, const string& password);
    bool verifyToken(const string& rollno, const string& token);
    void updatePassword(const string& rollno, const string& newPassword);
    string hashPassword(const std::string& password);
public:
    MainFrame(const wxString& title);
    void homepage();

};


