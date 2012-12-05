#ifndef EVENTS_H
#define EVENTS_H

#include "utildef.h"

class wxSetMapEvent : public wxNotifyEvent
{
public:
    wxSetMapEvent(int a = 0, int b = 0, int c = EMPTY, wxEventType notifyType = wxEVT_NULL, int id = 0) :
        wxNotifyEvent(notifyType, id),
        x(a),
        y(b),
        target(c)
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
    int GetTarget() const
    {
        return target;
    }
private:
    int x, y, target;
    DECLARE_DYNAMIC_CLASS(wxSetMapEvent);
};
typedef void (wxEvtHandler::*wxSetMapFunction) (wxSetMapEvent&);

class wxSendMsgEvent : public wxNotifyEvent
{
public:
    wxSendMsgEvent(wxString str = "", wxEventType notifyType = wxEVT_NULL, int id = 0) :
        wxNotifyEvent(notifyType, id),
        msg(str)
    {}
    virtual wxEvent* Clone() const
    {
        return new wxSendMsgEvent(*this);
    }
    wxString GetMsg() const
    {
        return msg;
    }
private:
    wxString msg;
    DECLARE_DYNAMIC_CLASS(wxSendMsgEvent);
};
typedef void (wxEvtHandler::*wxSendMsgFunction) (wxSendMsgEvent&);

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_LOCAL_EVENT_TYPE(wxEVT_SET_MAP, 802);
DECLARE_LOCAL_EVENT_TYPE(wxEVT_SEND_MSG, 803);
DECLARE_LOCAL_EVENT_TYPE(wxEVT_SEND_STATUS, 804);
DECLARE_LOCAL_EVENT_TYPE(wxEVT_CHANGE_LV, 805);
END_DECLARE_EVENT_TYPES()
#define EVT_SET_MAP(id, fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_SET_MAP, id, -1, (wxObjectEventFunction)(wxEventFunction)(wxSetMapFunction) & fn, (wxObject *) NULL),
#define EVT_SEND_MSG(id, fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_SEND_MSG, id, -1, (wxObjectEventFunction)(wxEventFunction)(wxSendMsgFunction) & fn, (wxObject *) NULL),
#define EVT_SEND_STATUS(id, fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_SEND_STATUS, id, -1, (wxObjectEventFunction)(wxEventFunction)(wxSendMsgFunction) & fn, (wxObject *) NULL),

#endif // EVENTS_H
