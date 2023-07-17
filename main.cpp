#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "rangeMinimumQuerie.h"
#include "eliasFano.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << "Invalid number of arguments. Use the following format:" << std::endl;
        std::cout << "ads_program [pd|rmq] input_file output_file" << std::endl;
        return 1;
    }
    std::string action = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];

    if (action == "pd") {
        // Open file and handling the file
        std::ifstream inFile(inputFile);
        if (!inFile) {
            std::cout << "Error opening file! Provide the full path to the file" << std::endl;
            exit(1);
        }

        uint64_t n;
        uint64_t number;
        std::vector<uint64_t> bitVector;

        // Reading the first line to determine how many numbers are in the input
        inFile >> n;

        // Create a bit vector with n entries
        for (int i = 0; i < n; i++) {
            inFile >> number;
            bitVector.emplace_back(number);
        }

        // Record time to create the datastructure
        auto startTime = std::chrono::high_resolution_clock::now();

        eliasFano pred(bitVector);

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        // Process the remaining lines as queries
        std::ofstream ofs;
        ofs.open(outputFile);
        if (!ofs) {
            std::cout << "Error opening output file! Provide the full path to the file" << std::endl;
            exit(1);
        }

        while (inFile >> number) {
            ofs << pred.pred(number) << std::endl;
        }

        std::cout << "RESULT algo=" << action << " name=Christoph_Hoffmeyer time=" << duration.count() << " space="
                  << pred.requiredSpace() << std::endl;

    } else if (action == "rmq") {
        // Open file in text mode
        std::ifstream inFile(inputFile);
        // Handle errors
        if (!inFile) {
            std::cout << "Error opening file! Provide the full path to the file" << std::endl;
            exit(1);
        }

        // Read the first line
        uint64_t n;
        uint64_t number;
        uint64_t number2;
        char komma;
        std::vector<uint64_t> bitVector;
        inFile >> n;

        // Create a bit vector with n entries
        for (int i = 0; i < n; i++) {
            inFile >> number;
            bitVector.emplace_back(number);
        }

        // Record time to create the datastructure
        auto startTime = std::chrono::high_resolution_clock::now();

        rangeMinimumQuerie range(bitVector);

        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

        // Process the remaining lines as queries
        std::ofstream ofs;
        ofs.open(outputFile);
        if (!ofs) {
            std::cout << "Error opening output file! Provide the full path to the file" << std::endl;
            exit(1);
        }

        while (inFile >> number && inFile >> komma && inFile >> number2) {
            ofs << range.rmq(number, number2) << std::endl;
        }

        std::cout << "RESULT algo=" << action << " name=Christoph_Hoffmeyer time=" << duration.count() << " space="
                  << range.requiredSpace() << std::endl;

    } else {
        std::cout
                << "Invalid action. Use 'pd' for Predecessor data structure or 'rmq' for Range-Minimum-Query data structure."
                << std::endl;
        return 1;
    }

    return 0;
}