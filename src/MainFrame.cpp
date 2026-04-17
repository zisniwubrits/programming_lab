// MainFrame.cpp
#include "MainFrame.h"
#include <wx/sizer.h>
#include <wx/clipbrd.h>
#include <cstring>

// Menu IDs
enum {
    ID_STATISTICS = 10000,
    ID_SEARCH_NAME,
    ID_SEARCH_TYPE,
    ID_SEARCH_NOTE,
    ID_SEARCH_DATE_RANGE,
    ID_CLEAR_SEARCH,
    ID_SORT_ID_ASC,
    ID_SORT_DATE_ASC,
    ID_SORT_NAME_ASC,
    ID_SORT_TYPE_ASC,
    ID_SORT_AMOUNT_ASC,
    ID_SORT_ID_DESC,
    ID_SORT_DATE_DESC,
    ID_SORT_NAME_DESC,
    ID_SORT_TYPE_DESC,
    ID_SORT_AMOUNT_DESC,

    ID_VIEW_DETAILS
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(wxID_ADD, MainFrame::OnAddRecord)
    EVT_BUTTON(wxID_REPLACE, MainFrame::OnUpdateRecord)
    EVT_BUTTON(wxID_DELETE, MainFrame::OnDeleteRecord)
    EVT_LIST_ITEM_SELECTED(wxID_ANY, MainFrame::OnItemSelected)
    // Menu events
    EVT_MENU(ID_STATISTICS, MainFrame::OnStatistics)
    EVT_MENU(ID_SEARCH_NAME, MainFrame::OnSearchByName)
    EVT_MENU(ID_SEARCH_TYPE, MainFrame::OnSearchByType)
    EVT_MENU(ID_SEARCH_NOTE, MainFrame::OnSearchByNote)
    EVT_MENU(ID_SEARCH_DATE_RANGE, MainFrame::OnSearchByDateRange)
    EVT_MENU(ID_CLEAR_SEARCH, MainFrame::OnClearSearch)
    EVT_MENU(ID_SORT_ID_ASC, MainFrame::OnSortById)
    EVT_MENU(ID_SORT_DATE_ASC, MainFrame::OnSortByDate)
    EVT_MENU(ID_SORT_NAME_ASC, MainFrame::OnSortByName)
    EVT_MENU(ID_SORT_TYPE_ASC, MainFrame::OnSortByType)
    EVT_MENU(ID_SORT_AMOUNT_ASC, MainFrame::OnSortByAmount)
    EVT_MENU(ID_SORT_ID_DESC, MainFrame::OnSortByIdDesc)
    EVT_MENU(ID_SORT_DATE_DESC, MainFrame::OnSortByDateDesc)
    EVT_MENU(ID_SORT_NAME_DESC, MainFrame::OnSortByNameDesc)
    EVT_MENU(ID_SORT_TYPE_DESC, MainFrame::OnSortByTypeDesc)
    EVT_MENU(ID_SORT_AMOUNT_DESC, MainFrame::OnSortByAmountDesc)

    EVT_BUTTON(ID_VIEW_DETAILS, MainFrame::OnViewDetail)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)),
      m_dataManager("data.dat") {

    // Load data
    m_dataManager.LoadData();
    m_currentRecords = m_dataManager.GetData();

    // Create menu bar
    wxMenuBar* menuBar = new wxMenuBar();
    
    // Statistics menu
    wxMenu* statisticsMenu = new wxMenu();
    statisticsMenu->Append(ID_STATISTICS, wxT("显示统计信息\tCtrl+S"));
    menuBar->Append(statisticsMenu, wxT("统计"));
    
    // Search menu
    wxMenu* searchMenu = new wxMenu();
    searchMenu->Append(ID_SEARCH_NAME, wxT("按项目名称查找\tCtrl+F"));
    searchMenu->Append(ID_SEARCH_TYPE, wxT("按类型查找"));
    searchMenu->Append(ID_SEARCH_NOTE, wxT("按备注查找"));
    searchMenu->Append(ID_SEARCH_DATE_RANGE, wxT("按日期范围查找"));
    searchMenu->AppendSeparator();
    searchMenu->Append(ID_CLEAR_SEARCH, wxT("清除搜索结果\tEsc"));
    menuBar->Append(searchMenu, wxT("查找"));
    
    // Sort menu
    wxMenu* sortMenu = new wxMenu();
    wxMenu* sortAscMenu = new wxMenu();
    sortAscMenu->Append(ID_SORT_ID_ASC, wxT("按ID升序"));
    sortAscMenu->Append(ID_SORT_DATE_ASC, wxT("按日期升序"));
    sortAscMenu->Append(ID_SORT_NAME_ASC, wxT("按项目升序"));
    sortAscMenu->Append(ID_SORT_TYPE_ASC, wxT("按类型升序"));
    sortAscMenu->Append(ID_SORT_AMOUNT_ASC, wxT("按金额升序"));
    
    wxMenu* sortDescMenu = new wxMenu();
    sortDescMenu->Append(ID_SORT_ID_DESC, wxT("按ID降序"));
    sortDescMenu->Append(ID_SORT_DATE_DESC, wxT("按日期降序"));
    sortDescMenu->Append(ID_SORT_NAME_DESC, wxT("按项目降序"));
    sortDescMenu->Append(ID_SORT_TYPE_DESC, wxT("按类型降序"));
    sortDescMenu->Append(ID_SORT_AMOUNT_DESC, wxT("按金额降序"));
    
    sortMenu->AppendSubMenu(sortAscMenu, wxT("升序排列"));
    sortMenu->AppendSubMenu(sortDescMenu, wxT("降序排列"));
    menuBar->Append(sortMenu, wxT("排序"));
    
    SetMenuBar(menuBar);

    // Create main panel and top-level sizer
    wxPanel* mainPanel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainPanel->SetSizer(mainSizer);

    // 1. Create action buttons area
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    m_addButton = new wxButton(mainPanel, wxID_ADD, wxT("添加"));
    m_updateButton = new wxButton(mainPanel, wxID_REPLACE, wxT("修改"));
    m_deleteButton = new wxButton(mainPanel, wxID_DELETE, wxT("删除"));
    wxButton* viewButton = new wxButton(mainPanel, ID_VIEW_DETAILS, wxT("查看详情"));
    buttonSizer->Add(m_addButton, 0, wxRIGHT, 5);
    buttonSizer->Add(m_updateButton, 0, wxRIGHT, 5);
    buttonSizer->Add(m_deleteButton, 0, wxRIGHT, 5);
    buttonSizer->Add(viewButton, 0);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxALL, 10);

    // 2. Create data display area
    m_listCtrl = new wxListCtrl(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    // Add columns
    m_listCtrl->AppendColumn("ID", wxLIST_FORMAT_LEFT, 40);
    m_listCtrl->AppendColumn(wxT("日期"), wxLIST_FORMAT_LEFT, 100);
    m_listCtrl->AppendColumn(wxT("项目"), wxLIST_FORMAT_LEFT, 150);
    m_listCtrl->AppendColumn(wxT("类型"), wxLIST_FORMAT_LEFT, 60);
    m_listCtrl->AppendColumn(wxT("金额"), wxLIST_FORMAT_RIGHT, 100);
    m_listCtrl->AppendColumn(wxT("备注"), wxLIST_FORMAT_LEFT, 200);
    mainSizer->Add(m_listCtrl, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Initial refresh
    RefreshListCtrl();

    // Set minimum window size
    this->SetMinSize(wxSize(700, 500));
}

// --- Original Event Handlers ---
void MainFrame::OnAddRecord(wxCommandEvent& event) {
    // Create a dialog for adding a new record
    wxDialog addDialog(this, wxID_ANY, wxT("添加新记录"), wxDefaultPosition, wxSize(400, 300));
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    addDialog.SetSizer(mainSizer);

    // Input controls inside the dialog
    wxTextCtrl* dateTextCtrl; // Replaced with text control
    wxTextCtrl* nameTextCtrl;
    wxChoice* typeChoice;
    wxTextCtrl* amountTextCtrl;
    wxTextCtrl* noteTextCtrl;

    // Date row
    wxBoxSizer* dateRow = new wxBoxSizer(wxHORIZONTAL);
    dateRow->Add(new wxStaticText(&addDialog, wxID_ANY, wxT("日期:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    dateTextCtrl = new wxTextCtrl(&addDialog, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1));
    dateTextCtrl->SetHint("YYYY-MM-DD"); // Add format hint    
    // --- Add default date (today) ---
    wxDateTime today = wxDateTime::Now();
    dateTextCtrl->SetValue(today.Format("%Y-%m-%d"));
    
    dateRow->Add(dateTextCtrl, 1);
    mainSizer->Add(dateRow, 0, wxEXPAND | wxALL, 5);

    // Project name row
    wxBoxSizer* nameRow = new wxBoxSizer(wxHORIZONTAL);
    nameRow->Add(new wxStaticText(&addDialog, wxID_ANY, wxT("项目:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    nameTextCtrl = new wxTextCtrl(&addDialog, wxID_ANY);
    nameRow->Add(nameTextCtrl, 1);
    mainSizer->Add(nameRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // Type and amount row
    wxBoxSizer* typeAmountRow = new wxBoxSizer(wxHORIZONTAL);
    // Type
    typeAmountRow->Add(new wxStaticText(&addDialog, wxID_ANY, wxT("类型:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    typeChoice = new wxChoice(&addDialog, wxID_ANY);
    typeChoice->Append(wxT("收入"));
    typeChoice->Append(wxT("支出"));
    typeChoice->SetSelection(1); // Default to "Expense"
    typeAmountRow->Add(typeChoice, 0, wxRIGHT, 10);
    // Amount
    typeAmountRow->Add(new wxStaticText(&addDialog, wxID_ANY, wxT("金额(元):")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    amountTextCtrl = new wxTextCtrl(&addDialog, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1));
    typeAmountRow->Add(amountTextCtrl, 0);
    mainSizer->Add(typeAmountRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // Note row
    wxBoxSizer* noteRow = new wxBoxSizer(wxHORIZONTAL);
    noteRow->Add(new wxStaticText(&addDialog, wxID_ANY, wxT("备注")), 0, wxALIGN_TOP | wxRIGHT, 5);
    noteTextCtrl = new wxTextCtrl(&addDialog, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 60), wxTE_MULTILINE);
    noteRow->Add(noteTextCtrl, 1, wxEXPAND);
    mainSizer->Add(noteRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // --- Refactored Button Placement (Bottom Right) ---
    wxStdDialogButtonSizer* buttonSizer = new wxStdDialogButtonSizer();
    wxButton* okButton = new wxButton(&addDialog, wxID_OK, wxT("确定"));
    wxButton* cancelButton = new wxButton(&addDialog, wxID_CANCEL, wxT("取消"));
    buttonSizer->AddButton(okButton);
    buttonSizer->AddButton(cancelButton);
    buttonSizer->Realize();

    // Add the button sizer to the main sizer with right alignment
    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 10);
    // --- End of Button Refactor ---

    if (addDialog.ShowModal() != wxID_OK) {
        return; // User cancelled
    }

    Record newRecord;
    
    // Get input data from dialog controls
    wxString dateStr = dateTextCtrl->GetValue();
    wxDateTime date;
    if (!date.ParseFormat(dateStr, "%Y-%m-%d") || !date.IsValid()) {
        wxMessageBox(wxT("日期格式错误！请使用 YYYY-MM-DD 格式。"), wxT("错误"), wxOK | wxICON_ERROR);
        return;
    }
    newRecord.year = date.GetYear();
    newRecord.month = date.GetMonth() + 1;
    newRecord.day = date.GetDay();
    
    // Convert wxString to UTF-8 for char array
    wxString nameStr = nameTextCtrl->GetValue();
    auto utf8Name = nameStr.ToUTF8();
    std::strncpy(newRecord.name, utf8Name.data(), 30);
    newRecord.name[30] = '\0';
    
    wxString typeStr = typeChoice->GetStringSelection();
    auto utf8Type = typeStr.ToUTF8();
    std::strncpy(newRecord.type, utf8Type.data(), 6);       // Copy 6 bytes
    newRecord.type[6] = '\0';                               // Ensure null termination
    
    // Parse amount, convert to 0.001 unit
    double amountDbl;
    if (!amountTextCtrl->GetValue().ToDouble(&amountDbl) || amountDbl <= 0) {
        wxMessageBox(wxT("请输入有效的金额！"), wxT("错误"), wxOK | wxICON_ERROR);
        return;
    }
    newRecord.amount = static_cast<long long>(amountDbl * 1000 + 0.5);

    wxString noteStr = noteTextCtrl->GetValue();
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
    
    // Update current records and refresh UI
    m_currentRecords = m_dataManager.GetData();
    RefreshListCtrl();
}

void MainFrame::OnUpdateRecord(wxCommandEvent& event) {
    long itemIndex = m_listCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1) {
        // Fixed: Correct prompt message
        wxMessageBox(wxT("请先在列表中选择一条记录进行修改。"), wxT("提示"), wxOK | wxICON_INFORMATION);
        return;
    }

    // Get record to update
    const auto& originalData = m_dataManager.GetData();
    int recordId = wxAtoi(m_listCtrl->GetItemText(itemIndex, 0));
    auto it = std::find_if(originalData.begin(), originalData.end(), [recordId](const Record& r) { return r.id == recordId; });
    if (it == originalData.end()) return;

    // Create a dialog for updating the record, pre-filled with existing data
    wxDialog updateDialog(this, wxID_ANY, wxT("修改记录"), wxDefaultPosition, wxSize(400, 300));
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    updateDialog.SetSizer(mainSizer);

    // Input controls inside the dialog
    wxTextCtrl* dateTextCtrl; // Replaced with text control
    wxTextCtrl* nameTextCtrl;
    wxChoice* typeChoice;
    wxTextCtrl* amountTextCtrl;
    wxTextCtrl* noteTextCtrl;

    // Date row
    wxBoxSizer* dateRow = new wxBoxSizer(wxHORIZONTAL);
    dateRow->Add(new wxStaticText(&updateDialog, wxID_ANY, wxT("日期:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    dateTextCtrl = new wxTextCtrl(&updateDialog, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1));
    // --- Fixed Date Display Bug: Ensure two-digit month/day ---
    dateTextCtrl->SetValue(wxString::Format("%04d-%02d-%02d", it->year, it->month, it->day)); // Fill with old data
    // --- End of Fix ---
    dateTextCtrl->SetHint("YYYY-MM-DD"); // Add format hint
    dateRow->Add(dateTextCtrl, 1);
    mainSizer->Add(dateRow, 0, wxEXPAND | wxALL, 5);

    // Project name row
    wxBoxSizer* nameRow = new wxBoxSizer(wxHORIZONTAL);
    nameRow->Add(new wxStaticText(&updateDialog, wxID_ANY, wxT("项目:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    nameTextCtrl = new wxTextCtrl(&updateDialog, wxID_ANY);
    nameTextCtrl->SetValue(wxString::FromUTF8(it->name));
    nameRow->Add(nameTextCtrl, 1);
    mainSizer->Add(nameRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // Type and amount row
    wxBoxSizer* typeAmountRow = new wxBoxSizer(wxHORIZONTAL);
    // Type
    typeAmountRow->Add(new wxStaticText(&updateDialog, wxID_ANY, wxT("类型:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    typeChoice = new wxChoice(&updateDialog, wxID_ANY);
    typeChoice->Append(wxT("收入"));
    typeChoice->Append(wxT("支出"));
    typeChoice->SetStringSelection(wxString::FromUTF8(it->type));
    typeAmountRow->Add(typeChoice, 0, wxRIGHT, 10);
    // Amount
    typeAmountRow->Add(new wxStaticText(&updateDialog, wxID_ANY, wxT("金额(元):")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    double amountToShow = static_cast<double>(it->amount) / 1000.0;
    amountTextCtrl = new wxTextCtrl(&updateDialog, wxID_ANY, wxString::Format("%.2f", amountToShow), wxDefaultPosition, wxSize(100, -1));
    typeAmountRow->Add(amountTextCtrl, 0);
    mainSizer->Add(typeAmountRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // Note row
    wxBoxSizer* noteRow = new wxBoxSizer(wxHORIZONTAL);
    noteRow->Add(new wxStaticText(&updateDialog, wxID_ANY, wxT("备注")), 0, wxALIGN_TOP | wxRIGHT, 5);
    noteTextCtrl = new wxTextCtrl(&updateDialog, wxID_ANY, wxString::FromUTF8(it->note), wxDefaultPosition, wxSize(-1, 60), wxTE_MULTILINE);
    noteRow->Add(noteTextCtrl, 1, wxEXPAND);
    mainSizer->Add(noteRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // --- Refactored Button Placement (Bottom Right) ---
    wxStdDialogButtonSizer* buttonSizer = new wxStdDialogButtonSizer();
    wxButton* okButton = new wxButton(&updateDialog, wxID_OK, wxT("确定"));
    wxButton* cancelButton = new wxButton(&updateDialog, wxID_CANCEL, wxT("取消"));
    buttonSizer->AddButton(okButton);
    buttonSizer->AddButton(cancelButton);
    buttonSizer->Realize();

    // Add the button sizer to the main sizer with right alignment
    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 10);
    // --- End of Button Refactor ---

    if (updateDialog.ShowModal() != wxID_OK) {
        return; // User cancelled
    }

    // Now update the actual record in the data manager
    auto& data = m_dataManager.GetData();
    auto updateIt = std::find_if(data.begin(), data.end(), [recordId](const Record& r) { return r.id == recordId; });
    if (updateIt == data.end()) return;

    // Update record data from dialog controls
    wxString dateStr = dateTextCtrl->GetValue();
    wxDateTime date;
    if (!date.ParseFormat(dateStr, "%Y-%m-%d") || !date.IsValid()) {
        wxMessageBox(wxT("日期格式错误！请使用 YYYY-MM-DD 格式。"), wxT("错误"), wxOK | wxICON_ERROR);
        return;
    }
    updateIt->year = date.GetYear();
    updateIt->month = date.GetMonth() + 1;
    updateIt->day = date.GetDay();
    
    wxString nameStr = nameTextCtrl->GetValue();
    auto utf8Name = nameStr.ToUTF8();
    std::strncpy(updateIt->name, utf8Name.data(), 30);
    updateIt->name[30] = '\0';
    
    wxString typeStr = typeChoice->GetStringSelection();
    auto utf8Type = typeStr.ToUTF8();
    std::strncpy(updateIt->type, utf8Type.data(), 6);
    updateIt->type[6] = '\0';
    
    double amountDbl;
    if (!amountTextCtrl->GetValue().ToDouble(&amountDbl) || amountDbl <= 0) {
        wxMessageBox(wxT("请输入有效的金额！"), wxT("错误"), wxOK | wxICON_ERROR);
        return;
    }
    updateIt->amount = static_cast<long long>(amountDbl * 1000 + 0.5);

    wxString noteStr = noteTextCtrl->GetValue();
    auto utf8Note = noteStr.ToUTF8();
    std::strncpy(updateIt->note, utf8Note.data(), 50);
    updateIt->note[50] = '\0';

    // Save and refresh
    m_dataManager.SaveData();
    m_currentRecords = m_dataManager.GetData();
    RefreshListCtrl();
}

void MainFrame::OnDeleteRecord(wxCommandEvent& event) {
    long itemIndex = m_listCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1) {
        // 修正：正确的提示信息
        wxMessageBox(wxT("请先在列表中选择一条记录进行删除。"), wxT("提示"), wxOK | wxICON_INFORMATION);
        return;
    }

    if (wxMessageBox(wxT("确定要删除这条记录吗？"), wxT("确认"), wxYES_NO | wxICON_QUESTION) != wxYES) {
        return;
    }

    // Get record ID to delete
    int recordId = wxAtoi(m_listCtrl->GetItemText(itemIndex, 0));
    auto& data = m_dataManager.GetData();
    data.erase(std::remove_if(data.begin(), data.end(), [recordId](const Record& r) { return r.id == recordId; }), data.end());

    m_dataManager.SaveData();
    m_currentRecords = m_dataManager.GetData();
    RefreshListCtrl();
}

void MainFrame::OnItemSelected(wxListEvent& event) {
    // This handler is kept for potential future use or if other logic depends on selection.
    // The form filling logic has been moved into the OnViewDetail dialog.
}

// --- Menu Event Handlers ---

void MainFrame::OnStatistics(wxCommandEvent& event) {
    auto stats = AlgorithmUtils::calculateStatistics(m_currentRecords);
    wxString message = wxString::Format(
        wxT("统计信息:\n\n")
        wxT("总收入: %.2f 元\n")
        wxT("总支出: %.2f 元\n")
        wxT("净余额: %.2f 元\n")
        wxT("记录总数: %zu 条"),
        stats.totalIncome,
        stats.totalExpense,
        stats.netBalance,
        stats.recordCount
    );
    wxMessageBox(message, wxT("统计信息"), wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSearchByName(wxCommandEvent& event) {
    wxString keyword = wxGetTextFromUser(wxT("请输入要查找的项目名称:"), wxT("查找项目"));
    if (!keyword.IsEmpty()) {
        std::string utf8Keyword = keyword.ToUTF8().data();
        auto filtered = AlgorithmUtils::searchRecords(
            m_currentRecords,
            AlgorithmUtils::SearchField::NAME,
            utf8Keyword
        );
        m_currentRecords = std::move(filtered);
        RefreshListCtrl();
    }
}

void MainFrame::OnSearchByType(wxCommandEvent& event) {
    wxArrayString choices;
    choices.Add(wxT("收入"));
    choices.Add(wxT("支出"));
    int selection = wxGetSingleChoiceIndex(
        wxT("请选择要查找的类型:"),
        wxT("查找类型"),
        choices
    );
    if (selection != -1) {
        std::string typeStr = (selection == 0) ? "\u6536\u5165" : "\u652f\u51fa";
        auto filtered = AlgorithmUtils::searchRecords(
            m_currentRecords,
            AlgorithmUtils::SearchField::TYPE,
            typeStr
        );
        m_currentRecords = std::move(filtered);
        RefreshListCtrl();
    }
}

void MainFrame::OnSearchByNote(wxCommandEvent& event) {
    wxString keyword = wxGetTextFromUser(wxT("请输入要查找的备注关键词:"), wxT("查找备注"));
    if (!keyword.IsEmpty()) {
        std::string utf8Keyword = keyword.ToUTF8().data();
        auto filtered = AlgorithmUtils::searchRecords(
            m_currentRecords,
            AlgorithmUtils::SearchField::NOTE,
            utf8Keyword
        );
        m_currentRecords = std::move(filtered);
        RefreshListCtrl();
    }
}

void MainFrame::OnSearchByDateRange(wxCommandEvent& event) {
    auto parseDate = [](const wxString& str, wxDateTime& dt) -> bool {
        int y, m, d;
        if (std::sscanf(str.ToUTF8().data(), "%d-%d-%d", &y, &m, &d) != 3)
            return false;
        dt.Set(d, wxDateTime::Month(m - 1), y);
        return dt.IsValid() && m >= 1 && m <= 12 && d >= 1 && d <= 31;
    };

    wxString startStr = wxGetTextFromUser(
        wxT("请输入开始日期（格式：YYYY-MM-DD）："),
        wxT("日期范围查找 - 开始")
    );
    if (startStr.empty()) return;

    wxString endStr = wxGetTextFromUser(
        wxT("请输入结束日期（格式：YYYY-MM-DD）："),
        wxT("日期范围查找 - 结束")
    );
    if (endStr.empty()) return;

    wxDateTime startDate, endDate;
    if (!parseDate(startStr, startDate)) {
        wxMessageBox(wxT("开始日期格式无效！请使用 YYYY-MM-DD 格式。"), wxT("错误"), wxOK | wxICON_ERROR);
        return;
    }
    if (!parseDate(endStr, endDate)) {
        wxMessageBox(wxT("结束日期格式无效！请使用 YYYY-MM-DD 格式。"), wxT("错误"), wxOK | wxICON_ERROR);
        return;
    }

    if (endDate < startDate) {
        wxMessageBox(wxT("结束日期不能早于开始日期！"), wxT("错误"), wxOK | wxICON_ERROR);
        return;
    }

    AlgorithmUtils::DateRange range;
    range.startYear = startDate.GetYear();
    range.startMonth = startDate.GetMonth() + 1;
    range.startDay = startDate.GetDay();
    range.endYear = endDate.GetYear();
    range.endMonth = endDate.GetMonth() + 1;
    range.endDay = endDate.GetDay();

    auto filtered = AlgorithmUtils::searchRecords(
        m_currentRecords,
        AlgorithmUtils::SearchField::DATE_RANGE,
        "",
        &range
    );
    m_currentRecords = std::move(filtered);
    RefreshListCtrl();
}

void MainFrame::OnClearSearch(wxCommandEvent& event) {
    m_currentRecords = m_dataManager.GetData();
    RefreshListCtrl();
}

void MainFrame::OnSortById(wxCommandEvent& event) {
    m_currentRecords = AlgorithmUtils::sortRecords(
        m_currentRecords,
        AlgorithmUtils::SortField::ID,
        AlgorithmUtils::SortOrder::ASCENDING
    );
    RefreshListCtrl();
}

void MainFrame::OnSortByDate(wxCommandEvent& event) {
    m_currentRecords = AlgorithmUtils::sortRecords(
        m_currentRecords,
        AlgorithmUtils::SortField::DATE,
        AlgorithmUtils::SortOrder::ASCENDING
    );
    RefreshListCtrl();
}

void MainFrame::OnSortByName(wxCommandEvent& event) {
    m_currentRecords = AlgorithmUtils::sortRecords(
        m_currentRecords,
        AlgorithmUtils::SortField::NAME,
        AlgorithmUtils::SortOrder::ASCENDING
    );
    RefreshListCtrl();
}

void MainFrame::OnSortByType(wxCommandEvent& event) {
    m_currentRecords = AlgorithmUtils::sortRecords(
        m_currentRecords,
        AlgorithmUtils::SortField::TYPE,
        AlgorithmUtils::SortOrder::ASCENDING
    );
    RefreshListCtrl();
}

void MainFrame::OnSortByAmount(wxCommandEvent& event) {
    m_currentRecords = AlgorithmUtils::sortRecords(
        m_currentRecords,
        AlgorithmUtils::SortField::AMOUNT,
        AlgorithmUtils::SortOrder::ASCENDING
    );
    RefreshListCtrl();
}

void MainFrame::OnSortByIdDesc(wxCommandEvent& event) {
    m_currentRecords = AlgorithmUtils::sortRecords(
        m_currentRecords,
        AlgorithmUtils::SortField::ID,
        AlgorithmUtils::SortOrder::DESCENDING
    );
    RefreshListCtrl();
}

void MainFrame::OnSortByDateDesc(wxCommandEvent& event) {
    m_currentRecords = AlgorithmUtils::sortRecords(
        m_currentRecords,
        AlgorithmUtils::SortField::DATE,
        AlgorithmUtils::SortOrder::DESCENDING
    );
    RefreshListCtrl();
}

void MainFrame::OnSortByNameDesc(wxCommandEvent& event) {
    m_currentRecords = AlgorithmUtils::sortRecords(
        m_currentRecords,
        AlgorithmUtils::SortField::NAME,
        AlgorithmUtils::SortOrder::DESCENDING
    );
    RefreshListCtrl();
}

void MainFrame::OnSortByTypeDesc(wxCommandEvent& event) {
    m_currentRecords = AlgorithmUtils::sortRecords(
        m_currentRecords,
        AlgorithmUtils::SortField::TYPE,
        AlgorithmUtils::SortOrder::DESCENDING
    );
    RefreshListCtrl();
}

void MainFrame::OnSortByAmountDesc(wxCommandEvent& event) {
    m_currentRecords = AlgorithmUtils::sortRecords(
        m_currentRecords,
        AlgorithmUtils::SortField::AMOUNT,
        AlgorithmUtils::SortOrder::DESCENDING
    );
    RefreshListCtrl();
}

// --- Helper Functions ---

void MainFrame::RefreshListCtrl() {
    m_listCtrl->DeleteAllItems();
    
    for (size_t i = 0; i < m_currentRecords.size(); ++i) {
        const Record& rec = m_currentRecords[i];
        long index = m_listCtrl->InsertItem(i, wxString::Format("%d", rec.id));
        
        // Format date
        m_listCtrl->SetItem(index, 1, wxString::Format("%04d-%02d-%02d", rec.year, rec.month, rec.day));
        m_listCtrl->SetItem(index, 2, wxString::FromUTF8(rec.name));
        m_listCtrl->SetItem(index, 3, wxString::FromUTF8(rec.type));
        
        // Format amount
        double amountToShow = static_cast<double>(rec.amount) / 1000.0;
        wxString amountStr = wxString::Format("%.2f", amountToShow);
        // Add negative sign for expenses
        if (std::string(rec.type) == "支出") {
            amountStr = "" + amountStr;
        }
        m_listCtrl->SetItem(index, 4, amountStr);
        
        m_listCtrl->SetItem(index, 5, wxString::FromUTF8(rec.note));
    }
}

void MainFrame::OnViewDetail(wxCommandEvent& event) {
    // 1. 获取当前选中的列表项
    long itemIndex = m_listCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1) {
        wxMessageBox(wxT("请先在列表中选择一条记录。"), wxT("提示"), wxOK | wxICON_INFORMATION);
        return;
    }

    // 2. 根据ID找到完整的Record数据
    int recordId = wxAtoi(m_listCtrl->GetItemText(itemIndex, 0));
    const auto& data = m_dataManager.GetData();
    auto it = std::find_if(data.begin(), data.end(), [recordId](const Record& r) { return r.id == recordId; });
    if (it == data.end()) return;

    // 3. 创建并设置弹窗
    wxDialog detailDialog(this, wxID_ANY, wxT("账目详情"), wxDefaultPosition, wxSize(400, 350)); // 稍微增加高度
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    detailDialog.SetSizer(mainSizer);

    // 普通信息行（单行）
    auto createInfoRow = [&](const wxString& label, const wxString& value) {
        wxBoxSizer* row = new wxBoxSizer(wxHORIZONTAL);
        row->Add(new wxStaticText(&detailDialog, wxID_ANY, label), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
        wxTextCtrl* ctrl = new wxTextCtrl(&detailDialog, wxID_ANY, value, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
        row->Add(ctrl, 1, wxEXPAND);
        mainSizer->Add(row, 0, wxEXPAND | wxALL, 5);
    };

    // Fill standard fields
    createInfoRow(wxT("日期:"), wxString::Format("%04d-%02d-%02d", it->year, it->month, it->day));
    createInfoRow(wxT("项目:"), wxString::FromUTF8(it->name));
    createInfoRow(wxT("类型:"), wxString::FromUTF8(it->type));
    double amountToShow = static_cast<double>(it->amount) / 1000.0;
    createInfoRow(wxT("金额(元):"), wxString::Format("%.2f", amountToShow));

    // 特殊处理：备注（多行）
    {
        wxBoxSizer* noteRow = new wxBoxSizer(wxHORIZONTAL);
        noteRow->Add(new wxStaticText(&detailDialog, wxID_ANY, wxT("备注:")), 0, wxALIGN_TOP | wxRIGHT | wxTOP, 5);
        wxTextCtrl* noteCtrl = new wxTextCtrl(
            &detailDialog, 
            wxID_ANY, 
            wxString::FromUTF8(it->note), 
            wxDefaultPosition, 
            wxSize(-1, 80), // 固定高度以显示多行
            wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH
        );
        noteRow->Add(noteCtrl, 1, wxEXPAND);
        mainSizer->Add(noteRow, 0, wxEXPAND | wxALL, 5);
    }

    // Add close button
    wxButton* closeButton = new wxButton(&detailDialog, wxID_OK, wxT("关闭"));
    mainSizer->Add(closeButton, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    // 4. Show the dialog
    detailDialog.ShowModal();
}
