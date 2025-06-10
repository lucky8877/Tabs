#include "Table/SortTable.h"
#include "Table/ArrayTable.h"

void SortTable::InsertSort(PTabRecord *data, size_t count){
    PTabRecord tmp;
    _efficiency = 0;
    for(int i = 0, j; i < _dataCount; i++){
        tmp =_records[i];
        for(j = i - 1; j > -1; j--){
            if(_records[j]->_key > tmp->_key){
                _records[j+1] = _records[j];
                _efficiency++;
            }else{
                break;
            }
        }
        _records[j+1] = tmp;
    }
}

void SortTable::MergeSort(PTabRecord *data, size_t count) {
    if (count <= 1) return;
    
    PTabRecord* buffer = new PTabRecord[count];
    
    try {
        MergeSorter(data, buffer, count);
        delete[] buffer; 
    } catch (...) {
        delete[] buffer; 
        throw;
    }
}

void SortTable::MergeSorter(PTabRecord* &data, PTabRecord* &buffer, size_t size) {
    if (size <= 1) return; 
    
    size_t mid = size / 2;
    PTabRecord* left = data;
    PTabRecord* right = data + mid;
    
    MergeSorter(left, buffer, mid);
    MergeSorter(right, buffer, size - mid);
    
    MergeData(data, buffer, size, mid);
}

void SortTable::MergeData(PTabRecord* &data, PTabRecord* &buffer, size_t size, size_t mid) {
    size_t i = 0, left = 0, right = mid;
    
    while (left < mid && right < size) {
        if (data[left]->GetKey() <= data[right]->GetKey()) {
            buffer[i++] = data[left++];
        } else {
            buffer[i++] = data[right++];
        }
        _efficiency++;
    }
    
    while (left < mid) {
        buffer[i++] = data[left++];
    }
    while (right < size) {
        buffer[i++] = data[right++];
    }
    
    for (i = 0; i < size; i++) {
        data[i] = buffer[i];
    }
    
    _efficiency += size; 
}

void SortTable::QuickSort(PTabRecord *data, size_t count){
    size_t pivot;
    size_t n1, n2;
    if(count > 1){
        QuickSplit(data, count, pivot);
        n1 = pivot + 1;
        n2 = count - n1;
        QuickSort(data, n1 - 1);
        QuickSort(data + n1, n2);
    }
}
void SortTable::QuickSplit(PTabRecord *data, size_t count, size_t& pivot){
    PTabRecord ppivot = data[0];
    PTabRecord tmp;
    size_t i1 = 1, i2 = count - 1;
    while(i1 <= i2){
        while (i1 < count && !(data[i1]->GetKey() > ppivot->GetKey()))
        {
            i1++;
        }
        while (data[i2]->GetKey() > ppivot->GetKey())
        {
            i2--;
        }
        if(i1 < i2){
            tmp = data[i1];
            data[i1] = data[i2];
            data[i2] = tmp;
        }
    }
    _efficiency += int(count);
    pivot = i2;
    data[0] = data[i2];
    data[i2] = ppivot;
}

void SortTable::SortData(){
    _efficiency = 0;
        switch (_sortMethod)
    {
        case SortMethod::Insert:
            InsertSort(_records, _dataCount);
            break;
        case SortMethod::Merge:
            MergeSort(_records, _dataCount);
            break;
        case SortMethod::Quick:
            QuickSort(_records, _dataCount);
            break;
        default:
            throw "Unavailable sort method";
    }
}
    
SortTable& SortTable::operator=(const ScanTable& st){
    if (_records != nullptr){
        for (size_t i = 0; i < _dataCount; i++){
            delete _records[i];
        }
        delete [] _records;
    }

    _tabSize = st._tabSize;
    _dataCount = st.GetDataCount();
    _records = new PTabRecord[_tabSize];
    for(size_t i = 0; i < _dataCount; i++){
        _records[i] = (PTabRecord)st._records[i]->GetCopy();
    }

    SortData();
    _curPos = 0;
    return *this;
}

void SortTable::ResetEff() {
    _efficiency = 0;
}
PDataValue SortTable::FindRecord(Key key) {
    if (_dataCount == 0) {
        _curPos = 0;
        return nullptr;
    }

    size_t i, i1 = 0, i2 = _dataCount - 1;
    _efficiency = 0;

    while (i1 <= i2) {
        _efficiency++;
        i = (i1 + i2) / 2;

        if (_records[i] == nullptr) {
            int left = i - 1;
            int right = i + 1;
            bool found = false;

            while (left >= (int)i1 || right <= (int)i2) {
                if (left >= (int)i1 && _records[left] != nullptr) {
                    i = left;
                    found = true;
                    break;
                }
                if (right <= (int)i2 && _records[right] != nullptr) {
                    i = right;
                    found = true;
                    break;
                }
                left--;
                right++;
            }

            if (!found) {
                _curPos = i1;
                return nullptr;
            }
        }

        Key currentKey = _records[i]->GetKey();
        if (currentKey == key) {
            _curPos = i;
            return _records[i]->GetData();
        }
        else if (currentKey > key) {
            i2 = i - 1;
        }
        else {
            i1 = i + 1;  
        }
    }

    _curPos = i1;
    return nullptr;
}
 
    void SortTable::DelRecotd(Key key){
        if (IsEmpty()) {
            throw "Table is empty";
        }
        
        PDataValue tmp = FindRecord(key);
        if (tmp == nullptr) {
            return;
        }

        for (int i =0;i < _dataCount;i++) {
            if (_records[i] != nullptr && _records[i]->GetKey() == key) {
                delete _records[i];
                _records[i] = nullptr;
            }
        }
     
        // delete _records[_curPos];
        // _records[_curPos] = nullptr;
        // for (size_t i = _curPos; i < _dataCount - 1; i++) {
        //     _records[i] = _records[i + 1]; 
        // }
        // _records[_dataCount - 1] = nullptr; 
        // _dataCount--;
        // Reset(); 
    }

// void SortTable::DelRecotd(const Key key) {
//     if (IsEmpty())
//         throw "table is empty";
    // PDataValue tmp = FindRecord(key);
    // if (!FindRecord(key)){
    //     throw "Element not found";
    // }
    // else{
    //     for (size_t i = _curPos;i < _dataCount - 1;i++) {
    //         _efficiency++;
    //         _records[i] = _records[i + 1];

    //     }
    //     _records[_dataCount] = nullptr;
    //     delete tmp;
    //     _dataCount--;
    //     _curPos = 0;
//     }
// }
void SortTable::InsRecord(Key key, PDataValue data){
        if(IsFull()){
            throw "Table is full";
        }

        _records[_curPos] = new TabRecord(key, data);
        _dataCount++;
        _curPos++;
    }
