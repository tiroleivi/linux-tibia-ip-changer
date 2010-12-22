#include "gui.h"
#include "change.h"
#include "tibiapid.h"
#include "history.h"
#include "languages.h"
#include "setup.h"
#include "clientsversions.h"
#include "ico.xpm"

Setup s;
History h;
Languages lang;
std::vector<wxString>wxLang;

BEGIN_EVENT_TABLE(Panel,wxFrame)
	EVT_CLOSE(Panel::OnClose)
	EVT_MENU(wxID_EXIT, Panel::Quit)
	EVT_MENU(wxID_SETUP, Panel::Setup)
	EVT_MENU(MENU_INFO, Panel::Info)
	EVT_MENU(MENU_CLEAR_HISTORY, Panel::ClearHistory)
	EVT_BUTTON(wxID_EXIT, Panel::Quit)
	EVT_BUTTON(BUTTON_TO_TRAY, Panel::toTray)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyTaskBarIcon, wxTaskBarIcon)
    EVT_TASKBAR_LEFT_DCLICK(MyTaskBarIcon::trayLeftClick)
    EVT_MENU(wxID_EXIT, MyTaskBarIcon::Quit)
    EVT_MENU(MENU_O_M, MyTaskBarIcon::miniOrMax)
END_EVENT_TABLE()

MyTaskBarIcon::MyTaskBarIcon(Panel* frame) 
: wxTaskBarIcon() 
, frame(frame) 
{ 
} 

MyDialog::MyDialog(Panel* frame) 
: wxDialog(frame, wxID_ANY, wxLang.at(21), wxDefaultPosition, wxSize(160, 130))
, frame(frame) 
{ 
	wxPanel *p = new wxPanel(this, wxID_ANY, wxPoint(-1, -1), wxSize(130, 130));
	(void) new wxStaticText(p, wxID_ANY, wxLang.at(22), wxPoint(5, 5), wxSize(160, 25));
	wxString tab[lang.count];
	vector<string>z;
	z = lang.getLanguagesList();
	for(int i = 0; i < z.size(); i++){
		wxString wxs(z.at(i).c_str(), wxConvUTF8);
		tab[i] = wxs;
	}
	langCombo = new wxComboBox(p, wxID_ANY, tab[0], wxPoint(5, 25), wxSize(80, 25), WXSIZEOF(tab), tab, wxCB_READONLY);
	(void) new wxButton(p, BUTTON_OK, wxT("Ok"), wxPoint(125, 95), wxSize(30, 25));
	Connect(BUTTON_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyDialog::Ok));
} 

Panel::Panel(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxPoint(-1, -1), wxSize(350, 130))
{
	panel = new wxPanel(this, wxID_ANY, wxPoint(-1, -1), wxSize(350, 130));
	SetMinSize(wxSize(350, 130));
	SetMaxSize(wxSize(350, 130));
	std::string tmp;
	std::string st = lang.loadLanguage(s.loadSetupValue("lang"));
	for(int i = 0; i < st.size(); i++){
		std::string str;
		str += st.at(i);
		if(str != ",")
			tmp += str;
		else{
			wxString wxs(tmp.c_str(), wxConvUTF8);
			wxLang.push_back(wxs);
			tmp.clear();
		}
		str.clear();
	}
	int ile = int(sizeof(clients)/sizeof(clients[1]));
	wxString tab[ile+1];
	tab[0] = wxT("Auto");
	for(int i = 0; i < ile; i++){
		wxString wxs(clients[i][0].insert(1, ".").c_str(), wxConvUTF8);
		tab[i+1] = wxs;
	}
	tibiaversion = new wxComboBox(panel, -1, tab[0], wxPoint(274, 20), wxSize(65, 25), WXSIZEOF(tab), tab, wxCB_READONLY);
	statusbar = new wxStatusBar(this, wxID_ANY);
	statusbar->SetFieldsCount(1);
	int stat[1];
	stat[0] = -1;
	statusbar->SetStatusWidths(1,stat);
	SetStatusBar(statusbar);
	(void) new wxStaticText(panel, wxID_ANY, wxLang.at(1), wxPoint(5, 5), wxSize(350, 25));
	(void) new wxStaticText(panel, wxID_ANY, wxLang.at(2), wxPoint(226, 5), wxSize(350, 25));
	(void) new wxStaticText(panel, wxID_ANY, wxLang.at(3), wxPoint(272, 5), wxSize(350, 25));
	vector<string>z;
	z = h.getHistory();
	wxString tabb[z.size()];
	for(int i = 0; i < z.size(); i++){
		wxString wxs(z[i].c_str(), wxConvUTF8);
		tabb[i] = wxs;
	}
	ip = new wxComboBox(panel, wxID_ANY, wxT(""), wxPoint(5, 20), wxSize(214, 25), WXSIZEOF(tabb), tabb);
	port = new wxTextCtrl(panel, wxID_ANY, wxT("7171"), wxPoint(227, 20), wxSize(40, 25));
	port->SetMaxLength(4);
	menubar = new wxMenuBar;
	file = new wxMenu;
	file->Append(MENU_CLEAR_HISTORY, wxLang.at(4));
	file->Append(wxID_SETUP, wxLang.at(5));
	file->AppendSeparator(); 
	file->Append(wxID_EXIT, wxLang.at(6));
	menubar->Append(file, wxLang.at(7));
	help = new wxMenu;
	help->Append(MENU_INFO, wxLang.at(8));
	menubar->Append(help, wxLang.at(9));
	SetMenuBar(menubar);
	trayIcon = new MyTaskBarIcon(this);
	wxIcon icon(ico_xpm);
	if(!trayIcon->SetIcon(icon, wxT("IP Chabger")))
        wxMessageBox(wxT("Could not set icon."));
    wxButton *cIp = new wxButton(panel, BUTTON_CHANGE_IP, wxLang.at(10), wxPoint(60, 50), wxSize(90, 33));
    Connect(cIp->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Panel::guiChangeIP));
    wxButton *bTray = new wxButton(panel, BUTTON_TO_TRAY, wxLang.at(11), wxPoint(155, 50), wxSize(90, 33));
    wxButton *bQuit = new wxButton(panel, wxID_EXIT, wxLang.at(12), wxPoint(250, 50), wxSize(90, 33));
	
	this->SetIcon(icon);
	Centre();
}

void Panel::OnClose(wxCloseEvent& event){
	Iconize(true);
	Show(false);
}

void Panel::Quit(wxCommandEvent& event){ 
	trayIcon->RemoveIcon();
	Destroy();
}

void MyTaskBarIcon::Quit(wxCommandEvent& event){ 
	RemoveIcon();
	frame->Destroy();
}

void MyTaskBarIcon::trayLeftClick(wxTaskBarIconEvent& event){
	if(frame->IsIconized()){
		frame->Iconize(false);
		frame->Show(true);
	}else{
		frame->Iconize(true);
		frame->Show(false);	
	}
}

void MyTaskBarIcon::miniOrMax(wxCommandEvent& event){
	if(frame->IsIconized()){
		frame->Iconize(false);
		frame->Show(true);
	}else{
		frame->Iconize(true);
		frame->Show(false);	
	}
}

void Panel::toTray(wxCommandEvent& event){
	Iconize(true);
	Show(false);	
}

wxMenu* MyTaskBarIcon::CreatePopupMenu() 
{ 
    menuTray = new wxMenu();
	if(frame->IsIconized())
		menuTray->Append(MENU_O_M, wxLang.at(13));
	else
		menuTray->Append(MENU_O_M, wxLang.at(14));
    menuTray->AppendSeparator(); 
    menuTray->Append(wxID_EXIT, wxLang.at(15)); 

    return menuTray; 
}

void Panel::guiChangeIP(wxCommandEvent& event){
	TibiaPid pid;
	int tpid = pid.getTibiaPid();
	if(tpid == 0){
		statusbar->SetStatusText(wxLang.at(16),0);
		return;
	}

    std::string sip = std::string(ip->GetValue().mb_str());
    std::string sport = std::string(port->GetValue().mb_str());
    std::string client = std::string(tibiaversion->GetValue().mb_str());
    if(sip == ""){
		statusbar->SetStatusText(wxLang.at(17),0);
		return;
	}
	
	if(client != "Auto"){
		std::string s ="";
		s += client.at(1);
		if(s == ".")
			client = client.erase(1,1);
	}
	
	Change c(tpid);	
	bool ret = c.changeIP(sip, atoi(sport.c_str()), client);
	 
	if(ret)
		statusbar->SetStatusText(wxLang.at(18),0);
	else{
		statusbar->SetStatusText(wxLang.at(19),0);
		return;
	}
	
	h.saveHistory(sip);
	ip->Clear();
	vector<string>z;
	z = h.getHistory();
	for(int i = 0; i < z.size(); i++){
		wxString wxs(z[i].c_str(), wxConvUTF8);
		ip->Append(wxs);
	}
}

void Panel::Info(wxCommandEvent& event){
	wxMessageDialog *dial = new wxMessageDialog(NULL, wxT("This program change IP tibia client in order to connect to Open Tibia Server.\nContinue Moraxus project.\nLicense: GNU GPL\nProgrammer: Miziak.\nAdress Finder: Virtelio.\nWrote in wxWidgets library."), wxLang.at(24), wxOK);
	dial->ShowModal();
}

void Panel::Setup(wxCommandEvent& event){
	wxDialog *dial = new MyDialog(this);
	dial->Show();
}

void MyDialog::Ok(wxCommandEvent& event){
	s.saveSetup("lang", std::string(langCombo->GetValue().mb_str()));
	this->Destroy();
	wxMessageDialog *dial = new wxMessageDialog(NULL, wxLang.at(23), wxLang.at(24), wxOK);
	dial->ShowModal();
}

void Panel::ClearHistory(wxCommandEvent& event){
    ofstream plik(string("/home/" + h.getUserName() +"/.linux-ip-changer/history").c_str()); 
	plik<<""; 
	plik.close();
	ip->Clear();
	statusbar->SetStatusText(wxLang.at(20),0);
	ip->SetValue(wxT(""));
}
