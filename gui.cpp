#include "gui.h"
#include "change.h"
#include "tibiapid.h"
#include "history.h"
#include "languages.h"
#include "setup.h"
#include "clientsversions.h"
#include "network.h"
#include "ico.xpm"
#include "refresh.xpm"
#include "download.xpm"

Setup s;
History h;
Languages lang;
std::vector<wxString>wxLang;

BEGIN_EVENT_TABLE(Panel,wxFrame)
	EVT_CLOSE(Panel::OnClose)
	EVT_MENU(wxID_EXIT, Panel::PanelClickEvents)
	EVT_MENU(wxID_SETUP, Panel::PanelClickEvents)
	EVT_MENU(MENU_INFO, Panel::PanelClickEvents)
	EVT_MENU(MENU_CLEAR_HISTORY, Panel::PanelClickEvents)
	EVT_BUTTON(wxID_EXIT, Panel::PanelClickEvents)
	EVT_BUTTON(BUTTON_TO_TRAY, Panel::PanelClickEvents)
	EVT_BUTTON(BUTTON_REFRESH, Panel::PanelClickEvents)
	EVT_COMBOBOX(CB_IP, Panel::comboSelect)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyTaskBarIcon, wxTaskBarIcon)
    EVT_TASKBAR_LEFT_DCLICK(MyTaskBarIcon::TrayLeftClick)
    EVT_MENU(wxID_EXIT, MyTaskBarIcon::TrayClickEvents)
    EVT_MENU(MENU_O_M, MyTaskBarIcon::TrayClickEvents)
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

UpDialog::UpDialog(Panel* frame) 
: wxDialog(frame, wxID_ANY, wxT("Update"), wxDefaultPosition, wxSize(200, 170), wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP)
, frame(frame) 
{ 
	wxPanel *p = new wxPanel(this, wxID_ANY, wxPoint(-1, -1), wxSize(200, 170));
	(void) new wxStaticBitmap(p, wxID_ANY, wxBitmap(download_xpm), wxPoint(5, 5), wxSize(100, 100));
	(void) new wxStaticText(p, wxID_ANY, wxT("Update!"), wxPoint(125, 35), wxSize(50, 25));
	(void) new wxStaticText(p, wxID_ANY, wxLang.at(27), wxPoint(5, 115), wxSize(195, 50));
	link = new wxHyperlinkCtrl(p, HYPERLINK, wxLang.at(28), wxT("http://code.google.com/p/linux-tibia-ip-changer/downloads/list"), wxPoint(35, 147), wxDefaultSize, wxHL_DEFAULT_STYLE, wxHyperlinkCtrlNameStr);
	Connect(link->GetId(), wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler(UpDialog::useLink));
} 

Panel::Panel(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxPoint(-1, -1), wxSize(350, 170))
{
	panel = new wxPanel(this, wxID_ANY, wxPoint(-1, -1), wxSize(350, 170));
	SetMinSize(wxSize(350, 170));
	SetMaxSize(wxSize(350, 170));
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
	tibiaversion = new wxComboBox(panel, -1, tab[0], wxPoint(274, 60), wxSize(65, 25), WXSIZEOF(tab), tab, wxCB_READONLY);
	statusbar = new wxStatusBar(this, wxID_ANY);
	statusbar->SetFieldsCount(1);
	int stat[1];
	stat[0] = -1;
	statusbar->SetStatusWidths(1,stat);
	SetStatusBar(statusbar);
	(void) new wxStaticText(panel, wxID_ANY, wxLang.at(1), wxPoint(5, 5), wxSize(350, 25));
	(void) new wxStaticText(panel, wxID_ANY, wxLang.at(2), wxPoint(286, 5), wxSize(350, 25));
	(void) new wxStaticText(panel, wxID_ANY, wxLang.at(3), wxPoint(272, 47), wxSize(350, 25));
	(void) new wxStaticText(panel, wxID_ANY, wxLang.at(25), wxPoint(5, 47), wxSize(350, 25));
	vector<string>z;
	z = h.getHistory();
	wxString tabb[z.size()];
	for(int i = 0; i < z.size(); i++){
		wxString wxs(z[i].c_str(), wxConvUTF8);
		tabb[i] = wxs;
	}
	ip = new wxComboBox(panel, CB_IP, wxT(""), wxPoint(5, 20), wxSize(264, 25), WXSIZEOF(tabb), tabb);
	port = new wxSpinCtrl(panel, wxID_ANY, wxT("7171"), wxPoint(287, 20), wxSize(54, 25), wxSP_ARROW_KEYS, 1000, 9999, 7171);
	TibiaPid pid;
	wxArrayString arr = pid.getTibiaPidGUI(), arrTmp;
	if(arr.GetCount() != 0){
		for(int i = 0; i < arr.GetCount(); i++)
			arrTmp.Add(wxString(wxT("Tibia (") + arr[i] + wxT(")")));
	}else
		arrTmp.Add(wxLang.at(26));
	process = new wxComboBox(panel, -1, arrTmp[0], wxPoint(5, 60), wxSize(215, 25), arrTmp, wxCB_READONLY);
	wxMenuBar *menubar = new wxMenuBar;
	wxMenu *file = new wxMenu;
	file->Append(MENU_CLEAR_HISTORY, wxLang.at(4));
	file->Append(wxID_SETUP, wxLang.at(5));
	file->AppendSeparator(); 
	file->Append(wxID_EXIT, wxLang.at(6));
	menubar->Append(file, wxLang.at(7));
	wxMenu *help = new wxMenu;
	help->Append(MENU_INFO, wxLang.at(8));
	menubar->Append(help, wxLang.at(9));
	SetMenuBar(menubar);
	trayIcon = new MyTaskBarIcon(this);
	wxIcon icon(ico_xpm);
	if(!trayIcon->SetIcon(icon, wxT("IP Changer")))
        wxMessageBox(wxT("Could not set icon."));
    (void) new wxButton(panel, BUTTON_CHANGE_IP, wxLang.at(10), wxPoint(60, 90), wxSize(90, 33));
    Connect(BUTTON_CHANGE_IP, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Panel::PanelClickEvents));
    (void) new wxButton(panel, BUTTON_TO_TRAY, wxLang.at(11), wxPoint(155, 90), wxSize(90, 33));
    (void) new wxButton(panel, wxID_EXIT, wxLang.at(12), wxPoint(250, 90), wxSize(90, 33));
    (void) new wxBitmapButton(panel, BUTTON_REFRESH, wxBitmap(refresh_xpm), wxPoint(236, 60), wxSize(25, 25), wxBU_TOP);

	this->SetIcon(icon);
	Centre();
	Network n;
	std::string socketinfo = n.getIpCangerFromWebSite();
	if(socketinfo != ACTUALVERSION && socketinfo != ""){
		wxDialog *dial = new UpDialog(this);
		dial->Show();
	}
}

void sendDialog(wxString str, long styles){
	wxMessageDialog *dial = new wxMessageDialog(NULL, str, wxLang.at(24), styles);
	dial->ShowModal();
}

void guiChangeIP(Panel *frame, int eventId){
	TibiaPid pid;
	int tpid;
	wxArrayString arr = pid.getTibiaPidGUI();
	if(arr.GetCount() != 0){
		int select = frame->process->GetCurrentSelection();
		if(select == -1 || arr.GetCount() < select+1)
			select = 0;
		tpid = wxAtoi(arr[select]);
	}
	if(tpid == 0){
		frame->statusbar->SetStatusText(wxLang.at(16),0);
		if(eventId == MENU_CHANGE_IP)
			sendDialog(wxLang.at(16), wxOK | wxICON_ERROR);
		return;
	}

	std::string sip = std::string(frame->ip->GetValue().mb_str());
	int sport = frame->port->GetValue();
	std::string client = std::string(frame->tibiaversion->GetValue().mb_str());
	if(sip == ""){
		frame->statusbar->SetStatusText(wxLang.at(17),0);
		if(eventId == MENU_CHANGE_IP)
			sendDialog(wxLang.at(17), wxOK | wxICON_ERROR);
		return;
	}
	
    size_t found = sip.find(":");
    if(found != string::npos){
        sport = atoi(sip.substr(found+1, sip.size()).c_str());
        sip = sip.substr(0, found);
        wxString wxip(sip.c_str(), wxConvUTF8);
        frame->ip->SetValue(wxip);
        frame->port->SetValue(sport);
    }

	if(client != "Auto"){
		std::string s ="";
		s += client.at(1);
		if(s == ".")
			client = client.erase(1,1);
	}

	Change c(tpid);	
	bool ret = c.changeIP(sip, sport, client);

	if(ret){
		frame->statusbar->SetStatusText(wxLang.at(18),0);
		if(eventId == MENU_CHANGE_IP)
			sendDialog(wxString(wxLang.at(18) + wxT("\n") + wxLang.at(1) + wxT(" ") + frame->ip->GetValue() + wxT("\n") + wxLang.at(2) + wxT(" ") + wxString::Format(wxT("%i"), sport)), wxOK);
	}else{
		frame->statusbar->SetStatusText(wxLang.at(19),0);
		if(eventId == MENU_CHANGE_IP)
			sendDialog(wxLang.at(16), wxOK | wxICON_ERROR);
		return;
	}

    char buf[4];
    sprintf(buf,"%d",sport);
	h.saveHistory(std::string(sip + ":" + buf));
	frame->ip->Clear();
	vector<string>z;
	z = h.getHistory();
	for(int i = 0; i < z.size(); i++){
		wxString wxs(z[i].c_str(), wxConvUTF8);
		frame->ip->Append(wxs);
	}
}

void Panel::comboSelect(wxCommandEvent& event){
    std::string sip = std::string(event.GetString().mb_str());
    size_t found = sip.find(":");
    if(found != string::npos){
        int host = atoi(sip.substr(found+1, sip.size()).c_str());
        sip = sip.substr(0, found);
        wxString wxs(sip.c_str(), wxConvUTF8);
        ip->SetValue(wxs);
        port->SetValue(host);
    }
}

void Panel::OnClose(wxCloseEvent& event){
	Iconize(true);
	Show(false);
}

void Panel::PanelClickEvents(wxCommandEvent& event){ 
	switch(event.GetId()){
		case wxID_EXIT:{
			trayIcon->RemoveIcon();
			Destroy();
			break;
		}
		case BUTTON_TO_TRAY:{
			Iconize(true);
			Show(false);
			break;
		}
		case BUTTON_CHANGE_IP:{
			guiChangeIP(this, BUTTON_CHANGE_IP);
			break;
		}
		case MENU_INFO:{
			sendDialog(wxT("This program change IP tibia client in order to connect to Open Tibia Server.\nContinue Moraxus project.\nLicense: GNU GPL\nProgrammer: Miziak.\nAdress Finder: Virtelio.\nWrote in wxWidgets library."), wxOK);
			break;
		}
		case wxID_SETUP:{
			wxDialog *dial = new MyDialog(this);
			dial->Show();
			break;
		}
		case MENU_CLEAR_HISTORY:{
			ofstream plik(string("/home/" + h.getUserName() +"/.linux-ip-changer/history").c_str()); 
			plik<<""; 
			plik.close();
			ip->Clear();
			statusbar->SetStatusText(wxLang.at(20),0);
			ip->SetValue(wxT(""));
			break;
		}
		case BUTTON_REFRESH:{
			TibiaPid pid;
			wxArrayString arr = pid.getTibiaPidGUI();
			if(arr.GetCount() != 0){
				process->Clear();
				for(int i = 0; i < arr.GetCount(); i++)
					process->Append(wxString(wxT("Tibia (") + arr[i] + wxT(")")));
				process->SetValue(wxString(wxT("Tibia (") + arr[0] + wxT(")")));
			}else{
				process->Clear();
				process->Append(wxLang.at(26));
				process->SetValue(wxLang.at(26));
			}
			break;
		}
	}
}

void MyDialog::Ok(wxCommandEvent& event){
	s.saveSetup("lang", std::string(langCombo->GetValue().mb_str()));
	this->Destroy();
	wxMessageDialog *dial = new wxMessageDialog(NULL, wxLang.at(23), wxLang.at(24), wxOK);
	dial->ShowModal();
}

void UpDialog::useLink(wxHyperlinkEvent& event){
	wxLaunchDefaultBrowser(event.GetURL());
	this->Destroy();
}

void MyTaskBarIcon::TrayClickEvents(wxCommandEvent& event){ 
	switch(event.GetId()){
		case wxID_EXIT:{
			RemoveIcon();
			frame->Destroy();
			break;
		}
		case MENU_O_M:{
			if(frame->IsIconized()){
				frame->Iconize(false);
				frame->Show(true);
			}else{
				frame->Iconize(true);
				frame->Show(false);	
			}
			break;
		}
		case MENU_CHANGE_IP:{
			guiChangeIP(frame, MENU_CHANGE_IP);
			break;
		}
	}
}

void MyTaskBarIcon::TrayLeftClick(wxTaskBarIconEvent& event){
	if(frame->IsIconized()){
		frame->Iconize(false);
		frame->Show(true);
	}else{
		frame->Iconize(true);
		frame->Show(false);	
	}
}

wxMenu* MyTaskBarIcon::CreatePopupMenu() 
{ 
    menuTray = new wxMenu();
    menuTray->Append(MENU_CHANGE_IP, wxLang.at(10));
    Connect(MENU_CHANGE_IP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyTaskBarIcon::TrayClickEvents));
	if(frame->IsIconized())
		menuTray->Append(MENU_O_M, wxLang.at(13));
	else
		menuTray->Append(MENU_O_M, wxLang.at(14));
    menuTray->AppendSeparator(); 
    menuTray->Append(wxID_EXIT, wxLang.at(15)); 

    return menuTray; 
}
