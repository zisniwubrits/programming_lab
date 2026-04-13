// MainFrame.h
#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/listctrl.h>
#include "DataManager.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

private:
    // Data model
    DataManager m_dataManager;

    // Input controls
    wxDatePickerCtrl* m_datePicker;
    wxTextCtrl* m_nameTextCtrl;
    wxChoice* m_typeChoice;
    wxTextCtrl* m_amountTextCtrl;
    wxTextCtrl* m_noteTextCtrl;

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

    void RefreshListCtrl();

    wxDECLARE_EVENT_TABLE();
};

#endif // MAIN_FRAME_H