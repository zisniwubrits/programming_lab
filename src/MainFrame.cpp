// MainFrame.cpp
#include "MainFrame.h"
#include <wx/sizer.h>
#include <wx/valnum.h> // for numeric validator

// 定义事件ID (也可以使用 wxID_ANY)
enum
{
    ID_ADD_BUTTON = 1000,
    ID_DELETE_BUTTON,
    ID_UPDATE_BUTTON,
    ID_LIST_BOX
};

// 实现事件表
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(ID_ADD_BUTTON, MainFrame::OnAdd)
    EVT_BUTTON(ID_DELETE_BUTTON, MainFrame::OnDelete)
    EVT_BUTTON(ID_UPDATE_BUTTON, MainFrame::OnUpdate)
    EVT_LISTBOX(ID_LIST_BOX, MainFrame::OnItemSelected)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1280, 720)),
    m_dataManager("data.txt") // ← 初始化数据管理器，数据文件为 data.txt
{
    // 启动时加载数据
    m_dataManager.LoadData();

    // 创建主面板
    wxPanel* panel = new wxPanel(this);

    // 创建控件
    m_inputTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
    m_listBox = new wxListBox(panel, ID_LIST_BOX, wxDefaultPosition, wxDefaultSize, 0, nullptr, wxLB_SINGLE | wxLB_NEEDED_SB);
    m_addButton = new wxButton(panel, ID_ADD_BUTTON, "Add");
    m_deleteButton = new wxButton(panel, ID_DELETE_BUTTON, "Delete");
    m_updateButton = new wxButton(panel, ID_UPDATE_BUTTON, "Update");

    // 初始状态：禁用删除和更新按钮
    m_deleteButton->Enable(false);
    m_updateButton->Enable(false);

    RefreshListBox();

    // 布局 - 使用 Sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Value:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    inputSizer->Add(m_inputTextCtrl, 1, wxEXPAND);

    buttonSizer->Add(m_addButton, 0, wxRIGHT, 5);
    buttonSizer->Add(m_deleteButton, 0, wxRIGHT, 5);
    buttonSizer->Add(m_updateButton, 0);

    mainSizer->Add(inputSizer, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(m_listBox, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    panel->SetSizer(mainSizer);
}

void MainFrame::OnAdd(wxCommandEvent& event)
{
    wxString valueStr = m_inputTextCtrl->GetValue().Trim();
    if (valueStr.empty()) {
        wxMessageBox("Please enter a number.", "Input Error", wxOK | wxICON_WARNING);
        return;
    }

    long long value;
    if (!valueStr.ToLongLong(&value)) {
        wxMessageBox("Invalid number format.", "Input Error", wxOK | wxICON_ERROR);
        return;
    }

    auto& data = m_dataManager.GetData();
    data.push_back(value);
    m_dataManager.SaveData(); // ← 保存到文件

    RefreshListBox();
    m_inputTextCtrl->Clear();
    m_inputTextCtrl->SetFocus();
}

void MainFrame::OnDelete(wxCommandEvent& event)
{
    int selection = m_listBox->GetSelection();
    if (selection == wxNOT_FOUND) return;

    auto& data = m_dataManager.GetData();
    data.erase(data.begin() + selection);
    m_dataManager.SaveData(); // ← 保存到文件

    RefreshListBox();
    m_inputTextCtrl->Clear();
    m_deleteButton->Enable(false);
    m_updateButton->Enable(false);
}

void MainFrame::OnUpdate(wxCommandEvent& event)
{
    int selection = m_listBox->GetSelection();
    if (selection == wxNOT_FOUND) return;

    wxString valueStr = m_inputTextCtrl->GetValue().Trim();
    if (valueStr.empty()) {
        wxMessageBox("Please enter a number.", "Input Error", wxOK | wxICON_WARNING);
        return;
    }

    long long value;
    if (!valueStr.ToLongLong(&value)) {
        wxMessageBox("Invalid number format.", "Input Error", wxOK | wxICON_ERROR);
        return;
    }

    auto& data = m_dataManager.GetData();
    data[selection] = value;
    m_dataManager.SaveData(); // ← 保存到文件

    RefreshListBox();
    m_inputTextCtrl->Clear();
}

void MainFrame::OnItemSelected(wxCommandEvent& event)
{
    int selection = event.GetSelection();
    if (selection != wxNOT_FOUND) {
        // 将选中的值填充到输入框
        const auto& data = m_dataManager.GetData();
        m_inputTextCtrl->SetValue(wxString::Format("%lld", data[selection]));
        m_deleteButton->Enable(true);
        m_updateButton->Enable(true);
    }
}

void MainFrame::RefreshListBox()
{
    m_listBox->Clear();
    const auto& data = m_dataManager.GetData();
    for (const auto& item : data) {
        m_listBox->Append(wxString::Format("%lld", item));
    }
}