/*
Name: Chiayu Tu
Email: tuchi@oregonstate.edu
ONID: tuchi
ssh tuchi@hadoop-master.engr.oregonstate.edu
g++ -std=c++11 main.cpp -o main.out
main.out
*/
#include <limits>
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

    Record(){
        id = -1;
        name = "";
        bio = "";
        manager_id = 0;
    }

    void print() {
        cout << "\tID: " << id << "\n";
        cout << "\tNAME: " << name << "\n";
        cout << "\tBIO: " << bio << "\n";
        cout << "\tMANAGER_ID: " << manager_id << "\n";
    }

    // Count the size of record
    int recordSize(){
        // First 8 is id, last 8 is manager id, 4 is delimiters
        int Size = 8 + name.size() + bio.size() + 8 + 4;
        return Size;
    }

    //Write the data into the record and store it in Variable-length records
    void writeRecord(const string& filename){
        fstream file(filename, ios::out | ios::binary);

        if(!file){
            cout << "Cannot open the file." << endl;
            return;
        }

        // Delimiters
        char delimiter = '~';

        // Write ID and manager id
        file.write(reinterpret_cast<char *>(&id), sizeof(id));
        file.write(&delimiter, sizeof(delimiter));
        file.write(reinterpret_cast<char *>(&manager_id), sizeof(manager_id));
        file.write(&delimiter, sizeof(delimiter));

        // Write Name's length and content
        int nameLength = name.length();
        file.write(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
        file.write(name.c_str(), nameLength);
        file.write(&delimiter, sizeof(delimiter));

        // Write bio's length and content
        int bioLength = bio.length();
        file.write(reinterpret_cast<char *>(&bioLength), sizeof(bioLength));
        file.write(bio.c_str(), bioLength);
        file.write(&delimiter, sizeof(delimiter));

        file.close();
    }
};

class Block {
    private:
        const int BLOCK_SIZE = 4096;
        // Delimiter
        const char DELIMITER = '~';


    public:
        // The records in the block
        vector<Record> records;  
        // The size of the block
        int blockSize;
        // The index of the overflow block in the block directory
        int ofIdx;
        // Number of records in the block
        int numRecords;
        // The offset of block in the index file
        int blockIdx;

        Block() : blockSize(0), blockIdx(-1), ofIdx(-1), numRecords(0) {}

        Block(int realIdx) : blockSize(0), blockIdx(realIdx), ofIdx(-1), numRecords(0) {}

        // Read in a record from the index file
        void readRecord(ifstream& file) {
            int64_t id, managerId;
            string name, bio;

            // Read in ID and manager_id
            file.read(reinterpret_cast<char*>(&id), sizeof(id));
            file.read(reinterpret_cast<char*>(&managerId), sizeof(managerId));

            // Ignore the delimiter
            file.ignore(numeric_limits<streamsize>::max(), DELIMITER);

            // Read name and bio
            getline(file, name, DELIMITER);
            getline(file, bio, DELIMITER);

            // Convert 8-byte interger to 4-byte integer
            int idInt = static_cast<int>(id);
            int managerIdInt = static_cast<int>(managerId);

            // Output the info 
            cout << "ID: " << idInt << ", Manager ID: " << managerIdInt << ", Name: " << name << ", Bio: " << bio << endl;

            records.push_back(Record({to_string(idInt), name, bio, to_string(managerIdInt)}));
        }    

        void readBlock(const string& filename){
            ifstream file(filename, ios::binary);

            if (!file) {
                cout << "Cannot open the file." << endl;
                return;
            }

            // Move to the beginning of the data block
            file.seekg(blockIdx * BLOCK_SIZE, ios::beg);

            // Read overflow pointer index and record number
            file.read(reinterpret_cast<char*>(&ofIdx), sizeof(ofIdx));
            file.read(reinterpret_cast<char*>(&numRecords), sizeof(numRecords));

            // Output the information read
            cout << "Overflow Index: " << ofIdx << ", Number of Records: " << numRecords << endl;

            // Loop to read each record
            for (int i = 0; i < numRecords; ++i) {
                readRecord(file);
                // Update blockSize
                blockSize += sizeof(int) * 2 + records.back().recordSize();
            }

            // Output the updated blockSize and other information
            cout << "Updated Block Size: " << blockSize << ", Block Index: " << blockIdx << endl;
        }   

        void writeBlock(const string& filename, int offset) {
            fstream file(filename, ios::out | ios::binary);

            if (!file) {
                cout << "Cannot open the file." << endl;
                return;
            }

            // Move to the specified offset in the file
            file.seekp(offset, ios::beg);

            // Write overflow pointer index and record number
            file.write(reinterpret_cast<char*>(&ofIdx), sizeof(ofIdx));
            file.write(reinterpret_cast<char*>(&numRecords), sizeof(numRecords));

            // Write each record
            for (Record record : records) {
                record.writeRecord(file);
            }

            file.close();
        }
};

class LinearHashIndex {

    private:
        const int BLOCK_SIZE = 4096;
        // 70% of the page capacity, the value should be equal to 2867
        const int capacitySize = static_cast<int>(BLOCK_SIZE * 0.7); 

        vector<int> blockDirectory; // Map the least-significant-bits of h(id) to a bucket location in EmployeeIndex (e.g., the jth bucket)
                                    // can scan to correct bucket using j*BLOCK_SIZE as offset (using seek function)
                                    // can initialize to a size of 256 (assume that we will never have more than 256 regular (i.e., non-overflow) buckets)
        int n;  // The number of indexes in blockDirectory currently being used
        int i;	// The number of least-significant-bits of h(id) to check. Will need to increase i once n > 2^i
        int numRecords;    // Records currently in index. Used to test whether to increase n
        int nextFreeBlock; // Next place to write a bucket. Should increment it by BLOCK_SIZE whenever a bucket is written to EmployeeIndex
        string fName;      // Name of index file


        // Insert new record into index
        void insertRecord(Record record) {
            // No records written to index yet
            if (numRecords == 0) {
                // Initialize index with first blocks (start with 4)
                for(int j = 0; j < n; j++){
                    blockDirectory.push_back(nextFreeBlock);
                    nextFreeBlock += BLOCK_SIZE;
                }
            }

            // Add record to the index in the correct block, creating a overflow block if necessary
            int bucketIdx = std::hash<int>(record.id) & ((1 << i) - 1);
            int bucketOffset = blockDirectory[bucketIdx];

            // Load bucket, handle overflow, splitting, etc.
            Block block(bucketOffset);
            block.readBlock(fName);

            if (block.blockSize >= capacitySize) {
                // Split bucket
                int oldBucketIdx = std::hash<int>(record.id) & ((1 << i) - 1);
                int newBucketIdx = oldBucketIdx + (1 << i);

                // Create a new block for the split records
                Block newBlock(nextFreeBlock); 
                blockDirectory.push_back(nextFreeBlock);
                nextFreeBlock += BLOCK_SIZE; 

                // Redistribute records between old and new blocks
                for (int k = 0; k < block.records.size(); k++) {
                    Record r = block.records[k];
                    int targetBucketIdx = std::hash<int>(r.id) & ((1 << i) - 1);
                    if (targetBucketIdx == newBucketIdx) {
                        newBlock.records.push_back(r);
                        block.records.erase(block.records.begin() + k);
                        k--; // Decrement index as we removed an element
                    }
                }

                // Add the new record to the appropriate bucket
                int targetBucketIdx = std::hash<int>(record.id) & ((1 << i) - 1);
                if (targetBucketIdx == newBucketIdx) {
                    newBlock.records.push_back(record);
                } else {
                    block.records.push_back(record);
                }

                // Write updated blocks back to the index file
                block.writeBlock(fName, bucketOffset);
                newBlock.writeBlock(fName, newBucketIdx * BLOCK_SIZE);
            } else {
                // Add record to block
                block.records.push_back(record);
                block.writeBlock(fName, bucketOffset);
            }

            // Take neccessary steps if capacity is reached:
            // increase n; increase i (if necessary); place records in the new bucket that may have been originally misplaced due to a bit flip
            if (numRecords > capacitySize * n) {
                // Increase n
                n *= 2;
                // Increase i if necessary
                if (i < 8) {
                    i++;
                }
                // Rehash and place records in the new buckets
                for (int l = 0; l < numRecords; l++) {
                    // Get record
                    Record record = block.records[l];
                    // Calculate new bucket index
                    int newBucketIdx = std::hash<int>(record.id) & ((1 << l) - 1);
                    // Get new bucket offset
                    int newBucketOffset = blockDirectory[newBucketIdx];
                    // Load new bucket
                    Block newBlock(newBucketOffset);
                    // Add record to new bucket
                    newBlock.records.push_back(record);
                    // Write new bucket
                    newBlock.writeBlock(fName, newBucketOffset);
                }
            }

            // Update numRecords
            numRecords++;
        }

    public:
        LinearHashIndex(string indexFileName) {
            n = 4; // Start with 4 buckets in index
            i = 2; // Need 2 bits to address 4 buckets
            numRecords = 0;
            nextFreeBlock = 0;
            fName = indexFileName;

            // Create your EmployeeIndex file and write out the initial 4 buckets
            // make sure to account for the created buckets by incrementing nextFreeBlock appropriately
            fstream indexFile(fName, ios::out | ios::binary);
            for (int i = 0; i < n; i++) {
                blockDirectory.push_back(nextFreeBlock);
                nextFreeBlock += BLOCK_SIZE;
                // Write empty block
                Block emptyBlock;
                emptyBlock.writeBlock(fName, nextFreeBlock);
            }
        }

        // Read csv file and add records to the index
        void Block::createFromFile(string csvFName) {
            ifstream infile(csvFName);
            string line;

            while (getline(infile, line)) {
                // Create a string stream from the line
                std::stringstream ss(line);

                // Declare variables to store the fields
                string id, name, bio, managerId;

                // Use comma as delimiter and extract fields
                getline(ss, id, ',');
                getline(ss, name, ',');
                getline(ss, bio, ',');
                getline(ss, managerId, ',');

                // Create a record from the extracted fields
                Record record({id, name, bio, managerId});

                // Add the record to the block
                records.push_back(record);
            }

            // Update the number of records
            numRecords = records.size();
        }

        // Given an ID, find the relevant record and print it
        Record findRecordById(int id) {
            int bucketIdx = std::hash<int>(id) & ((1 << i) - 1);
            int bucketOffset = blockDirectory[bucketIdx];

            // Load bucket
            Block block(bucketOffset);
            block.readBlock(fName);

            // Search for record in block and overflow chain
            for (Record record : block.records) {
                if (record.id == id) {
                    return record;
                }
            }

            // Record not found
            return Record();           
        }
};
