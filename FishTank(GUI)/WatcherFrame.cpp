#include "WatcherFrame.h"

BEGIN_EVENT_TABLE(WatcherFrame, wxFrame)
    EVT_SEND_MSG(SEND_MSG_ID, WatcherFrame::OnSendMsg)
    EVT_CLOSE(WatcherFrame::OnClose)
END_EVENT_TABLE()

WatcherFrame::WatcherFrame(FishInfo* info, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    :wxFrame((wxFrame*)NULL, -1, title, pos, size, style), data(info)
{
    wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    txtctrl = new wxTextCtrl(notebook, wxID_ANY, "",
                               wxDefaultPosition, wxSize(300, 350),
                               wxTE_MULTILINE | wxTE_READONLY);

    wxFont font(wxNORMAL_FONT->GetPointSize(), wxFONTFAMILY_TELETYPE,
                wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    txtctrl->SetFont(font);

    txtctrl->SetFocus();

    list = new wxListCtrl(notebook, wxID_ANY, wxDefaultPosition, wxSize(300, 350), wxLC_REPORT | wxLC_SINGLE_SEL);

    list->InsertColumn(0, "ID", wxLIST_FORMAT_CENTRE);
    list->InsertColumn(1, "Lv", wxLIST_FORMAT_CENTRE);
    list->InsertColumn(2, "HP", wxLIST_FORMAT_CENTRE);
    list->InsertColumn(3, "MaxHP", wxLIST_FORMAT_CENTRE);
    list->InsertColumn(4, "Att", wxLIST_FORMAT_CENTRE);
    list->InsertColumn(5, "Sp", wxLIST_FORMAT_CENTRE);
    list->InsertColumn(6, "Exp", wxLIST_FORMAT_CENTRE);
    list->InsertColumn(7, "Point", wxLIST_FORMAT_CENTRE);
    list->InsertColumn(8, "Score", wxLIST_FORMAT_CENTRE);
    for (int i = 1; i <= MAX_PLAYER; ++i)
    {
        list->InsertItem(i-1, wxString::Format("%d", i));
        list->SetColumnWidth(i-1, wxLIST_AUTOSIZE_USEHEADER);
        list->SetItem(i-1, 1, wxString::Format("%d", data->getLevel(i)));
        list->SetItem(i-1, 2, wxString::Format("%d", data->getHP(i)));
        list->SetItem(i-1, 3, wxString::Format("%d", data->getMaxHP(i)));
        list->SetItem(i-1, 4, wxString::Format("%d", data->getAtt(i)));
        list->SetItem(i-1, 5, wxString::Format("%d", data->getSp(i)));
        list->SetItem(i-1, 6, wxString::Format("%d", data->getExp(i)));
        list->SetItem(i-1, 7, wxString::Format("%d", data->getPoint(i)));
    }
    notebook->AddPage(txtctrl, wxT("Log"), true);
    notebook->AddPage(list, wxT("Status"), false);

}

void WatcherFrame::OnSendMsg(wxSendMsgEvent& event)
{
    txtctrl->AppendText(event.GetMsg() + '\n');
}

void WatcherFrame::OnClose(wxCloseEvent& event)
{
    event.Veto();
    return;
}
