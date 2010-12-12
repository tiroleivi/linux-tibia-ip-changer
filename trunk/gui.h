#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/menu.h>
#include <wx/string.h>
#include <wx/combobox.h>
#include <wx/statusbr.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/menu.h>
#include <wx/taskbar.h>

enum{
	MENU_O_M = 101,
	BUTTON_CHANGE_IP = 102,
	MENU_INFO = 103,
	BUTTON_TO_TRAY = 104
};

class Panel : public wxFrame
{
public:
	Panel(const wxString& title);
	
private:
	wxPanel *panel;
	wxComboBox *tibiaversion;
	wxStatusBar *statusbar;
	wxStaticText *statictext;
	wxTextCtrl *ip;
	wxTextCtrl *port;
	wxMenuBar *menubar;
    wxMenu *file;
    wxMenu *help;
    wxTaskBarIcon *trayIcon;
	//Functions
	void OnClose(wxCloseEvent& event);
	void Quit(wxCommandEvent& event);
	void guiChangeIP(wxCommandEvent& event);
	void Info(wxCommandEvent& event);
	void toTray(wxCommandEvent& event);
	DECLARE_EVENT_TABLE();
};

class MyTaskBarIcon: public wxTaskBarIcon
{
public:
    MyTaskBarIcon(Panel* frame);
	Panel* frame;
	virtual wxMenu* CreatePopupMenu();
private:
	wxMenu *menuTray;
	void trayLeftClick(wxTaskBarIconEvent& event);
	void Quit(wxCommandEvent& event);
	void miniOrMax(wxCommandEvent& event);
DECLARE_EVENT_TABLE()
};
