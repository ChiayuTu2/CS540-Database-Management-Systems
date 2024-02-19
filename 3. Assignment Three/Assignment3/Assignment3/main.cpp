/*
Name: Chiayu Tu
Email: tuchi@oregonstate.edu
ONID: tuchi
ssh tuchi@hadoop-master.engr.oregonstate.edu
g++ -std=c++11 main.cpp -o main.out
main.out
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

    // Create the index
    LinearHashIndex emp_index("EmployeeIndex");

    // Check if file exists
    ifstream infile("Employee.csv");
    if (!infile.is_open()) {
        cout << "Error opening file Employee.csv" << endl;
        return 1;
    }

    // Create the index from file
    emp_index.createFromFile("Employee.csv");

    // Loop to lookup IDs until user quits
    while (true) {
        int id;
        cout << "Enter ID (-1 to quit): ";
        cin >> id;

        if (id == -1) {
            break;
        }

        // Find the record
        Record record = emp_index.findRecordById(id);

        // Print record information
        if (record.id != -1) {
            cout << "Record found:" << endl;
            cout << "ID: " << record.id << endl;
            cout << "Name: " << record.name << endl;
        } else {
            cout << "Record not found." << endl;
        }
    }

    return 0;
}
/*
int main(int argc, char* const argv[]) {

    // Create the index
    LinearHashIndex emp_index("EmployeeIndex");
    emp_index.createFromFile("Employee.csv");
    
    // Loop to lookup IDs until user is ready to quit
    

    return 0;
}
*/

