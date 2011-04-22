#ifndef GUI_H
#define GUI_H

#include <wx/wx.h>
#include <wx/taskbar.h>
#include <wx/spinctrl.h>
#include <wx/hyperlink.h>

enum{
	MENU_O_M = 101,
	BUTTON_CHANGE_IP = 102,
	MENU_INFO = 103,
	BUTTON_TO_TRAY = 104,
	ID_COMBO = 105,
	MENU_CLEAR_HISTORY = 106,
	BUTTON_OK = 107,
	MENU_CHANGE_IP = 108,
	BUTTON_REFRESH = 109,
	HYPERLINK = 110,
	CB_IP = 111
};

class Panel : public wxFrame
{
public:
	Panel(const wxString& title);
	wxComboBox *ip;
	wxSpinCtrl *port;
	wxComboBox *tibiaversion;
	wxComboBox *process;
	wxStatusBar *statusbar;
	
private:
	wxPanel *panel;
    wxTaskBarIcon *trayIcon;
	//Functions
	void OnClose(wxCloseEvent& event);
	void PanelClickEvents(wxCommandEvent& event);
	void comboSelect(wxCommandEvent& event);
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
	void TrayLeftClick(wxTaskBarIconEvent& event);
	void TrayClickEvents(wxCommandEvent& event);
DECLARE_EVENT_TABLE()
};

class MyDialog: public wxDialog
{
public:
    MyDialog(Panel* frame);
	Panel* frame;
	wxComboBox *langCombo;
private:
	void Ok(wxCommandEvent& event);
};

class UpDialog: public wxDialog{
private:
	wxHyperlinkCtrl *link;
	void useLink(wxHyperlinkEvent& event);
public:
	UpDialog(Panel* frame);
	Panel* frame;
};

#endif //GUI_H
