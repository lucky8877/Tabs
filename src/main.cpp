#include "Table/TableTestKit.h"



int main() {
    int count = 10;
    int delCount = 3;
    TableTestKit TestKit(count); 
    
    SortTable table(count);
    
    TabRecord** records = TestKit.generateTestRecords();
    std::cout<<"====================="<<std::endl;
 
     for (int i = 0; i < count; i++) {
        table.InsRecord(records[i]->GetKey(), records[i]->GetData()->GetCopy());
        std::cout<<records[i]->GetKey()<<std::endl;
    }
    table.SetSortMethod(SortMethod::Insert);
    table.SortData();
    std::cout<<"====================="<<std::endl;
    for (int i = 0; i < delCount; i++) {
        std::cout<<records[i]->GetKey()<<std::endl;
        table.DelRecotd(records[i]->GetKey());
    }

    std::cout<<"====================="<<std::endl;
    for (int i = 0; i < count; i++) {
        std::cout<<records[i]->GetKey()<<" "<<table.FindRecord(records[i]->GetKey())<<std::endl;
    }
    return 0;
}