#ifndef EVENTS_H
#define EVENTS_H

#include "utildef.h"

class wxSetMapEvent : public wxNotifyEvent
{
public:
    wxSetMapEvent(int a = 0, int b = 0, wxEventType notifyType = wxEVT_NULL, int id = 0) :
        wxNotifyEvent(notifyType, id),
        x(a),
        y(b)
    {}
    virtual wxEvent* Clone() const
    {
        return new wxSetMapEvent(*this);
    }
    int GetX() const
    {
        return x;
    }
    int GetY() const
    {
        return y;
    }
private:
    int x, y;
    DECLARE_DYNAMIC_CLASS(wxSetMapEvent);
};
typedef void (wxEvtHandler::*wxSetMapFunction) (wxSetMapEvent&);

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_LOCAL_EVENT_TYPE(wxEVT_SET_MAP, 802);
DECLARE_LOCAL_EVENT_TYPE(wxEVT_SEND_MSG, 803);
DECLARE_LOCAL_EVENT_TYPE(wxEVT_SEND_SB, 804);
DECLARE_LOCAL_EVENT_TYPE(wxEVT_CHANGE_DATA, 805);
DECLARE_LOCAL_EVENT_TYPE(wxEVT_GAME_OVER, 806);
END_DECLARE_EVENT_TYPES()
#define EVT_SET_MAP(id, fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_SET_MAP, id, -1, (wxObjectEventFunction)(wxEventFunction)(wxSetMapFunction) & fn, (wxObject *) NULL),

#endif // EVENTS_H
