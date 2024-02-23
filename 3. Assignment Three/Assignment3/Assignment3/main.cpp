/*
Skeleton code for linear hash indexing
*/

#include <ios>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include "classes.h"
using namespace std;

int main() {
    // Initialize the index with the name of the index file
    LinearHashIndex myIndex("EmployeeIndex");

    // Create the index from a CSV file containing records
    std::string csvFileName = "Employee.csv";
    myIndex.createFromFile(csvFileName);

    std::string input;

    while (true) {
        std::cout << "Enter an ID to search for or type 'quit' to exit: ";
        std::getline(std::cin, input); // 從標準輸入讀取一行

        if (input == "quit") { // 如果用戶輸入"quit"，則退出循環
            break;
        }

        int searchId;
        try {
            searchId = std::stoi(input); // 嘗試將輸入轉換為整數
        } catch (const std::invalid_argument& e) {
            std::cout << "Invalid input. Please enter a valid ID or 'quit'." << std::endl;
            continue; // 如果轉換失敗，提示用戶並繼續循環
        }

        std::cout << "Searching for record with ID: " << searchId << std::endl;
        Record foundRecord = myIndex.findRecordById(searchId);
        // 假設你的findRecordById函數已經正確實現，這裡將會處理找到的紀錄
        // 例如，你可以添加一個方法在Record類中來打印紀錄的詳細信息
    }

    return 0;
}