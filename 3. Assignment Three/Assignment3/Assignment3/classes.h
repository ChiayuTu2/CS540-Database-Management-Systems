#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
#include <queue>
using namespace std;

class Record {
public:
    long long int id, manager_id;
    string bio, name;

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

struct BlockIndex {
    int numbucket; // Bucket number
    int blockpointer; // Pointer to the block's location in the file
    int indexpointer; // Pointer to the index's location in the file
    char resvd[2]; // Reserved space for future use or alignment
    bool fullbit; // Bit indicating if the block is full
    bool oflpagebit; // Overflow page bit indicating if the block has an overflow page
};

class LinearHashIndex {

private:
    const int BLOCK_SIZE = 4096;
    const int BucketsRecordNums = 6;// Number of records per bucket

    vector<int> blockDirectory; // Map the least-significant-bits of h(id) to a bucket location in EmployeeIndex (e.g., the jth bucket)
                                // can scan to correct bucket using j*BLOCK_SIZE as offset (using seek function)
								// can initialize to a size of 256 (assume that we will never have more than 256 regular (i.e., non-overflow) buckets)
    int n;  // The number of indexes in blockDirectory currently being used
    int i;	// The number of least-significant-bits of h(id) to check. Will need to increase i once n > 2^i
    int numRecords;    // Records currently in index. Used to test whether to increase n
    int nextFreeBlock; // Next place to write a bucket. Should increment it by BLOCK_SIZE whenever a bucket is written to EmployeeIndex
    int numIndex; // Number of indexes
    int nextptr; // Pointer for navigating through the index
    int IndexPagePtr = 0; // Pointer to the index page
    int FreeSpacePos = BLOCK_SIZE - sizeof(int); // Position indicating the start of free space in a block
    int NumEntriesPos = BLOCK_SIZE - (sizeof(int) + sizeof(int)); // Position indicating the number of entries in a block
    string fName;      // File name of the index

    vector<char> page; // Vector to hold page data

    // Compute the bucket for a given ID
    int ComputeBucket (int id) {
        int hashid = id % 216;
        int idbucket = hashid % static_cast<int>(pow(2,i));
        return idbucket;
    }

    // Flip the bit of the bucket number to manage expansion
    int FlipBit(int numbucket) {
        numbucket = numbucket % static_cast<int>(pow(2,i-1));
        return numbucket;
    }

    // Write data to file at a specific location
    void WriteToFile(int pagelocation, vector<char> _page) {
        // Write file function
        ofstream outfile;
        outfile.open(fName,ios::in|ios::out|ios::binary);
        outfile.seekp(pagelocation,ios::beg);
        outfile.write(_page.data(),BLOCK_SIZE);
        outfile.close();
    }

    // Read data from file at a specific location
    void ReadFromFile(int pagelocation, vector<char> &_page) {
        // Read file function
        ifstream infile;
        infile.open(fName,ios::in|ios::binary);
        infile.seekg(pagelocation,ios::beg);
        infile.read(_page.data(),BLOCK_SIZE);
        infile.close();
    }

    // Insert a block index into the index page and add a blank page
    BlockIndex InsertBlock(int numbucket, int indexpointer, bool overflow) {
        BlockIndex newindex;

        // Initialize the new index with zeros
        memset(&newindex,0,sizeof(BlockIndex));
        if (overflow) {
            // Set the overflow page bit if necessary
            newindex.oflpagebit = true;
        }

        // Set the bucket number
        newindex.numbucket = numbucket;
        // Set the block pointer to the next free block
        newindex.blockpointer = nextFreeBlock;
        // Set the index pointer
        newindex.indexpointer = indexpointer;

        // Copy the new index into the index page
        memcpy(&page[indexpointer],&newindex,sizeof(BlockIndex));
        WriteToFile(IndexPagePtr,page);

        // Initialize a new page with zeros
        vector<char> init(BLOCK_SIZE);
        WriteToFile(newindex.blockpointer,init);
        init.clear();

        // Add n if it's another bucket, if n > 2^i, increase i
        // Update control variables for new buckets and overflow pages
        if (!overflow) {
            n+=1;
            if(n > static_cast<int>(pow(2,i))) {
                i+=1;
            }
        }
        // Move the pointer to the next free block
        nextFreeBlock += BLOCK_SIZE;
        return newindex;
    }

    // Write specific page
    void WritePage(Record record, BlockIndex& index) {

        // Initialize a vector to store the page data, using the predefined BLOCK_SIZE
        vector<char> bucketpage(BLOCK_SIZE);

        // Read the existing data from the file into the bucketpage vector based on the blockpointer of the index
        ReadFromFile(index.blockpointer, bucketpage);

        // Determine the next available space in the page by reading the integer at FreeSpacePos
        int nextSpace = *reinterpret_cast<int *>(&bucketpage[FreeSpacePos]);
        // Record the start position of the new record for later reference
        int start_record_ptr = nextSpace;
        // Read the current number of entries from the page
        int NumEntries = *reinterpret_cast<int *>(&bucketpage[NumEntriesPos]);
        
        // Copy the record's ID into the page and update the nextSpace pointer
        memcpy(&bucketpage[nextSpace], &record.id, sizeof(record.id));
        nextSpace += sizeof(record.id);
        // Insert a comma as a delimiter
        bucketpage[nextSpace++] = ',';

        // Copy the record's name into the page and update the nextSpace pointer
        memcpy(&bucketpage[nextSpace], record.name.data(), record.name.size());
        nextSpace += record.name.size();
        bucketpage[nextSpace++] = ',';

        // Copy the record's biography into the page and update the nextSpace pointer
        memcpy(&bucketpage[nextSpace], record.bio.data(), record.bio.size());
        nextSpace += record.bio.size();
        bucketpage[nextSpace++] = ',';

        // Copy the record's manager ID into the page and update the nextSpace pointer
        memcpy(&bucketpage[nextSpace], &record.manager_id, sizeof(record.manager_id));
        nextSpace += sizeof(record.manager_id);

        // Increment the number of entries as we have added a new record
        NumEntries++;

        // If the number of entries equals the predefined bucket record limit, set the fullbit to true
        if (NumEntries == BucketsRecordNums) {
            index.fullbit = 1;
        }

        // Calculate the size of the record by subtracting the start position from the current pointer
        int recordsize = nextSpace - start_record_ptr;
        // Update the FreeSpacePtr with the current position
        int FreeSpacePtr = nextSpace;
        // Write the new free space pointer back to the page
        nextSpace = FreeSpacePos;
        memcpy(&bucketpage[nextSpace], &FreeSpacePtr, sizeof(FreeSpacePtr));

        // Write the new number of entries back to the page
        nextSpace = NumEntriesPos;
        memcpy(&bucketpage[nextSpace], &NumEntries, sizeof(NumEntries));

        // For each entry, store the start position and size at the end of the page
        nextSpace = NumEntriesPos - NumEntries * sizeof(int) * 2;
        memcpy(&bucketpage[nextSpace], &start_record_ptr, sizeof(start_record_ptr));

        // After the start position, store the size of the record
        nextSpace += sizeof(int);
        memcpy(&bucketpage[nextSpace], &recordsize, sizeof(recordsize));
        
        // Write the updated page data back to the file
        WriteToFile(index.blockpointer, bucketpage);
        // Clear the bucketpage vector to free up memory
        bucketpage.clear();
    }
    
    // Check if the capacity of hash bucket exceed 70%, if yes, add another page (increase n and i)
    bool CheckBucket() {
        float capacity = numRecords / (n * BucketsRecordNums);
        if (capacity > 0.7) {
            return true;
        }
        return false;
    }

    // Function to rehash records in a bucket after an expansion
    void Rehash(int numbucket) {
        // Initialize variables for the rehash process
        BlockIndex index;
        // Calculate the target bucket for rehashing based on the current level 'i'
        int checkrehashbucket = numbucket % static_cast<int>(pow(2,i-1));
        // Index pointer to navigate through the index page
        int indexpointer = 0;
        // Temporary storage for block indexes that need rehashing
        vector<BlockIndex> tmp;
        // Default record values for initializing temporary records
        vector<string> deflt = {"0", "", "", "0"};
        // Temporary record for processing
        Record tmpRecord = deflt;
        // Destination vector for temporarily holding records to be rehashed
        vector<Record> tempRecordDst;
        // Vector to hold page data during search
        vector<char> searchPage(BLOCK_SIZE);
        // Variables for navigating within a page
        int nextSpace = 0;
        int searchslotptr = 0;


        // Search through the index to find blocks corresponding to the rehash bucket
        for (int k = 0; k < numIndex; k++) {
            // Copy the index data into a local variable for processing
            memcpy(&index, &page[indexpointer], sizeof(BlockIndex));
            // Calculate the flip bit to identify the correct bucket after expansion
            int flp = FlipBit(numbucket);
            // If the current index corresponds to the target bucket, add it to the temporary list
            if (index.numbucket == flp) {
                tmp.push_back(index);
            }
            // Move the pointer to the next block index in the page
            indexpointer += sizeof(BlockIndex);
        }
        // Process each block index in the temporary list for rehashing
        for (auto index : tmp) {
            // Read the page data from the file into a vector
            ReadFromFile(index.blockpointer, searchPage);
            // Read the number of entries in the current page
            int NumEntries = *reinterpret_cast<int*>(&searchPage[NumEntriesPos]);

            // Skip empty pages
            if (NumEntries == 0) {
                continue;
            }

            // Extract and process each record in the page
            for (int k = 1; k <= NumEntries; k++) {
                // Calculate the pointer to the start of the record's data
                searchslotptr = NumEntriesPos - sizeof(int) * 2 * k;
                nextSpace = *reinterpret_cast<int*>(&searchPage[searchslotptr]);
                // Extract the record's fields from the page data
                tmpRecord.id = *reinterpret_cast<int*>(&searchPage[nextSpace]);
                nextSpace += sizeof(tmpRecord.id) + 1; // Skip past the ID and delimiter
                // Extract the name field
                string name;
                while (searchPage[nextSpace] != ',') {
                    name += searchPage[nextSpace++];
                }
                tmpRecord.name = name;
                nextSpace++; // Skip past the delimiter
                // Extract the biography field
                string bio;
                while (searchPage[nextSpace] != ',') {
                    bio += searchPage[nextSpace++];
                }
                tmpRecord.bio = bio;
                nextSpace++; // Skip past the delimiter
                // Extract the manager ID field
                tmpRecord.manager_id = *reinterpret_cast<int*>(&searchPage[nextSpace]);

                // Add the extracted record to the destination vector for reinsertion
                tempRecordDst.push_back(tmpRecord);
            }
            // Update the total record count and clear the processed page
            numRecords -= NumEntries;
            fill(searchPage.begin(), searchPage.end(), 0);
            WriteToFile(index.blockpointer, searchPage);
            // Reset the full bit indicator for the block index and update the index page
            index.fullbit = 0;
            memcpy(&page[index.indexpointer], &index, sizeof(BlockIndex));
            WriteToFile(IndexPagePtr, page);
        }
        // Clear the search page vector to free up memory
        searchPage.clear();
        // Reinsert the records into the index, which may now be placed in different buckets
        if (!tempRecordDst.empty()) {
            for (auto& record : tempRecordDst) {
                insertRecord(record);
            }
        }
    }

    // Function to insert a new record into the index
    void insertRecord(Record record) {

        // Flag to indicate if the target bucket has been found
        bool bucketfound = false;

        // Initialize a vector to hold page data, with size equal to BLOCK_SIZE
        vector<char> init(BLOCK_SIZE);
        // Assign the initialized vector to the page variable
        page = init;

        // Declare a BlockIndex structure to hold index information
        BlockIndex Bindex;

        // Read the index page from the file into the page vector
        ReadFromFile(IndexPagePtr, page);

        // Compute the target bucket for the given record ID
        int idbucket = ComputeBucket(record.id);
        // If the computed bucket exceeds the current number of buckets, adjust it
        if (idbucket >= n) {
            idbucket = FlipBit(idbucket);
        }

        // Initialize pointer to iterate through the index page
        nextptr = 0;
        // Iterate through the index to find the correct bucket for insertion
        for (int k = 0; k < numIndex; k++) {
            // Copy the current block index information into Bindex
            memcpy(&Bindex, &page[nextptr], sizeof(BlockIndex));
            // Check if the current block is the target bucket and it is not full
            if (idbucket == Bindex.numbucket && !Bindex.fullbit) {
                // Write the record to the corresponding block
                WritePage(record, Bindex);
                // Update the block index information to reflect the insertion
                memcpy(&page[nextptr], &Bindex, sizeof(BlockIndex));
                // Write the updated index page back to the file
                WriteToFile(IndexPagePtr, page);
                // Mark the bucket as found and update the record count
                bucketfound = true;
                numRecords++;
                // Exit the loop since the target bucket has been found and updated
                break;
            }
            // Move to the next block index in the page
            nextptr += sizeof(BlockIndex);
        }
        // Calculate the end position of the index information in the page
        int indexend = numIndex * sizeof(BlockIndex);
        
        // If the target bucket was not found or is full, handle overflow
        if (!bucketfound) {
            // Insert a new block index for the overflow bucket
            BlockIndex newindex = InsertBlock(idbucket, indexend, true);
            // Write the record to the new overflow block
            WritePage(record, newindex);
            // Update the index page with the new block index information
            memcpy(&page[nextptr], &newindex, sizeof(BlockIndex));
            // Write the updated index page back to the file
            WriteToFile(IndexPagePtr, page);
            // Adjust pointers and counters to account for the new overflow block
            indexend += sizeof(BlockIndex);
            numIndex++;
            numRecords++;
        }
        // Check if the bucket capacity is exceeded and expansion is needed
        if (CheckBucket()) {
            // Insert a new block index for the new bucket as part of expansion
            BlockIndex newindex = InsertBlock(n, indexend, false);
            // Increase the index count to account for the new bucket
            numIndex++;
            // Perform rehashing to redistribute records as necessary
            Rehash(newindex.numbucket);
            // Adjust the index end position to account for the new bucket
            indexend += sizeof(BlockIndex);
        }
        // Clear the page vector to free up memory
        page.clear();
    }

public:
    // Constructor for initializing a LinearHashIndex object with a given index file name
    LinearHashIndex(string indexFileName) {
        // Initialize the number of buckets to 4
        n = 4;
        // Set the bit depth to 2, as 2 bits are needed to address 4 buckets
        i = 2;
        // Initialize the number of indexes and records to 0
        numIndex = 0;
        numRecords = 0;
        // Set the pointer to the next free block in the file to 0
        nextFreeBlock = 0;
        // Store the name of the index file
        fName = indexFileName;

        // Initialize a vector to hold the initial page data, filled with zeros
        vector<char> init(BLOCK_SIZE);

        // Open the index file in binary mode for writing
        ofstream finput(indexFileName, ios::out | ios::binary);
        // Check for errors in opening the file
        if (!finput) {
            cerr << "Error in creating file!!!\n";
            // Exit the constructor if file creation failed
            return;
        }
        // Assign the initialized page vector to the page variable
        page = init;
        // Initialize the pointer for navigating the index page
        nextptr = 0;

        // Declare a variable for the offset of bucket pages (unused in this snippet but likely intended for future use)
        int bucketpageoffst;
        // Initialize the counter for the index of the bucket being processed
        int indexofbucket = 0;
        // Declare a BlockIndex structure to hold the details of each bucket index
        BlockIndex Bindex;

        // Initialize the index page with details of the first 4 buckets
        for (int k = 1; k <= n; k++) {
            // Clear the Bindex structure for the new bucket
            memset(&Bindex, 0, sizeof(BlockIndex));
            // Set the block pointer for the bucket, using the block size as a multiplier for the offset
            Bindex.blockpointer = BLOCK_SIZE * k;
            // Assign the current bucket index
            Bindex.numbucket = indexofbucket;
            // Set the index pointer within the page
            Bindex.indexpointer = nextptr;
            // Copy the bucket index details into the index page
            memcpy(&page[nextptr], &Bindex, sizeof(BlockIndex));
            // Increment the bucket index and the number of indexes
            indexofbucket++;
            numIndex++;
            // Move the page pointer to the next index position
            nextptr += sizeof(BlockIndex);
        }
        // Write the initialized index page to the file
        finput.write(page.data(), BLOCK_SIZE);
        // Update the pointer to the next free block in the file
        nextFreeBlock += BLOCK_SIZE;

        // Initialize each bucket page in the file with zeros
        for (int k = 1; k <= n; k++) {
            // Fill the page vector with zeros
            fill(page.begin(), page.end(), 0);
            // Increment the next free block pointer
            nextFreeBlock += BLOCK_SIZE;
            // Write the empty bucket page to the file
            finput.write(page.data(), BLOCK_SIZE);
        }
        // Close the file
        finput.close();
    }


    // Read csv file and add records to the index
    void createFromFile(string csvFName) {
        fstream finput;
        finput.open(csvFName,ios::in);
        if (!finput.is_open()) {
            cerr << "Couldn't read file: " << csvFName << "\n";
            return ;
        }
        string lineinfile;
        int dataset = 0;
        while (getline(finput,lineinfile)) {
            string sttrinfile;
            vector<string> attrvec;
            stringstream stringline(lineinfile);
            while(getline(stringline,sttrinfile,',')) {
                attrvec.push_back(sttrinfile);
            }
            Record somerecord(attrvec);
            insertRecord(somerecord);
            dataset++;

        }
    }

    // Function to find and return a record by its ID
    Record findRecordById(int id) {
        // Default values for a Record object in case the ID is not found
        vector<string> deflt = {"0", "", "", "0"};
        // Create a Record object with default values
        Record foundrecord = deflt;
        // Declare a BlockIndex object to store index information
        BlockIndex index;
        // Initialize a vector to hold page data
        vector<char> init(BLOCK_SIZE);
        // Assign the initialized vector to the page variable
        page = init;
        // Clear the init vector (not necessary as it's not used further)
        init.clear();
        
        // Compute the target bucket for the given ID
        int bucket = ComputeBucket(id);
        // If the bucket number is greater than the current number of buckets, adjust it
        if (bucket >= n) {
            bucket = FlipBit(bucket);
        }
        // Read the index page from the file into the page vector
        ReadFromFile(IndexPagePtr, page);
        // Initialize the pointer for navigating through the index page
        nextptr = 0;
        // Iterate through the indexes to find the correct bucket
        for (int k = 0; k < numIndex; k++) {
            // Copy the current block index information into the index object
            memcpy(&index, &page[nextptr], sizeof(BlockIndex));
            // Check if the current index corresponds to the target bucket
            if (index.numbucket == bucket) {
                // Initialize a vector to hold the bucket's page data
                vector<char> searchPage(BLOCK_SIZE);
                // Read the bucket's page from the file into the searchPage vector
                ReadFromFile(index.blockpointer, searchPage);
                // Determine the number of entries in the bucket
                int NumEntries = *reinterpret_cast<int*>(&searchPage[NumEntriesPos]);
                // Iterate through the entries to find the matching ID
                for (int t = 1; t <= NumEntries; t++) {
                    // Calculate the pointer to the start of the current record's data
                    int searchslotptr = NumEntriesPos - sizeof(int) * 2 * t;
                    // Determine the start position of the record's data
                    int nextSpace = *reinterpret_cast<int*>(&searchPage[searchslotptr]);
                    // Read the ID from the record's data
                    long long int foundid = *reinterpret_cast<long long int*>(&searchPage[nextSpace]);
                    // Check if the read ID matches the target ID
                    if (id == (int)foundid) {
                        // Update the found record's fields with the data from the matching record
                        foundrecord.id = foundid;
                        nextSpace += sizeof(foundrecord.id) + 1;
                        // Extract the name field
                        string name;
                        while (searchPage[nextSpace] != ',') {
                            name += searchPage[nextSpace++];
                        }
                        foundrecord.name = name;
                        nextSpace++;
                        // Extract the bio field
                        string bio;
                        while (searchPage[nextSpace] != ',') {
                            bio += searchPage[nextSpace++];
                        }
                        foundrecord.bio = bio;
                        nextSpace++;
                        // Extract the manager ID field
                        foundrecord.manager_id = *reinterpret_cast<long long int*>(&searchPage[nextSpace]);
                        // Print a message indicating that the ID was found and display the record
                        cout << "---------- ID found -----------" << endl;
                        foundrecord.print();
                        // Return the found record
                        return foundrecord;
                    }
                }
            }
            // Move to the next block index in the page
            nextptr += sizeof(BlockIndex);
        }
        // If the ID was not found, print a message and return the record with default values
        cout << "---------- ID Not Found ----------" << endl;
        foundrecord.print();
        return foundrecord;
    }

};