/*
Name: Chiayu Tu
Email: tuchi@oregonstate.edu
ONID: tuchi
*/

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
            // stoi: used to convert strings to integers.
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
        
        const int BLOCK_SIZE = 4096; // initialize the  block size allowed in main memory according to the question 
        int numRecords;
        std::vector<std::string> buffer; // Create an empty vector "line" to store strings



        // You may declare variables based on your need 

        // Insert new record 
        void insertRecord(Record record) {

            // Convert the record to the string
            std::string stringRecord = record.toString();
        
            // No records written yet
            if (numRecords == 0) {
                // Initialize first block
                std::string block;
                // Check the length of the block
                std::cout << "The length of block:" << block.length() <<  std::endl;
                buffer.push_back(block);

                // Increment the number of records
                numRecords++;
            }
            
            // Take neccessary steps if capacity is reached (you've utilized all the blocks in main memory)
            // Add record to the block
            // Add a record to the last block in the buffer
            // Use a reference to directly manipulate this element in the buffer instead of creating a copy of it
            std::string &lastBlock = buffer.back();
            // Test 
            std::cout << "Last block content: " << buffer.back() << std::endl;

            // Calculate the rest of the space in the last block
            int restSpace = BLOCK_SIZE - lastBlock.size();
            // Test
            std::cout << "The rest of buffer size: " << restSpace << std::endl;
            std::cout << "The last block size: " << lastBlock.size() << std::endl;

            // Append the stringRecord to the last block
            lastBlock.append(stringRecord);
            // Append a newline character to the last data block to separate records
            lastBlock.append("\n");

            // Increment the buffer_size and numRecords
            buffer_size++;
            numRecords++;
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
