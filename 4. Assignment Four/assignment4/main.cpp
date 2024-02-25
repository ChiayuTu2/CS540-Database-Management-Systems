/* This is a skeleton code for External Memory Sorting, you can make modifications as long as you meet 
   all question requirements*/  

#include <bits/stdc++.h>
#include "record_class.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm> 

using namespace std;

//defines how many blocks are available in the Main Memory 
#define buffer_size 22

Records buffers[buffer_size]; //use this class object of size 22 as your main memory

/***You can change return type and arguments as you want.***/

//PASS 1
//Sorting the buffers in main_memory and storing the sorted records into a temporary file (Runs) 
void Sort_Buffer(Records buffers[], int size, ofstream &tempOut){
    //Remember: You can use only [AT MOST] 22 blocks for sorting the records / tuples and create the runs
    // Sorting the buffer by Emloyee id
    sort(buffers, buffers + size, [](const Records &a, const Records &b){
        return a.emp_record.eid < b.emp_record.eid;
    });
    
    // Write the sorted Records into the tempOut
    for(int i = 0; i < size; i++){
        tempOut << buffers[i].emp_record.eid << ","
                << buffers[i].emp_record.ename << ","
                << buffers[i].emp_record.age << ","
                << buffers[i].emp_record.salary << endl;
    }
}

//PASS 2
//Use main memory to Merge the Runs 
//which are already sorted in 'runs' of the relation Emp.csv 
void Merge_Runs(){

    //and store the Sorted results of your Buffer using PrintSorted() 
    return;
}

void PrintSorted(){

    //Store in EmpSorted.csv
    return;
}

int main() {

    //Open file streams to read and write
    //Opening out the Emp.csv relation that we want to Sort
    fstream empin;
    empin.open("Emp.csv", ios::in);
   
    //Creating the EmpSorted.csv file where we will store our sorted results
    fstream SortOut;
    SortOut.open("EmpSorted.csv", ios::out | ios::app);

    // Temporart file to store sorted records from each chunks
    fstream tempOut;
    tempOut.open("temp.csv", ios::out | ios::app);

    //1. Create runs for Emp which are sorted using Sort_Buffer()
    int index = 0;
    while(!empin.eof()){
        index = 0;   // Reset
        while(index < buffer_size && !empin.eof()){
            buffers[index] = Grab_Emp_Record(empin);
            if(buffers[index].no_values == -1){
                // no records anymore
                break;
            }
            index++;
        }

        // Write the sorted data in the tempOut
        if(index > 0){
            Sort_Buffer(buffers, index, tempOut);
        }
    }

    //2. Use Merge_Runs() to Sort the runs of Emp relations 


    //Please delete the temporary files (runs) after you've sorted the Emp.csv

    empin.close();
    tempOut.close();
    SortOut.close();

    return 0;
}
