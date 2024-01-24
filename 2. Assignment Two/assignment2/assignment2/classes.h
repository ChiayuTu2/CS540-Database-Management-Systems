#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
using namespace std;

class Record {
public:
    int id, manager_id;
    std::string bio, name;

    Record(vector<std::string> fields) {
        id = stoi(fields[0]);
        name = fields[1];
        bio = fields[2];
        manager_id = stoi(fields[3]);
    }

    void print() {
        cout << "\tID: " << id << "\n";
        cout << "\tNAME: " << name << "\n";
        cout << "\tBIO: " << bio << "\n";
        cout << "\tMANAGER_ID: " << manager_id << "\n";
    }
};


class StorageBufferManager {

private:
    
    const int BLOCK_SIZE = ; // initialize the  block size allowed in main memory according to the question 

    // You may declare variables based on your need 

    // Insert new record 
    void insertRecord(Record record) {

        // No records written yet
        if (numRecords == 0) {
            // Initialize first block

        }
        // Add record to the block


        // Take neccessary steps if capacity is reached (you've utilized all the blocks in main memory)


    }

public:
    StorageBufferManager(string NewFileName) {
        
        //initialize your variables

        // Create your EmployeeRelation file 

        
    }

    // Read csv file (Employee.csv) and add records to the (EmployeeRelation)
    void createFromFile(string csvFName) {
        
    }

    // Given an ID, find the relevant record and print it
    Record findRecordById(int id) {
        
    }
};
