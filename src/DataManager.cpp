// DataManager.cpp
#include "DataManager.h"
#include <cstdio>

DataManager::DataManager(const std::string& filename) : m_filename(filename) {}

bool DataManager::LoadData() {
    FILE* file = fopen(m_filename.c_str(), "rb");
    if (!file) return false;

    m_data.clear();
    Record record;
    while (fread(&record, sizeof(Record), 1, file) == 1) {
        m_data.push_back(record);
    }
    fclose(file);
    return true;
}

bool DataManager::SaveData() const {
    FILE* file = fopen(m_filename.c_str(), "wb");
    if (!file) return false;

    for (const auto& record : m_data) {
        fwrite(&record, sizeof(Record), 1, file);
    }
    fclose(file);
    return true;
}

std::vector<Record>& DataManager::GetData() {
    return m_data;
}

void DataManager::SetData(const std::vector<Record>& data) {
    m_data = data;
}