#ifndef ENV_H
#define ENV_H

#include "utildef.h"
#include <cstdlib>
#include <fstream>
#include "wx/button.h"

class env : public wxThread
{
public:
    env(wxFrame* frame);
    void init();
    virtual ~env();
    virtual void* Entry();

    bool addAI(fish*);
    void play();

private:
    void foodRefresh();
    bool sortCmp(int, int) const;
    void sort(int, int);
    void fishInit();
    void fishRevive();
    void fishPlay();
    void calcPriority();
    void randXY(int&, int&);
    void printResult();
    int larger(int, int) const;
    int dis(int, int, int, int) const;
    int dis(int, int) const;
    bool validCor(int, int) const;

private:
    int current;
    fish* player[MAX_PLAYER+1];
    int fishNum;

    int map[N+1][M+1];
    int fishPoint[MAX_PLAYER+1];
    int fishLevel[MAX_PLAYER+1];
    int fishExp[MAX_PLAYER+1];
    int fishX[MAX_PLAYER+1];
    int fishY[MAX_PLAYER+1];
    int fishHP[MAX_PLAYER+1];
    int fishMaxHP[MAX_PLAYER+1];
    int fishAtt[MAX_PLAYER+1];
    int fishSp[MAX_PLAYER+1];
    int fishBonus[MAX_PLAYER+1];
    int fishKill[MAX_PLAYER+1];
    int fishDie[MAX_PLAYER+1];
    int fishRank[MAX_PLAYER+1];

    int reviveList[MAX_PLAYER+1];
    int reviveNum;

    int phase;

    enum
    {
        INIT_PHASE,
        MAIN_PHASE,
        MOVE_PHASE,
        ATTACK_PHASE,
        REVIVE_PHASE
    };

    int sequence[MAX_PLAYER+1];

public:
    bool move(int, int);
    bool attack(int, int);
    int getPoint() const;
    int getLevel() const;
    int getExp() const;
    int getX() const;
    int getY() const;
    int getHP() const;
    int getMaxHP() const;
    int getAtt() const;
    int getSp() const;
    int getID() const;
    int askWhat(int, int) const;
    int askWhatF(int, int) const;
    int askHP(int) const;
    bool increaseHealth();
    bool increaseStrength();
    bool increaseSpeed();

private:
    void setMap(int, int, int);
    void increasePoint(int);
    void increaseLevel(int);
    void increaseExp(int);
    void increaseHP(int);
    void decreaseHP(int, int);
    void setPoint(int, int);
    void setLevel(int, int);
    void setExp(int, int);
    void setXY(int, int, int);
    void setHP(int, int);
    void setMaxHP(int, int);
    void setAtt(int, int);
    void setSp(int, int);

    wxFrame* myFrame;
};

class wxSetMapEvent : public wxNotifyEvent
{
public:
    wxSetMapEvent(int a = 0, int b = 0, int c = EMPTY, wxEventType notifyType = wxEVT_NULL, int id = 0) :
        wxNotifyEvent(notifyType, id),
        x(a),
        y(b),
        target(c)
    {}
    virtual wxEvent* Clone() const {return new wxSetMapEvent(*this);}
    int GetX() const {return x;}
    int GetY() const {return y;}
    int GetTarget() const {return target;}
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
    virtual wxEvent* Clone() const {return new wxSendMsgEvent(*this);}
    wxString GetMsg() const {return msg;}
private:
    wxString msg;
DECLARE_DYNAMIC_CLASS(wxSendMsgEvent);
};
typedef void (wxEvtHandler::*wxSendMsgFunction) (wxSendMsgEvent&);

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_SET_MAP, 802);
    DECLARE_EVENT_TYPE(wxEVT_SEND_MSG, 803);
    DECLARE_EVENT_TYPE(wxEVT_SEND_STATUS, 804);
END_DECLARE_EVENT_TYPES()
#define EVT_SET_MAP(id, fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_SET_MAP, id, -1, (wxObjectEventFunction)(wxEventFunction)(wxSetMapFunction) & fn, (wxObject *) NULL),
#define EVT_SEND_MSG(id, fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_SEND_MSG, id, -1, (wxObjectEventFunction)(wxEventFunction)(wxSendMsgFunction) & fn, (wxObject *) NULL),
#define EVT_SEND_STATUS(id, fn) DECLARE_EVENT_TABLE_ENTRY(wxEVT_SEND_STATUS, id, -1, (wxObjectEventFunction)(wxEventFunction)(wxSendMsgFunction) & fn, (wxObject *) NULL),

class FishTankApp: public wxApp
{
    virtual bool OnInit();
};

class FishTankFrame: public wxFrame
{
public:
    FishTankFrame(const wxString& title,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize,
                    long style = wxDEFAULT_FRAME_STYLE);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnStart(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnResume(wxCommandEvent& event);
    void OnReset(wxCommandEvent& event);
    void OnSetMap(wxSetMapEvent& event);
    void OnSendMsg(wxSendMsgEvent& event);
    void OnSendStatus(wxSendMsgEvent& event);
    void OnPaint(wxPaintEvent& event);
    env* CreateThread();
    wxButton* BtnStart;
    wxButton* BtnPause;
    wxButton* BtnResume;
    wxButton* BtnReset;
    wxTextCtrl *m_txtctrl;
private:
    env* thread;
    static const int BoardLength = 600;
    static const int BoardX = 15;
    static const int BoardY = 15;
protected:
    DECLARE_EVENT_TABLE()
};

inline int env::larger(int a, int b) const
{
    return a > b ? a : b;
}

inline int env::dis(int x1, int y1, int x2, int y2) const
{
    return (std::abs(x1 - x2) + std::abs(y1 - y2));
}

inline int env::dis(int x, int y) const
{
    return dis(x, y, getX(), getY());
}

inline bool env::validCor(int x, int y) const
{
    return ((x >= 1) && (x <= N) && (y >= 1) && (y <= M));
}

#endif // ENV_H
