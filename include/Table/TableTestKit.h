#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <limits>
#include "Table/SortTable.h"
#include "Table/ScanTable.h"
#include "Table/TabRecord.h"
#include "Table/Marks.h"
class TableTestKit {
public:    
    int count = 10000;

TableTestKit(int _count) {
    count = _count;
}

TabRecord** generateTestRecords() {
    std::srand(std::time(NULL));
    TabRecord** records = new TabRecord*[count];
    
    for (int i = 0; i < count; i++) {
        Key key = std::to_string(i + 1);
        PDataValue val = new Marks(1, 2, 3, 4, 5);
        records[i] = new TabRecord(key, val);
    }
    
    for (int i = 0; i < count; i++) {
        int swapWith = rand() % count;
        Key temp = records[i]->GetKey();
        records[i]->SetKey(records[swapWith]->GetKey());
        records[swapWith]->SetKey(temp);
    }
    
    return records;
}

void FillTable(ScanTable &table, TabRecord** records) {
    for (int i = 0; i < count; i++) {
        table.InsRecord(records[i]->GetKey(), records[i]->GetData()->GetCopy());
    }
}

void TestScanTable() {
    std::cout << "\n=== Testing ScanTable ===" << std::endl;
    
    TabRecord** testRecords = generateTestRecords();
    ScanTable scanTable(count);
    auto insertStart = std::chrono::high_resolution_clock::now();
    FillTable(scanTable, testRecords);
    auto insertEnd = std::chrono::high_resolution_clock::now();
    int tstCount = 1000;

    int totalSearchSteps = 0;
    int maxSearchSteps = 0;
    int minSearchSteps = std::numeric_limits<int>::max();
    auto searchStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < tstCount; i++) {
        scanTable.FindRecord(testRecords[rand() % count]->GetKey());
        int steps = scanTable.GetEfficiency();
        totalSearchSteps += steps;
        if (steps > maxSearchSteps) maxSearchSteps = steps;
        if (steps < minSearchSteps) minSearchSteps = steps;
    }
    auto searchEnd = std::chrono::high_resolution_clock::now();
    
    ScanTable delTable(count);
    FillTable(delTable, testRecords);
    int totalDelSteps = 0;
    int maxDelSteps = 0;
    int minDelSteps = std::numeric_limits<int>::max();
    int revCount = count;
    auto delStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < tstCount; i++) {
        delTable.DelRecotd(testRecords[i]->GetKey());
        revCount--;
        int steps = delTable.GetEfficiency();
        totalDelSteps += steps;
        if (steps > maxDelSteps) maxDelSteps = steps;
        if (steps < minDelSteps) minDelSteps = steps;
    }
    auto delEnd = std::chrono::high_resolution_clock::now();
    std::cout << "Insert Time: " 
              << std::chrono::duration<double, std::milli>(insertEnd - insertStart).count() 
              << " ms" << std::endl;
              
    std::cout << "Search Time: " 
              << std::chrono::duration<double, std::milli>(searchEnd - searchStart).count() 
              << " ms" << std::endl;
    std::cout << "Search Efficiency:" << std::endl;
    std::cout << "  Max steps: " << maxSearchSteps << std::endl;
    std::cout << "  Min steps: " << minSearchSteps << std::endl;
    std::cout << "  Avg steps: " << (totalSearchSteps)/tstCount << std::endl;
    
    std::cout << "Delete Time: " 
              << std::chrono::duration<double, std::milli>(delEnd - delStart).count() 
              << " ms" << std::endl;
    std::cout << "Delete Efficiency:" << std::endl;
    std::cout << "  Max steps: " << maxDelSteps << std::endl;
    std::cout << "  Min steps: " << minDelSteps << std::endl;
    std::cout << "  Avg steps: " << (totalDelSteps)/tstCount << std::endl;
}

void TestSortTable() {
    std::cout << "\n=== Testing SortTable ===" << std::endl;
    SortMethod methods[] = {Insert, Merge, Quick};
    std::string methodsNames[] = {"Insert", "Merge", "Quick"};
    for (int i = 0; i < 3; i++) {
        TabRecord** testRecords = generateTestRecords();
        std::cout << "\nMethod: " <<methodsNames[i]<<" Sort" << std::endl;
        SortTable table(count);
        table.SetSortMethod(methods[i]);
        
        auto insertStart = std::chrono::high_resolution_clock::now();
        FillTable(table, testRecords);
        auto insertEnd = std::chrono::high_resolution_clock::now();
        
        table.ResetEff();
        auto sortStart = std::chrono::high_resolution_clock::now();
        table.SortData();
        auto sortEnd = std::chrono::high_resolution_clock::now();
        int sortSteps = table.GetEfficiency();
        
        table.ResetEff();
        auto searchStart = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < count; j++) {
            table.FindRecord(testRecords[j]->GetKey());
        }
        auto searchEnd = std::chrono::high_resolution_clock::now();
        int searchSteps = table.GetEfficiency();
        
        std::cout << "Insert Time: " 
                  << std::chrono::duration<double, std::milli>(insertEnd - insertStart).count() 
                  << " ms" << std::endl;
        std::cout << "Search Steps: " 
                  << searchSteps 
                  << " ms" << std::endl;
        std::cout << "Sort Time: " 
                  << std::chrono::duration<double, std::milli>(sortEnd - sortStart).count() 
                  << " ms" << std::endl;
        std::cout << "Sort Steps: " << sortSteps << std::endl;
        
        std::cout << "Search Time: " 
                  << std::chrono::duration<double, std::milli>(searchEnd - searchStart).count() 
                  << " ms" << std::endl;
    }
}
void TestHashTable() {
    
}
};

