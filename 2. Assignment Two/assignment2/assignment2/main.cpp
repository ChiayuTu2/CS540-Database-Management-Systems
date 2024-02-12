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
    StorageBufferManager manager("EmployeeRelation");

    try {
        manager.createFromFile("Employee.csv");
    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }

    int id;
    cout << "Enter employee ID to look up, or -1 to quit: ";
    while (cin >> id && id != -1) {
        try {
            Record found = manager.findRecordById(id);
            found.print();
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << endl;
        }
        cout << "Enter another employee ID to look up, or -1 to quit: ";
    }

    return 0;
}

