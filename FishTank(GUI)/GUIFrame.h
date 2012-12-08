#ifndef GUIFRAME_H
#define GUIFRAME_H

#include "utildef.h"
#include "Events.h"
#include "FishInfo.h"
#include "SystemThread.h"
#include "wx/listctrl.h"

class GUIFrame: public wxFrame
{
public:
    GUIFrame(FishInfo* info,
             SystemThread* env,
             const wxString& title,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxDEFAULT_FRAME_STYLE);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnStart(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnResume(wxCommandEvent& event);
    void OnStop(wxCommandEvent& event);
    void OnSetMap(wxSetMapEvent& event);
    void OnSendStatusBar(wxCommandEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnSendMsg(wxCommandEvent& event);
    void OnClickCol(wxListEvent& event);
    void OnRefreshList(wxCommandEvent& event);
    void PaintGrid(int, int);
private:
    static const int BoardLength = 600;
    static const int BoardX = 15;
    static const int BoardY = 15;
    FishInfo* const data;
    SystemThread* const thread;
    wxTextCtrl *txtctrl;
    wxListCtrl* list;
    wxListItem colItems[9];
    wxListItem colItems_s[9][2];
    int sortOrder;
    int sortCol;
    void SortOnCol(int);
protected:
    DECLARE_EVENT_TABLE()
};

#endif // GUIFRAME_H
