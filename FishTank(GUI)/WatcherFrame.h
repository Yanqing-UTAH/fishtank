#ifndef WATCHERFRAME_H
#define WATCHERFRAME_H

#include "utildef.h"
#include "Events.h"
#include "FishInfo.h"
#include "wx/listctrl.h"
#include "wx/notebook.h"

class WatcherFrame: public wxFrame
{
public:
    WatcherFrame(FishInfo* info,
                 const wxString& title,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxDEFAULT_FRAME_STYLE);
    void OnSendMsg(wxSendMsgEvent& event);
    void OnClose(wxCloseEvent& event);
    //void OnChangeData(wxChangeDataEvent& event);
    SystemThread* CreateThread();
    wxTextCtrl *txtctrl;
    wxListCtrl* list;
private:
    FishInfo* const data;
protected:
    DECLARE_EVENT_TABLE()
};

#endif // WATCHERFRAME_H
