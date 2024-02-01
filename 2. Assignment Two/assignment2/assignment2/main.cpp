/*
Skeleton code for storage and buffer management
*/

#include <string>
#include <ios>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include "classes.h"
using namespace std;


int main(int argc, char* const argv[]) {

    // Create the EmployeeRelation file from Employee.csv
    StorageBufferManager manager("EmployeeRelation");
    manager.createFromFile("Employee.csv");
    
    // Loop to lookup IDs until user is ready to quit
    string input;
    while (true) {
        cout << "Enter an ID to search for, or 'quit' to exit: ";
        cin >> input;

        // Check if the user wants to quit
        if (input == "quit") {
            break;
        }

        try {
            // Convert input to integer
            int id = stoi(input);

            // Find the record by ID and print it
            Record record = manager.findRecordById(id);
            record.print();
        } catch (const std::invalid_argument& e) {
            cerr << "Invalid ID format. Please enter a valid integer." << endl;
        } catch (const std::runtime_error& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
