// MainFrame.h
#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include "DataManager.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

private:
    // 数据模型
    DataManager m_dataManager; // ← 新增后台数据管理

    // UI 控件
    wxTextCtrl* m_inputTextCtrl;
    wxListBox* m_listBox;
    wxButton* m_addButton;
    wxButton* m_deleteButton;
    wxButton* m_updateButton;

    // 事件处理函数
    void OnAdd(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnUpdate(wxCommandEvent& event);
    void OnItemSelected(wxCommandEvent& event);

    // 辅助函数
    void RefreshListBox();

    // 声明事件表
    wxDECLARE_EVENT_TABLE();
};

#endif // MAIN_FRAME_H