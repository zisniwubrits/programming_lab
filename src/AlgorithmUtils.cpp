#include "AlgorithmUtils.h"
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

namespace AlgorithmUtils {
    
    // 统计功能实现
    Statistics calculateStatistics(const std::vector<Record>& records) {
        Statistics stats = {0.0, 0.0, 0.0, records.size()};
        
        for (const auto& record : records) {
            double amount = static_cast<double>(record.amount) / 1000.0;
            // std::printf(std::to_string(record.amount).c_str());
            // std::printf("\n");
            std::string type(record.type);
            
            if (type == "\u6536\u5165") {
                stats.totalIncome += amount;
            } else if (type == "\u652f\u51fa") {
                stats.totalExpense += amount;
            }
        }
        
        stats.netBalance = stats.totalIncome - stats.totalExpense;
        // std::printf(std::to_string(stats.netBalance).c_str());
        return stats;
    }
    
    // 辅助函数：比较日期
    bool isDateInRange(int year, int month, int day, const DateRange& range) {
        // 将日期转换为可比较的整数格式 YYYYMMDD
        long currentDate = year * 10000 + month * 100 + day;
        long startDate = range.startYear * 10000 + range.startMonth * 100 + range.startDay;
        long endDate = range.endYear * 10000 + range.endMonth * 100 + range.endDay;
        
        return currentDate >= startDate && currentDate <= endDate;
    }
    
    // 查找功能实现
    std::vector<Record> searchRecords(const std::vector<Record>& records, 
                                    SearchField field, 
                                    const std::string& keyword,
                                    const DateRange* dateRange) {
        std::vector<Record> results;
        
        if (field == SearchField::NAME) {
            for (const auto& record : records) {
                std::string name(record.name);
                if (name.find(keyword) != std::string::npos) {
                    results.push_back(record);
                }
            }
        }
        else if (field == SearchField::TYPE) {
            for (const auto& record : records) {
                std::string type(record.type);
                if (type == keyword) {
                    results.push_back(record);
                }
            }
        }
        else if (field == SearchField::NOTE) {
            for (const auto& record : records) {
                std::string note(record.note);
                if (note.find(keyword) != std::string::npos) {
                    results.push_back(record);
                }
            }
        }
        else if (field == SearchField::DATE_RANGE) {
            if (dateRange) {
                for (const auto& record : records) {
                    if (isDateInRange(record.year, record.month, record.day, *dateRange)) {
                        results.push_back(record);
                    }
                }
            } else {
                results = records; // 如果没有日期范围，返回所有记录
            }
        }
        
        return results;
    }
    
    // 排序功能实现（只使用sort）
    std::vector<Record> sortRecords(const std::vector<Record>& records, 
                                  SortField field, 
                                  SortOrder order) {
        std::vector<Record> sortedRecords = records;
        
        if (order == SortOrder::ASCENDING) {
            if (field == SortField::ID) {
                std::sort(sortedRecords.begin(), sortedRecords.end(), 
                         [](const Record& a, const Record& b) {
                             return a.id < b.id;
                         });
            }
            else if (field == SortField::DATE) {
                std::sort(sortedRecords.begin(), sortedRecords.end(), 
                         [](const Record& a, const Record& b) {
                             if (a.year != b.year) return a.year < b.year;
                             if (a.month != b.month) return a.month < b.month;
                             return a.day < b.day;
                         });
            }
            else if (field == SortField::NAME) {
                std::sort(sortedRecords.begin(), sortedRecords.end(), 
                         [](const Record& a, const Record& b) {
                             return std::string(a.name) < std::string(b.name);
                         });
            }
            else if (field == SortField::TYPE) {
                std::sort(sortedRecords.begin(), sortedRecords.end(), 
                         [](const Record& a, const Record& b) {
                             return std::string(a.type) < std::string(b.type);
                         });
            }
            else if (field == SortField::AMOUNT) {
                std::sort(sortedRecords.begin(), sortedRecords.end(), 
                         [](const Record& a, const Record& b) {
                             return a.amount < b.amount;
                         });
            }
        }
        else { // DESCENDING
            if (field == SortField::ID) {
                std::sort(sortedRecords.begin(), sortedRecords.end(), 
                         [](const Record& a, const Record& b) {
                             return a.id > b.id;
                         });
            }
            else if (field == SortField::DATE) {
                std::sort(sortedRecords.begin(), sortedRecords.end(), 
                         [](const Record& a, const Record& b) {
                             if (a.year != b.year) return a.year > b.year;
                             if (a.month != b.month) return a.month > b.month;
                             return a.day > b.day;
                         });
            }
            else if (field == SortField::NAME) {
                std::sort(sortedRecords.begin(), sortedRecords.end(), 
                         [](const Record& a, const Record& b) {
                             return std::string(a.name) > std::string(b.name);
                         });
            }
            else if (field == SortField::TYPE) {
                std::sort(sortedRecords.begin(), sortedRecords.end(), 
                         [](const Record& a, const Record& b) {
                             return std::string(a.type) > std::string(b.type);
                         });
            }
            else if (field == SortField::AMOUNT) {
                std::sort(sortedRecords.begin(), sortedRecords.end(), 
                         [](const Record& a, const Record& b) {
                             return a.amount > b.amount;
                         });
            }
        }
                 
        return sortedRecords;
    }
}