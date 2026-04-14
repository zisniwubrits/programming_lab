// DataManager.h
#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <vector>
#include <cstdint>
#include <string>

struct Record {
    int id;                     // Serial number (auto-increment)
    int year, month, day;       // Date
    char name[31];              // Item name (≤30 chars)
    char type[8];               // Type: "Income" or "Expense" (5 chars + '\0')
    long long amount;           // Amount (positive/negative for income/expense, unit: 0.001)
    char note[51];              // Note (≤50 chars)
};

class DataManager {
public:
    DataManager(const std::string& filename);
    bool LoadData();
    bool SaveData() const;
    std::vector<Record>& GetData();
    void SetData(const std::vector<Record>& data);

private:
    std::string m_filename;
    std::vector<Record> m_data;
};

#endif // DATA_MANAGER_H