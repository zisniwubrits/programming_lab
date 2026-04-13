// MainFrame.cpp
#include "MainFrame.h"
#include <wx/sizer.h>
#include <wx/clipbrd.h>
#include <cstring>

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(wxID_ADD, MainFrame::OnAddRecord)
    EVT_BUTTON(wxID_REPLACE, MainFrame::OnUpdateRecord)
    EVT_BUTTON(wxID_DELETE, MainFrame::OnDeleteRecord)
    EVT_LIST_ITEM_SELECTED(wxID_ANY, MainFrame::OnItemSelected)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)),
      m_dataManager("data.dat") {

    // Load data
    m_dataManager.LoadData();

    // Create main panel and top-level sizer
    wxPanel* mainPanel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainPanel->SetSizer(mainSizer);

    // 1. Create input form area
    wxStaticBoxSizer* formSizer = new wxStaticBoxSizer(wxVERTICAL, mainPanel, "Transaction Details");
    
    // Date row
    wxBoxSizer* dateRow = new wxBoxSizer(wxHORIZONTAL);
    dateRow->Add(new wxStaticText(mainPanel, wxID_ANY, "Date:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    m_datePicker = new wxDatePickerCtrl(mainPanel, wxID_ANY);
    dateRow->Add(m_datePicker, 1);
    formSizer->Add(dateRow, 0, wxEXPAND | wxALL, 5);

    // Project name row
    wxBoxSizer* nameRow = new wxBoxSizer(wxHORIZONTAL);
    nameRow->Add(new wxStaticText(mainPanel, wxID_ANY, "Item:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    m_nameTextCtrl = new wxTextCtrl(mainPanel, wxID_ANY);
    nameRow->Add(m_nameTextCtrl, 1);
    formSizer->Add(nameRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // Type and amount row
    wxBoxSizer* typeAmountRow = new wxBoxSizer(wxHORIZONTAL);
    // Type
    typeAmountRow->Add(new wxStaticText(mainPanel, wxID_ANY, "Type:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    m_typeChoice = new wxChoice(mainPanel, wxID_ANY);
    m_typeChoice->Append("Income");
    m_typeChoice->Append("Expense");
    m_typeChoice->SetSelection(1); // Default to "Expense"
    typeAmountRow->Add(m_typeChoice, 0, wxRIGHT, 10);
    // Amount
    typeAmountRow->Add(new wxStaticText(mainPanel, wxID_ANY, "Amount ($):"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    m_amountTextCtrl = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1));
    typeAmountRow->Add(m_amountTextCtrl, 0);
    formSizer->Add(typeAmountRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // Note row
    wxBoxSizer* noteRow = new wxBoxSizer(wxHORIZONTAL);
    noteRow->Add(new wxStaticText(mainPanel, wxID_ANY, "Note:"), 0, wxALIGN_TOP | wxRIGHT, 5);
    m_noteTextCtrl = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 60), wxTE_MULTILINE);
    noteRow->Add(m_noteTextCtrl, 1, wxEXPAND);
    formSizer->Add(noteRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    mainSizer->Add(formSizer, 0, wxEXPAND | wxALL, 10);

    // 2. Create action buttons area
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    m_addButton = new wxButton(mainPanel, wxID_ADD, "Add");
    m_updateButton = new wxButton(mainPanel, wxID_REPLACE, "Update");
    m_deleteButton = new wxButton(mainPanel, wxID_DELETE, "Delete");
    buttonSizer->Add(m_addButton, 0, wxRIGHT, 5);
    buttonSizer->Add(m_updateButton, 0, wxRIGHT, 5);
    buttonSizer->Add(m_deleteButton, 0);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    // 3. Create data display area
    m_listCtrl = new wxListCtrl(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    // Add columns
    m_listCtrl->AppendColumn("ID", wxLIST_FORMAT_LEFT, 40);
    m_listCtrl->AppendColumn("Date", wxLIST_FORMAT_LEFT, 100);
    m_listCtrl->AppendColumn("Item", wxLIST_FORMAT_LEFT, 150);
    m_listCtrl->AppendColumn("Type", wxLIST_FORMAT_LEFT, 60);
    m_listCtrl->AppendColumn("Amount", wxLIST_FORMAT_RIGHT, 100);
    m_listCtrl->AppendColumn("Note", wxLIST_FORMAT_LEFT, 200);
    mainSizer->Add(m_listCtrl, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Initial refresh
    RefreshListCtrl();

    // Set minimum window size
    this->SetMinSize(wxSize(700, 500));
}

// --- Event Handlers ---

void MainFrame::OnAddRecord(wxCommandEvent& event) {
    Record newRecord;
    
    // Get input data
    wxDateTime date = m_datePicker->GetValue();
    newRecord.year = date.GetYear();
    newRecord.month = date.GetMonth() + 1;
    newRecord.day = date.GetDay();
    
    // Convert wxString to UTF-8 for char array
    wxString nameStr = m_nameTextCtrl->GetValue();
    auto utf8Name = nameStr.ToUTF8();
    std::strncpy(newRecord.name, utf8Name.data(), 30);
    newRecord.name[30] = '\0';
    
    wxString typeStr = m_typeChoice->GetStringSelection();
    auto utf8Type = typeStr.ToUTF8();
    std::strncpy(newRecord.type, utf8Type.data(), 5);
    newRecord.type[5] = '\0';
    
    // Parse amount, convert to 0.001 unit
    double amountDbl;
    if (!m_amountTextCtrl->GetValue().ToDouble(&amountDbl) || amountDbl <= 0) {
        wxMessageBox("Please enter a valid amount!", "Error", wxOK | wxICON_ERROR);
        return;
    }
    newRecord.amount = static_cast<long long>(amountDbl * 1000 + 0.5);

    wxString noteStr = m_noteTextCtrl->GetValue();
    auto utf8Note = noteStr.ToUTF8();
    std::strncpy(newRecord.note, utf8Note.data(), 50);
    newRecord.note[50] = '\0';

    // Assign ID (simple: max ID + 1)
    int maxId = 0;
    for (const auto& rec : m_dataManager.GetData()) {
        if (rec.id > maxId) maxId = rec.id;
    }
    newRecord.id = maxId + 1;

    // Add to data manager
    auto& data = m_dataManager.GetData();
    data.push_back(newRecord);
    m_dataManager.SaveData();

    // Refresh UI and clear inputs
    RefreshListCtrl();
    m_nameTextCtrl->Clear();
    m_amountTextCtrl->Clear();
    m_noteTextCtrl->Clear();
    m_nameTextCtrl->SetFocus();
}

void MainFrame::OnUpdateRecord(wxCommandEvent& event) {
    long itemIndex = m_listCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1) {
        wxMessageBox("Please select a record in the list to update.", "Hint", wxOK | wxICON_INFORMATION);
        return;
    }

    // Get record to update
    auto& data = m_dataManager.GetData();
    int recordId = wxAtoi(m_listCtrl->GetItemText(itemIndex, 0));
    auto it = std::find_if(data.begin(), data.end(), [recordId](const Record& r) { return r.id == recordId; });
    if (it == data.end()) return;

    // Update record data
    wxDateTime date = m_datePicker->GetValue();
    it->year = date.GetYear();
    it->month = date.GetMonth() + 1;
    it->day = date.GetDay();
    
    wxString nameStr = m_nameTextCtrl->GetValue();
    auto utf8Name = nameStr.ToUTF8();
    std::strncpy(it->name, utf8Name.data(), 30);
    it->name[30] = '\0';
    
    wxString typeStr = m_typeChoice->GetStringSelection();
    auto utf8Type = typeStr.ToUTF8();
    std::strncpy(it->type, utf8Type.data(), 5);
    it->type[5] = '\0';
    
    double amountDbl;
    if (!m_amountTextCtrl->GetValue().ToDouble(&amountDbl) || amountDbl <= 0) {
        wxMessageBox("Please enter a valid amount!", "Error", wxOK | wxICON_ERROR);
        return;
    }
    it->amount = static_cast<long long>(amountDbl * 1000 + 0.5);

    wxString noteStr = m_noteTextCtrl->GetValue();
    auto utf8Note = noteStr.ToUTF8();
    std::strncpy(it->note, utf8Note.data(), 50);
    it->note[50] = '\0';

    // Save and refresh
    m_dataManager.SaveData();
    RefreshListCtrl();
}

void MainFrame::OnDeleteRecord(wxCommandEvent& event) {
    long itemIndex = m_listCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1) {
        wxMessageBox("Please select a record in the list to delete.", "Hint", wxOK | wxICON_INFORMATION);
        return;
    }

    if (wxMessageBox("Are you sure you want to delete this record?", "Confirm", wxYES_NO | wxICON_QUESTION) != wxYES) {
        return;
    }

    // Get record ID to delete
    int recordId = wxAtoi(m_listCtrl->GetItemText(itemIndex, 0));
    auto& data = m_dataManager.GetData();
    data.erase(std::remove_if(data.begin(), data.end(), [recordId](const Record& r) { return r.id == recordId; }), data.end());

    m_dataManager.SaveData();
    RefreshListCtrl();
}

void MainFrame::OnItemSelected(wxListEvent& event) {
    long itemIndex = event.GetIndex();
    if (itemIndex == -1) return;

    // Get selected record
    int recordId = wxAtoi(m_listCtrl->GetItemText(itemIndex, 0));
    const auto& data = m_dataManager.GetData();
    auto it = std::find_if(data.begin(), data.end(), [recordId](const Record& r) { return r.id == recordId; });
    if (it == data.end()) return;

    // Fill record data into input fields
    m_datePicker->SetValue(wxDateTime(it->day, wxDateTime::Month(it->month - 1), it->year));
    m_nameTextCtrl->SetValue(wxString::FromUTF8(it->name));
    m_typeChoice->SetStringSelection(wxString::FromUTF8(it->type));
    double amountToShow = static_cast<double>(it->amount) / 1000.0;
    m_amountTextCtrl->SetValue(wxString::Format("%.2f", amountToShow));
    m_noteTextCtrl->SetValue(wxString::FromUTF8(it->note));
}

// --- Helper Function ---

void MainFrame::RefreshListCtrl() {
    m_listCtrl->DeleteAllItems();
    const auto& data = m_dataManager.GetData();

    for (size_t i = 0; i < data.size(); ++i) {
        const Record& rec = data[i];
        long index = m_listCtrl->InsertItem(i, wxString::Format("%d", rec.id));
        
        // Format date
        m_listCtrl->SetItem(index, 1, wxString::Format("%04d-%02d-%02d", rec.year, rec.month, rec.day));
        m_listCtrl->SetItem(index, 2, wxString::FromUTF8(rec.name));
        m_listCtrl->SetItem(index, 3, wxString::FromUTF8(rec.type));
        
        // Format amount
        double amountToShow = static_cast<double>(rec.amount) / 1000.0;
        wxString amountStr = wxString::Format("%.2f", amountToShow);
        // Add negative sign for expenses
        if (std::string(rec.type) == "Expense") {
            amountStr = "-" + amountStr;
        }
        m_listCtrl->SetItem(index, 4, amountStr);
        
        m_listCtrl->SetItem(index, 5, wxString::FromUTF8(rec.note));
    }
}