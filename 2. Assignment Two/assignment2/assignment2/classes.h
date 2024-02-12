/*
Name: Chiayu Tu
Email: tuchi@oregonstate.edu
ONID: tuchi
ssh tuchi@hadoop-master.engr.oregonstate.edu
g++ -std=c++11 main.cpp -o main.out
main.out
*/

#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
#include <bits/stdc++.h>
#include <cstdio>

using namespace std;

// Create a slot to store the data into the buffer
struct Slot{
    int offset;  // Record starting position
    int length;  // Write dowm the length of the record
    //bool used;   // Is it in use
};

class Record {
    public:
        int id, manager_id;
        std::string bio, name;
        /*
        Record(const vector<string>& fields) {
            if (fields.size() < 4) {

                throw std::invalid_argument("Insufficient fields for initialization");
            }
            try {
                id = stoi(fields[0]);
                manager_id = stoi(fields[3]);
            } catch (const std::invalid_argument& e) {

                throw std::invalid_argument("Invalid integer format in fields");
            }
            name = fields[1];
            bio = fields[2];
        }       
        */

        Record(const vector<string>& fields) {
            if (fields.size() < 4) {
                throw std::invalid_argument("Insufficient fields for initialization");
            }
            try {
                // Check if fields[0] and fields[3] are not empty and contain only digits (and optionally a minus sign)
                if (!fields[0].empty() && std::all_of(fields[0].begin(), fields[0].end(), ::isdigit)) {
                    id = stoi(fields[0]);
                } else {
                    throw std::invalid_argument("Invalid ID format");
                }
                if (!fields[3].empty() && std::all_of(fields[3].begin(), fields[3].end(), ::isdigit)) {
                    manager_id = stoi(fields[3]);
                } else {
                    throw std::invalid_argument("Invalid Manager ID format");
                }
            } catch (const std::invalid_argument& e) {
                throw std::invalid_argument("Invalid integer format in fields: " + std::string(e.what()));
            } catch (const std::out_of_range& e) {
                throw std::out_of_range("Integer value out of range: " + std::string(e.what()));
            }
            name = fields[1];
            bio = fields[2];
        }


        void print() {
            cout << "\tID: " << id << "\n";
            cout << "\tNAME: " << name << "\n";
            cout << "\tBIO: " << bio << "\n";
            cout << "\tMANAGER_ID: " << manager_id << "\n";
        }

        // Function to convert a Record object to a comma-separated string representation
        std::string toString()
        {
            // Concatenate the id, name, bio, and manager_id fields with commas as separators
            return std::to_string(id) + "," + name + "," + bio + "," + std::to_string(manager_id);
        }
};


class StorageBufferManager {

    private:

        // You may declare variables based on your need 
        const int BLOCK_SIZE = 4096; // initialize the  block size allowed in main memory according to the question 
        int curBlockIndex = 0;
        const int Max_BLOCK = 3;   // Can only use three blocks in the assignment
        int numRecords;
        std::vector<std::string> buffer; // Create an empty vector "buffer" to store strings
        std::vector<std::vector<Slot>> slotDirectories; // Slot directory for each block
        std::string fileName;
        int bufferSize;

        // Insert new record 
        void insertRecord(Record record) {
    
            // Convert the record to the string
            std::string stringRecord = record.toString();
            // Count the length of the record
            int recordLength = stringRecord.size() + 1;    // Add 1 for '\n'

            // Check if block has a space for record
            if(!spaceRecord(curBlockIndex, recordLength)){

                // Move to the next block
                curBlockIndex = (curBlockIndex + 1) % Max_BLOCK;

                // Flush the next block for late use
                flushBlock(curBlockIndex);
            }

            // Insert the record to the current block
            buffer[curBlockIndex] += stringRecord;
            Slot newSlot = {static_cast<int>(buffer[curBlockIndex].size()), recordLength};
            slotDirectories[curBlockIndex].push_back(newSlot);
        }

        // Check if the block has a space for the record
        bool spaceRecord(int blockIndex, int recordLength){

            // The current block size
            int curBlockSize = buffer[blockIndex].size();
            // The current Slot directories size
            int curSlotSize = slotDirectories[blockIndex].size() * sizeof(Slot);
            return curBlockSize + curSlotSize + recordLength + sizeof(Slot) <= BLOCK_SIZE;

        }

        // Flush the block
        void flushBlock(int blockIndex){

            // Choose the block that needs to flush
            buffer[blockIndex].clear();
            slotDirectories[blockIndex].clear();
        }

            /*
            // Create new block if buffer is empty or last block cannot accommodate new record
            if(buffer.empty() || buffer.back().size() + stringRecord.size() + 1 > BLOCK_SIZE){

                buffer.push_back("");
            }

            // Add a new record to the last block
            std:: string &lastBlock = buffer.back();
            lastBlock.append(stringRecord + "\n");

            // Update the number of record
            numRecords++;         
            */

            /*
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
            */

    public:
        /*
        StorageBufferManager(){

            // Initialize the three blocks size
            buffer.resize(Max_BLOCK);
            slotDirectories.resize(Max_BLOCK);
        }
        */

        StorageBufferManager(const std::string &NewFileName):fileName(NewFileName), numRecords(0), bufferSize(0){

            std::ofstream outfile(fileName, std::ios::binary | std::ios::trunc);

            if(!outfile){

                throw std::runtime_error("Failed to open file: " + fileName);
            }

            // Initialize the three blocks size
            buffer.resize(Max_BLOCK);
            slotDirectories.resize(Max_BLOCK);
        }

        // Read csv file (Employee.csv) and add records to the (EmployeeRelation)
        void createFromFile(const std::string &csvFName) {

            std::ifstream infile(csvFName);

            if(!infile){

                throw std::runtime_error("Failed to open file: " + csvFName);
            }

            // Declare a string line to hold each line read from csvFName file
            std::string line;

            // Declare an interger to check the current line number
            int lineNumber = 1;

            while(std::getline(infile, line)){
                
                // Test
                /*
                std::cout << "Line number: " << lineNumber << std::endl;
                std::cout << "Line I got: " << line << std::endl;
                */
                lineNumber++;

                // Each tuple is in a separate line and the fields of each record are separated by commas
                // Declare a string vector to hold the individual fields in the current line.
                std::vector<std::string> fields;

                // Declare a stringstream from the current line to mark the fields
                std::stringstream ss(line);

                // Declare a string variable to hold a mark field
                std::string field;

                // Read each line from the input file
                while (std::getline(ss, field, ',')) {

                    // Add the marked fields to the vector of fields
                    fields.push_back(field);
                }
                
                // Check if the field has enough space
                if(fields.size() >= 4){

                    // Create a Record object from the vector fields
                    Record record(fields);

                    // Insert the record to the buffer
                    insertRecord(record);
                }
            }
        }

        // Given an ID, find the relevant record and print it
        Record findRecordById(int id) {

            for(int i = 0; i < Max_BLOCK; ++i){
                // Assume each block contains multiple "\n" records
                // Convert a record into a vector object containing each field of the record
                std::stringstream ss(buffer[i]);
                std::string recordString;

                while(std::getline(ss, recordString)){

                    std::vector<std::string> fields;
                    std::stringstream rs(recordString);
                    std::string field;

                    while(std::getline(rs, field, ',')){

                        fields.push_back(field);
                    }

                    if(fields.size() < 4){

                        continue;
                    }

                    Record record(fields);

                    if(record.id == id){

                        return record;
                    }
                }

            }

            throw std::runtime_error("Record not found with ID: " + std::to_string(id));
        }
};

            /*
            // Traverse all records and find records with matching IDs
            for(const std::string &block : buffer){

                // Assume each block contains multiple "\n" records
                // Convert a record into a vector object containing each field of the record
                std::stringstream blockStream(block);

                std::string recordString;

                while (std::getline(blockStream, recordString)) {

                    // Check each record
                    std::stringstream recordStream(recordString);
                    std::vector<std::string> fields;
                    std::string field;

                    while (std::getline(recordStream, field, ',')) {
                        
                        fields.push_back(field);
                    }

                    if (fields.size() < 4) {

                        // Check if the number of fields is sufficient
                        continue;
                    }

                    Record record(fields);

                    if (record.id == id) {

                        // Find the record
                        return record;
                    }
                }
            }

            // If no matching record is found, throw an exception or return an empty record
            throw std::runtime_error("Record not found with ID: " + std::to_string(id));
            */

