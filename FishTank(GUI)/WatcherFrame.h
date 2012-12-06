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
    void OnSendMsg(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnClickCol(wxListEvent& event);
    void Refresh();
    //void OnChangeData(wxChangeDataEvent& event);
    SystemThread* CreateThread();
    wxTextCtrl *txtctrl;
    wxListCtrl* list;
private:
    FishInfo* const data;
    wxListItem colItems[9];
    wxListItem colItems_s[9][2];
    int sortOrder;
    int sortCol;
    void SortOnCol(int);

protected:
    DECLARE_EVENT_TABLE()
};

#endif // WATCHERFRAME_H
