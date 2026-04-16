#ifndef ALGORITHM_UTILS_H
#define ALGORITHM_UTILS_H

#include <vector>
#include <string>
#include <functional>
#include "DataManager.h"

namespace AlgorithmUtils {
    // 统计功能
    struct Statistics {
        double totalIncome;
        double totalExpense;
        double netBalance;
        size_t recordCount;
    };
    
    Statistics calculateStatistics(const std::vector<Record>& records);
    
    // 查找功能
    enum class SearchField {
        NAME,
        TYPE,
        NOTE,
        DATE_RANGE
    };
    
    struct DateRange {
        int startYear, startMonth, startDay;
        int endYear, endMonth, endDay;
    };
    
    std::vector<Record> searchRecords(const std::vector<Record>& records, 
                                    SearchField field, 
                                    const std::string& keyword = "",
                                    const DateRange* dateRange = nullptr);
    
    // 排序功能
    enum class SortField {
        ID,
        DATE,
        NAME,
        TYPE,
        AMOUNT
    };
    
    enum class SortOrder {
        ASCENDING,
        DESCENDING
    };
    
    std::vector<Record> sortRecords(const std::vector<Record>& records, 
                                  SortField field, 
                                  SortOrder order = SortOrder::ASCENDING);
}

#endif // ALGORITHM_UTILS_H