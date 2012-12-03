#include "env.h"
#include "fish.h"

#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include "TAAI.h"

using namespace std;

DEFINE_EVENT_TYPE(wxEVT_SET_MAP)
DEFINE_EVENT_TYPE(wxEVT_SEND_MSG)
DEFINE_EVENT_TYPE(wxEVT_SEND_STATUS)
IMPLEMENT_DYNAMIC_CLASS(wxSetMapEvent, wxNotifyEvent)
IMPLEMENT_DYNAMIC_CLASS(wxSendMsgEvent, wxNotifyEvent)

wxThread::ExitCode env::Entry()
{
    for (int i = 1; i <= MAX_PLAYER; ++i)
    {
        fish* AI = new TAAI();
        AI->setHost(this);
        addAI(AI);
    }
    play();
    return NULL;
}

env::env(wxFrame* frame) : myFrame(frame)
{
    init();
}

void env::init()
{
    fishNum = 0;
    reviveNum = 0;
    phase = INIT_PHASE;
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
            setMap(i, j, EMPTY);
    srand(time(NULL));
}

env::~env()
{
    for (int i = 1; i <= MAX_PLAYER; ++i)
        delete player[i];
}

bool env::addAI(fish* newAI)
{
    if ((fishNum < MAX_PLAYER) && (newAI))
    {
        player[++fishNum] = newAI;
        return true;
    }
    return false;
}

void env::foodRefresh()
{
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
            if (askWhat(i, j) == FOOD)
                setMap(i, j, EMPTY);
    for (int i = 1; i <= MAX_FOOD; ++i)
    {
        int x = 0, y = 0;
        randXY(x, y);
        setMap(x, y, FOOD);
    }
    myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Food refreshed"), wxEVT_SEND_MSG, SEND_MSG_ID));
}

void env::fishInit()
{
    phase = INIT_PHASE;
    for (int i = 1; i <= fishNum; ++i)
    {
        int x = 0, y = 0;
        randXY(x, y);
        setMap(x, y, i);
        setPoint(i, 10);
        setLevel(i, 1);
        setExp(i, 0);
        setXY(i, x, y);
        setHP(i, 0);
        setMaxHP(i, 0);
        setAtt(i, 0);
        setSp(i, 0);
        fishBonus[i] = 0;
        fishKill[i] = 0;
        fishDie[i] = 0;    }
    for (int i = 1; i <= fishNum; ++i)
    {
        current = i;
        player[i]->init();
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d borns at (%d, %d), MaxHP = %d, Sp = %d, Att = %d", i, getX(), getY(), getMaxHP(), getSp(), getAtt()), wxEVT_SEND_MSG, SEND_MSG_ID));
    }
}

void env::calcPriority()
{
    for (int i = 1; i <= fishNum; ++i)
        sequence[i] = i;
    sort(1, fishNum);
}

void env::fishRevive()
{
    for (int i = 1; i <= reviveNum; ++i)
    {
        phase = REVIVE_PHASE;
        int x = 0, y = 0, target = reviveList[i];
        current = reviveList[i];
        player[target]->revive(x, y);
        if (!validCor(x, y) || (askWhat(x, y) != EMPTY))
            randXY(x, y);
        setMap(x, y, target);
        setXY(target, x, y);
        setHP(target, larger(fishMaxHP[target] / 10, 1));
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d revives at (%d, %d)", target, x, y), wxEVT_SEND_MSG, SEND_MSG_ID));
    }
    reviveNum = 0;
    for (int i = 1; i <= fishNum; ++i)
        if (askHP(i) <= 0)
            reviveList[++reviveNum] = i;
}

void env::fishPlay()
{
    for (int i = 1; i <= fishNum; ++i)
    {
        phase = MAIN_PHASE;
        current = sequence[i];
        if (getHP() > 0)
        {
            myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d is in action now", current), wxEVT_SEND_MSG, SEND_MSG_ID));
            myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d is in action now", current), wxEVT_SEND_STATUS, SEND_STATUS_ID));
            player[current]->play();
            Sleep(50);
        }
    }
}

void env::play()
{
    fishInit();
    for (int round = 1; round <= GAME_ROUND; ++round)
    {
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Round: %d", round), wxEVT_SEND_MSG, SEND_MSG_ID));
        if (round % 5 == 1)
            foodRefresh();
        fishRevive();
        calcPriority();
        fishPlay();
    }
    printResult();
}

bool env::move(int x, int y)
{
    if (phase != MAIN_PHASE)
    {
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d attempts to move from (%d, %d) to (%d, %d)", current, getX(), getY(), x, y), wxEVT_SEND_MSG, SEND_MSG_ID));
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Failed (Cannot move in this phase)"), wxEVT_SEND_MSG, SEND_MSG_ID));
        return false;
    }
    if (!validCor(x, y))
    {
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d attempts to move from (%d, %d) to (%d, %d)", current, getX(), getY(), x, y), wxEVT_SEND_MSG, SEND_MSG_ID));
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Failed (Invalid coordinate)"), wxEVT_SEND_MSG, SEND_MSG_ID));
        return false;
    }
    if (askWhat(x, y) != EMPTY)
    {
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d attempts to move from (%d, %d) to (%d, %d)", current, getX(), getY(), x, y), wxEVT_SEND_MSG, SEND_MSG_ID));
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Failed (Target area is not empty)"), wxEVT_SEND_MSG, SEND_MSG_ID));
        return false;
    }
    if (dis(x, y) > getSp())
    {
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d attempts to move from (%d, %d) to (%d, %d)", current, getX(), getY(), x, y), wxEVT_SEND_MSG, SEND_MSG_ID));
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Failed (Out of move range)"), wxEVT_SEND_MSG, SEND_MSG_ID));
        return false;
    }
    myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d moves from (%d, %d) to (%d, %d)", current, getX(), getY(), x, y), wxEVT_SEND_MSG, SEND_MSG_ID));
    setMap(getX(), getY(), EMPTY);
    setXY(current, x, y);
    setMap(x, y, current);
    phase = MOVE_PHASE;
    return true;
}

bool env::attack(int x, int y)
{
    if ((phase != MAIN_PHASE) && (phase != MOVE_PHASE))
    {
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d attempts to attack (%d, %d)", current, x, y), wxEVT_SEND_MSG, SEND_MSG_ID));
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Failed (Cannot attack in this phase)"), wxEVT_SEND_MSG, SEND_MSG_ID));
        return false;
    }
    if (!validCor(x, y))
    {
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d attempts to attack (%d, %d)", current, x, y), wxEVT_SEND_MSG, SEND_MSG_ID));
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Failed (Invalid coordinate)"), wxEVT_SEND_MSG, SEND_MSG_ID));
        return false;
    }
    if (dis(x, y) != 1)
    {
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d attempts to attack (%d, %d)", current, x, y), wxEVT_SEND_MSG, SEND_MSG_ID));
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Failed (Out of attack range)"), wxEVT_SEND_MSG, SEND_MSG_ID));
        return false;
    }
    if (askWhat(x, y) == EMPTY)
    {
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d attempts to attack (%d, %d)", current, x, y), wxEVT_SEND_MSG, SEND_MSG_ID));
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Failed (Target area is empty)"), wxEVT_SEND_MSG, SEND_MSG_ID));
        return false;
    }
    if (askWhat(x, y) == FOOD)
    {
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d eats food at (%d, %d)", current, x, y), wxEVT_SEND_MSG, SEND_MSG_ID));
        setMap(x, y, EMPTY);
        increaseHP(larger(getMaxHP() / 10, 2));
        increaseExp(1);
    }
    else
    {
        int target = askWhat(x, y);
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d attacks Fish %d at (%d, %d)", current, target, x, y), wxEVT_SEND_MSG, SEND_MSG_ID));
        decreaseHP(target, getAtt());
        if (!askHP(target))
        {
            setMap(x, y, EMPTY);
            if (fishLevel[target] > getLevel())
                fishBonus[current] += 2 * (fishLevel[target] - getLevel());
            increaseExp(larger(1, fishLevel[target] / 2));
            ++fishKill[current];
            ++fishDie[target];
        }
    }
    phase = ATTACK_PHASE;
    return true;
}

int env::getPoint() const
{
    return fishPoint[current];
}

int env::getLevel() const
{
    return fishLevel[current];
}

int env::getExp() const
{
    return fishExp[current];
}

int env::getX() const
{
    return fishX[current];
}

int env::getY() const
{
    return fishY[current];
}

int env::getHP() const
{
    return fishHP[current];
}

int env::getMaxHP() const
{
    return fishMaxHP[current];
}

int env::getAtt() const
{
    return fishAtt[current];
}

int env::getSp() const
{
    return fishSp[current];
}

int env::getID() const
{
    return current;
}

int env::askWhat(int x, int y) const
{
    if (phase != REVIVE_PHASE)
        if (validCor(x, y))
            return map[x][y];
        return EMPTY;
}


int env::askWhatF(int x, int y) const
{
    if (phase != REVIVE_PHASE)
    {
        if (validCor(x, y))
            return map[x][y];
        return EMPTY;
    }
    return EMPTY;
}


int env::askHP(int id) const
{
    if ((id >= 1) && (id <= fishNum))
        return fishHP[id];
    return 0;
}

void env::increaseHP(int value)
{
    if (value <= 0)
        return;
    int HP = getHP() + value;
    if (HP > getMaxHP())
        HP = getMaxHP();
    setHP(current, HP);
    if (phase != INIT_PHASE)
    myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d's HP increases to %d", current, HP), wxEVT_SEND_MSG, SEND_MSG_ID));
}

void env::decreaseHP(int target, int value)
{
    if (value <= 0)
        return;
    int HP = askHP(target) - value;
    if (HP < 0)
        HP = 0;
    setHP(target, HP);
    myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d's HP decreases to %d", target, HP), wxEVT_SEND_MSG, SEND_MSG_ID));
    if (!HP)
    {
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d is killed", target), wxEVT_SEND_MSG, SEND_MSG_ID));
    }
}

void env::increaseExp(int value)
{
    setExp(current, getExp() + value);
    myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d's Exp increases to %d", current, getExp()), wxEVT_SEND_MSG, SEND_MSG_ID));
    while (getExp() >= (2 + getLevel() + 1) * getLevel() / 2)
    {
        setLevel(current, getLevel() + 1);
        setPoint(current, getPoint() + 3);
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d's Level is upgraded to %d", current, getLevel()), wxEVT_SEND_MSG, SEND_MSG_ID));
    }
}

bool env::increaseHealth()
{
    if (getPoint())
    {
        setMaxHP(current, getMaxHP() + 2);
        increaseHP(2);
        setPoint(current, getPoint() - 1);
        if (phase != INIT_PHASE)
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d's MaxHP increases to %d", current, getMaxHP()), wxEVT_SEND_MSG, SEND_MSG_ID));
        return true;
    }
    return false;
}

bool env::increaseStrength()
{
    if (getPoint())
    {
        setAtt(current, getAtt() + 1);
        setPoint(current, getPoint() - 1);
        if (phase != INIT_PHASE)
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d's Strength increases to %d", current, getAtt()), wxEVT_SEND_MSG, SEND_MSG_ID));
        return true;
    }
    return false;
}

bool env::increaseSpeed()
{
    if (getPoint())
    {
        setSp(current, getSp() + 1);
        setPoint(current, getPoint() - 1);
        if (phase != INIT_PHASE)
        myFrame->GetEventHandler()->QueueEvent(new wxSendMsgEvent(wxString::Format("Fish %d's Speed increases to %d", current, getSp()), wxEVT_SEND_MSG, SEND_MSG_ID));
        return true;
    }
    return false;
}

bool env::sortCmp(int t1, int t2) const
{
    int x1 = fishSp[t1];
    int x2 = fishSp[t2];
    if (x1 < x2) return true;
    if (x1 > x2) return false;
    double y1 = fishMaxHP[t1] == 0 ? 0 : fishHP[t1] / fishMaxHP[t1];
    double y2 = fishMaxHP[t2] == 0 ? 0 : fishHP[t2] / fishMaxHP[t2];
    if (y1 < y2) return true;
    if (y1 > y2) return false;
    int z1 = fishExp[t1] + fishBonus[t1];
    int z2 = fishExp[t2] + fishBonus[t2];
    if (z1 > z2) return true;
    if (z1 < z2) return false;
    return false;
}

void env::sort(int l, int r)
{
    int i = l;
    int j = r;
    int m = sequence[(l + r) >> 1];
    while (i < j)
    {
        while (sortCmp(sequence[j], m)) --j;
        while (sortCmp(m, sequence[i])) ++i;
        if (i <= j)
        {
            int t = sequence[i];
            sequence[i] = sequence[j];
            sequence[j] = t;
            ++i;
            --j;
        }
    }
    if (l < j) sort(l, j);
    if (i < r) sort(i, r);
}

void env::randXY(int& x, int& y)
{
    int x0 = rand() % N + 1;
    int y0 = rand() % M + 1;
    while (askWhat(x0, y0) != EMPTY)
    {
        x0 = rand() % N + 1;
        y0 = rand() % M + 1;
    }
    x = x0;
    y = y0;
}

void env::setMap(int x, int y, int target)
{
    if (validCor(x, y))
    {
        map[x][y] = target;
        wxSetMapEvent* event = new wxSetMapEvent(x, y, target, wxEVT_SET_MAP, SET_MAP_ID);
        myFrame->GetEventHandler()->QueueEvent(event);
    }
}

void env::setXY(int target, int x, int y)
{
    fishX[target] = x;
    fishY[target] = y;
}

void env::setHP(int target, int value)
{
    fishHP[target] = value;
}

void env::setMaxHP(int target, int value)
{
    fishMaxHP[target] = value;
}

void env::setAtt(int target, int value)
{
    fishAtt[target] = value;
}

void env::setSp(int target, int value)
{
    fishSp[target] = value;
}

void env::setExp(int target, int value)
{
    fishExp[target] = value;
}

void env::setLevel(int target, int value)
{
    fishLevel[target] = value;
}

void env::setPoint(int target, int value)
{
    fishPoint[target] = value;
}

void env::printResult()
{
    for (int i = 1; i <= fishNum; ++i)
        fishRank[i] = i;
    cout << "ID Score  Lv   HP MaxHP  Sp Att Kill Die" << endl;
    for (int j = 1; j <= fishNum; ++j)
    {
        int i = fishRank[j];
        cout << setw(3) << i << setw(5) << fishExp[i] + fishBonus[i] << setw(4) << fishLevel[i] << setw(5) << fishHP[i] << setw(6) << fishMaxHP[i] << setw(4) << fishSp[i] << setw(4) << fishAtt[i] << setw(5) << fishKill[i] << setw(4) << fishDie[i] << endl;
    }
}


DECLARE_APP(FishTankApp)
IMPLEMENT_APP(FishTankApp)

BEGIN_EVENT_TABLE(FishTankFrame, wxFrame)
    EVT_MENU(wxID_EXIT, FishTankFrame::OnQuit)
    EVT_MENU(wxID_ABOUT, FishTankFrame::OnAbout)
    EVT_PAINT(FishTankFrame::OnPaint)
    EVT_BUTTON(START_ID, FishTankFrame::OnStart)
    EVT_BUTTON(PAUSE_ID, FishTankFrame::OnPause)
    EVT_BUTTON(RESUME_ID, FishTankFrame::OnResume)
    EVT_BUTTON(RESET_ID, FishTankFrame::OnReset)
    EVT_SET_MAP(SET_MAP_ID, FishTankFrame::OnSetMap)
    EVT_SEND_MSG(SEND_MSG_ID, FishTankFrame::OnSendMsg)
    EVT_SEND_STATUS(SEND_STATUS_ID, FishTankFrame::OnSendStatus)
END_EVENT_TABLE()

bool FishTankApp::OnInit()
{
    FishTankFrame* frame = new FishTankFrame(wxT("FishTank system"), wxDefaultPosition, wxGetDisplaySize(), wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN | wxFULL_REPAINT_ON_RESIZE);
    frame -> Show(true);
    SetTopWindow(frame);
    return true;
}

FishTankFrame::FishTankFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    :wxFrame((wxFrame*)NULL, -1, title, pos, size, style)
{
    SetIcon(wxICON(wxWidgets));
    wxMenu* menuFile = new wxMenu;
    menuFile -> Append(wxID_EXIT, wxT("E&xit"), wxT("Quit"));
    wxMenu* menuHelp = new wxMenu;
    menuHelp -> Append(wxID_ABOUT, wxT("&About..."), wxT("Shou about dialog"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar -> Append(menuFile, wxT("&File"));
    menuBar -> Append(menuHelp, wxT("&Help"));
    SetMenuBar(menuBar);

    wxBoxSizer* bSizer1 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* bSizer = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Game Control")), wxHORIZONTAL);
    bSizer1->Add(BoardLength + BoardX, BoardLength + BoardY * 3);
    BtnStart = new wxButton(this, START_ID, wxT("&Start"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer->Add(BtnStart, 0, wxALL, 5);
    BtnPause = new wxButton(this, PAUSE_ID, wxT("&Pause"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer->Add(BtnPause, 0, wxALL, 5);
    BtnResume = new wxButton(this, RESUME_ID, wxT("&Resume"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer->Add(BtnResume, 0, wxALL, 5);
    BtnReset = new wxButton(this, RESET_ID, wxT("R&eset"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer->Add(BtnReset, 0, wxALL, 5);

    m_txtctrl = new wxTextCtrl(this, wxID_ANY, "",
                               wxDefaultPosition, wxSize(400, 510),
                               wxTE_MULTILINE | wxTE_READONLY);

    wxFont font(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE,
                wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    m_txtctrl->SetFont(font);

    m_txtctrl->SetFocus();

    wxBoxSizer* bSizer2 = new wxBoxSizer(wxVERTICAL);

    bSizer2->Add(bSizer, 0, wxALL, 10);
    bSizer2->Add(m_txtctrl, 0, wxALL, 10);

    bSizer1->Add(bSizer2, 0, wxEXPAND, 5);
    SetSizer(bSizer1);
    Layout();
    bSizer1->Fit(this);

    BtnStart->Enable(true);
    BtnPause->Enable(false);
    BtnResume->Enable(false);
    BtnReset->Enable(false);

    CreateStatusBar();
    SetStatusText(wxT("FishTank System Ready"));
}

void FishTankFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void FishTankFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(wxT("This is the GUI system of FishTank made by Ianking Penn"), wxT("About AISystem"), wxOK | wxICON_INFORMATION);
}

void FishTankFrame::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxGREY_BRUSH);

    const int W = BoardLength / (wxMax(N, M) + 2);

    dc.DrawRectangle(BoardX, BoardY, W * (N + 2), W * (M + 2));

    for (int i = 1; i <= N + 1; ++i)
    {
        if (i % 5 == 0)
            dc.DrawText(wxString::Format("%d", i), wxPoint(BoardX + i * W + (i < 10 ? W / 4 : W / 8), BoardY));
        dc.DrawLine(BoardX + i * W, BoardY + W, BoardX + i * W, BoardY + (M + 1) * W + 1);
    }

    for (int i = 1; i <= M + 1; ++i)
    {
        if (i % 5 == 0)
            dc.DrawText(wxString::Format("%d", i), wxPoint(BoardX + (i < 10 ? W * 3 / 8 : 0), BoardY + i * W));
        dc.DrawLine(BoardX + W, BoardY + i * W, BoardX + (N + 1) * W + 1, BoardY + i * W);
    }
    if (thread)
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j)
        {
            wxSetMapEvent* event = new wxSetMapEvent(i, j, thread->askWhat(i, j));
            OnSetMap(*event);
        }
}

void FishTankFrame::OnStart(wxCommandEvent& WXUNUSED(event))
{
    if (!thread)
        thread = CreateThread();
    BtnStart->Enable(false);
    BtnPause->Enable(true);
    BtnResume->Enable(false);
    BtnReset->Enable(true);
}

void FishTankFrame::OnPause(wxCommandEvent& WXUNUSED(event))
{
    if (thread)
        if (thread->IsRunning())
            thread -> Pause();
    BtnPause->Enable(false);
    BtnResume->Enable(true);
}

void FishTankFrame::OnResume(wxCommandEvent& WXUNUSED(event))
{
    if (thread)
        if (thread->IsPaused())
            thread -> Resume();
    BtnPause->Enable(true);
    BtnResume->Enable(false);
}

void FishTankFrame::OnReset(wxCommandEvent& WXUNUSED(event))
{
    thread->Kill();
    thread = 0;
    BtnStart->Enable(true);
    BtnPause->Enable(false);
    BtnResume->Enable(false);
    BtnReset->Enable(false);
    m_txtctrl->Clear();
    Refresh();
}

env *FishTankFrame::CreateThread()
{
    env *thread = new env(this);
    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
    }
    thread->Run();
    return thread;
}

void FishTankFrame::OnSendMsg(wxSendMsgEvent& event)
{
    m_txtctrl->AppendText(event.GetMsg() + '\n');
}

void FishTankFrame::OnSendStatus(wxSendMsgEvent& event)
{
    SetStatusText(event.GetMsg() + '\n');
}

void FishTankFrame::OnSetMap(wxSetMapEvent& event)
{
    wxClientDC dc(this);
    const int W = BoardLength / (wxMax(N, M) + 2);
    if (event.GetTarget() == EMPTY)
    {
        dc.SetPen(*wxGREY_PEN);
        dc.SetBrush(*wxGREY_BRUSH);
        dc.DrawRectangle(BoardX + W * event.GetX() + 1, BoardY + W * event.GetY() + 1, W - 1, W - 1);
    }
    else if (event.GetTarget() == FOOD)
    {
        dc.SetPen(*wxGREEN_PEN);
        dc.SetBrush(*wxGREEN_BRUSH);
        dc.DrawCircle(BoardX + W * event.GetX() + W / 2, BoardY + W * event.GetY() + W / 2, W * 5 / 12);
    }
    else
    {
        dc.SetPen(*wxBLUE_PEN);
        dc.SetBrush(*wxBLUE_BRUSH);
        dc.DrawCircle(BoardX + W * event.GetX() + W / 2, BoardY + W * event.GetY() + W / 2, W * 5 / 12);
        dc.DrawText(wxString::Format("%d", event.GetTarget()), wxPoint(BoardX + W * event.GetX() + W / 8, BoardY + W * event.GetY() + W / 8));
    }
}
