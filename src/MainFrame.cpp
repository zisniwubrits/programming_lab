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
    ID_SORT_AMOUNT_DESC
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

    // 1. Create input form area
    wxStaticBoxSizer* formSizer = new wxStaticBoxSizer(wxVERTICAL, mainPanel, wxT("账目详情"));
    
    // Date row
    wxBoxSizer* dateRow = new wxBoxSizer(wxHORIZONTAL);
    dateRow->Add(new wxStaticText(mainPanel, wxID_ANY, wxT("日期:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    m_datePicker = new wxDatePickerCtrl(mainPanel, wxID_ANY);
    dateRow->Add(m_datePicker, 1);
    formSizer->Add(dateRow, 0, wxEXPAND | wxALL, 5);

    // Project name row
    wxBoxSizer* nameRow = new wxBoxSizer(wxHORIZONTAL);
    nameRow->Add(new wxStaticText(mainPanel, wxID_ANY, wxT("项目:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    m_nameTextCtrl = new wxTextCtrl(mainPanel, wxID_ANY);
    nameRow->Add(m_nameTextCtrl, 1);
    formSizer->Add(nameRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // Type and amount row
    wxBoxSizer* typeAmountRow = new wxBoxSizer(wxHORIZONTAL);
    // Type
    typeAmountRow->Add(new wxStaticText(mainPanel, wxID_ANY, wxT("类型:")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    m_typeChoice = new wxChoice(mainPanel, wxID_ANY);
    m_typeChoice->Append(wxT("收入"));
    m_typeChoice->Append(wxT("支出"));
    m_typeChoice->SetSelection(1); // Default to "Expense"
    typeAmountRow->Add(m_typeChoice, 0, wxRIGHT, 10);
    // Amount
    typeAmountRow->Add(new wxStaticText(mainPanel, wxID_ANY, wxT("金额(元):")), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    m_amountTextCtrl = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(100, -1));
    typeAmountRow->Add(m_amountTextCtrl, 0);
    formSizer->Add(typeAmountRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    // Note row
    wxBoxSizer* noteRow = new wxBoxSizer(wxHORIZONTAL);
    noteRow->Add(new wxStaticText(mainPanel, wxID_ANY, wxT("备注")), 0, wxALIGN_TOP | wxRIGHT, 5);
    m_noteTextCtrl = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 60), wxTE_MULTILINE);
    noteRow->Add(m_noteTextCtrl, 1, wxEXPAND);
    formSizer->Add(noteRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);

    mainSizer->Add(formSizer, 0, wxEXPAND | wxALL, 10);

    // 2. Create action buttons area
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    m_addButton = new wxButton(mainPanel, wxID_ADD, wxT("添加"));
    m_updateButton = new wxButton(mainPanel, wxID_REPLACE, wxT("修改"));
    m_deleteButton = new wxButton(mainPanel, wxID_DELETE, wxT("删除"));
    buttonSizer->Add(m_addButton, 0, wxRIGHT, 5);
    buttonSizer->Add(m_updateButton, 0, wxRIGHT, 5);
    buttonSizer->Add(m_deleteButton, 0);
    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    // 3. Create data display area
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
    std::strncpy(newRecord.type, utf8Type.data(), 6);       // 复制6字节
    newRecord.type[6] = '\0';                               // 确保null结尾
    
    // Parse amount, convert to 0.001 unit
    double amountDbl;
    if (!m_amountTextCtrl->GetValue().ToDouble(&amountDbl) || amountDbl <= 0) {
        wxMessageBox(wxT("请输入有效的金额！"), wxT("错误"), wxOK | wxICON_ERROR);
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
    
    // Update current records and refresh UI
    m_currentRecords = m_dataManager.GetData();
    RefreshListCtrl();
    m_nameTextCtrl->Clear();
    m_amountTextCtrl->Clear();
    m_noteTextCtrl->Clear();
    m_nameTextCtrl->SetFocus();
}

void MainFrame::OnUpdateRecord(wxCommandEvent& event) {
    long itemIndex = m_listCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex == -1) {
        // 修正：正确的提示信息
        wxMessageBox(wxT("请先在列表中选择一条记录进行修改。"), wxT("提示"), wxOK | wxICON_INFORMATION);
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
    std::strncpy(it->type, utf8Type.data(), 6);
    it->type[6] = '\0';
    
    double amountDbl;
    if (!m_amountTextCtrl->GetValue().ToDouble(&amountDbl) || amountDbl <= 0) {
        wxMessageBox(wxT("请输入有效的金额！"), wxT("错误"), wxOK | wxICON_ERROR);
        return;
    }
    it->amount = static_cast<long long>(amountDbl * 1000 + 0.5);

    wxString noteStr = m_noteTextCtrl->GetValue();
    auto utf8Note = noteStr.ToUTF8();
    std::strncpy(it->note, utf8Note.data(), 50);
    it->note[50] = '\0';

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

// --- Menu Event Handlers ---

void MainFrame::OnStatistics(wxCommandEvent& event) {
    auto stats = AlgorithmUtils::calculateStatistics(m_dataManager.GetData());
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
        if (std::string(rec.type) == "Expense") {
            amountStr = "-" + amountStr;
        }
        m_listCtrl->SetItem(index, 4, amountStr);
        
        m_listCtrl->SetItem(index, 5, wxString::FromUTF8(rec.note));
    }
}