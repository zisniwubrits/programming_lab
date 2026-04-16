// MainFrame.h
#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/listctrl.h>
#include "DataManager.h"
#include "AlgorithmUtils.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

private:
    // Data model
    DataManager m_dataManager;
    
    // Current displayed records (for sorting/searching)
    std::vector<Record> m_currentRecords;

    // Action buttons
    wxButton* m_addButton;
    wxButton* m_updateButton;
    wxButton* m_deleteButton;

    // Data display control
    wxListCtrl* m_listCtrl;

    // Event handlers
    void OnAddRecord(wxCommandEvent& event);
    void OnUpdateRecord(wxCommandEvent& event);
    void OnDeleteRecord(wxCommandEvent& event);
    void OnItemSelected(wxListEvent& event);
    
    // Menu event handlers
    void OnStatistics(wxCommandEvent& event);
    void OnSearchByName(wxCommandEvent& event);
    void OnSearchByType(wxCommandEvent& event);
    void OnSearchByNote(wxCommandEvent& event);
    void OnSearchByDateRange(wxCommandEvent& event);
    void OnClearSearch(wxCommandEvent& event);
    void OnSortById(wxCommandEvent& event);
    void OnSortByDate(wxCommandEvent& event);
    void OnSortByName(wxCommandEvent& event);
    void OnSortByType(wxCommandEvent& event);
    void OnSortByAmount(wxCommandEvent& event);
    void OnSortByIdDesc(wxCommandEvent& event);
    void OnSortByDateDesc(wxCommandEvent& event);
    void OnSortByNameDesc(wxCommandEvent& event);
    void OnSortByTypeDesc(wxCommandEvent& event);
    void OnSortByAmountDesc(wxCommandEvent& event);

    // 新增：用于弹出详情窗口
    void OnViewDetail(wxCommandEvent& event);
    void RefreshListCtrl();

    wxDECLARE_EVENT_TABLE();
};

#endif // MAIN_FRAME_H