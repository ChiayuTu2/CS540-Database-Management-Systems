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
#include <bits/stdc++.h>
#include <cstdio>

using namespace std;

class Record {
    public:
        int id, manager_id;
        std::string bio, name;

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
        
        const int BLOCK_SIZE = 4096; // initialize the  block size allowed in main memory according to the question 
        int numRecords;
        std::vector<std::string> buffer; // Create an empty vector "buffer" to store strings
        std::string fileName;
        int bufferSize;


        // You may declare variables based on your need 

        // Insert new record 
        void insertRecord(Record record) {
    
            // Convert the record to the string
            std::string stringRecord = record.toString();

            // Create new block if buffer is empty or last block cannot accommodate new record
            if(buffer.empty() || buffer.back().size() + stringRecord.size() + 1 > BLOCK_SIZE){

                buffer.push_back("");
            }

            // Add a new record to the last block
            std:: string &lastBlock = buffer.back();
            lastBlock.append(stringRecord + "\n");

            // Update the number of record
            numRecords++;

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
            */
        }

    public:
        StorageBufferManager(const std::string &NewFileName):fileName(NewFileName), numRecords(0), bufferSize(0){

            std::ofstream outfile(fileName, std::ios::binary | std::ios::trunc);

            if(!outfile){

                throw std::runtime_error("Failed to open file: " + fileName);
            }
        }
        /*
        StorageBufferManager(string NewFileName) {
            
            //initialize your variables
            fileName = NewFileName;  // Initialize the file name
            numRecords = 0;          // Initialize the number of records to 0
            bufferSize = 0;          // Initialize the buffer size to 0

            // Create your EmployeeRelation file 
            // Open the file for writing in binary mode
            FILE *outfile = fopen(fileName.c_str(), "wb");
            if(outfile == nullptr){
                // If file opening error
                throw std::runtime_error("Failed to open file");
            }

            fclose(outfile);
            
        }
        */

        // Read csv file (Employee.csv) and add records to the (EmployeeRelation)
        void createFromFile(const std::string &csvFName) {
            /*
            // Open the input file for reading in text model
            FILE *infile = fopen(csvFName.c_str(), "r");
            if(infile == nullptr){
                return;
            }
            */

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
                std::stringstream stringLine(line);

                // Declare a string variable to hold a mark field
                std::string field;

                // Read each line from the input file
                while (std::getline(stringLine, field, ',')) {

                    // Add the marked fields to the vector of fields
                    fields.push_back(field);
                }

                // Create a Record object from the vector fields
                Record record(fields);

                // Insert the record to the buffer
                insertRecord(record);
            }
        }

        // Given an ID, find the relevant record and print it
        Record findRecordById(int id) {

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
        }
};
