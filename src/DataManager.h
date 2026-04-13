// DataManager.h
#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <vector>
#include <cstdint>
#include <string>


struct Record {
    int id;                     // 序号（自增）
    int year, month, day;       // 日期
    char name[31];              // 收支项目名称（≤30字符）
    char type[6];               // 类型："收入" 或 "支出"（4字+'\0'）
    long long amount;           // 金额（正/负表示收支,单位为0.001元）
    char note[51];              // 备注（≤50字符）
};


class DataManager {
public:
    DataManager(const std::string& filename);
    bool LoadData();
    bool SaveData() const;
    std::vector<int64_t>& GetData();
    void SetData(const std::vector<int64_t>& data);

private:
    std::string m_filename;
    std::vector<int64_t> m_data;
};

#endif // DATA_MANAGER_H