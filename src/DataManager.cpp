// DataManager.cpp
#include "DataManager.h"
#include <fstream>
#include <iostream>

DataManager::DataManager(const std::string& filename) : m_filename(filename) {}

bool DataManager::LoadData() {
    std::ifstream file(m_filename);
    if (!file.is_open()) return false;

    m_data.clear();
    int64_t value;
    while (file >> value) {
        m_data.push_back(value);
    }
    file.close();
    return true;
}

bool DataManager::SaveData() const {
    std::ofstream file(m_filename);
    if (!file.is_open()) return false;

    for (const auto& item : m_data) {
        file << item << '\n';
    }
    file.close();
    return true;
}

std::vector<int64_t>& DataManager::GetData() {
    return m_data;
}

void DataManager::SetData(const std::vector<int64_t>& data) {
    m_data = data;
}