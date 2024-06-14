#include "config.h"
#include "board.h"
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;

void printHelp() {
    cout << endl;
    cout << "Options:" << endl;
    cout << "--input <path> \t\t Specify path to the input file. Mandatory." << endl;
    cout << "--iterations <int> \t Specify the number of iterations. "  <<
        "Mandatory, must be a positive integer" << endl;
    cout << "--all \t\t\t Create one output file for each iteration. " << endl <<
        "\t\t\t If this option is not set, only the final iteration will generate " <<
         "an output." << endl;
    cout << "--no-limit \t\t Remove the limit of 256x256 on the board size." << endl <<
        " \t\t\t Please use with care as this could lead to memory overflow and  " <<
        "filesystem issues." << endl;
    cout << "--help \t\t\t Show this help message" << endl;
}

bool isOption(char* argstr) {
    return string(argstr).rfind("--", 0) == 0;
}

int main(int argc, char* argv[]){
    cout << "Welcome to LifeGame " <<LifeGame_VERSION_MAJOR << "."
        << LifeGame_VERSION_MINOR << endl;

    string inputFile = "";
    int iterations = -1;
    bool saveAll = false;
    bool noLimit = false;

    for (int i = 0; i < argc; i++) {

        if (isOption(argv[i])){

            if (string(argv[i]) == "--input") {
                if (i < argc - 1 && !isOption(argv[i+1])) {
                    inputFile = string(argv[i+1]);
                } else {
                    cout << "Empty input" << endl;
                    printHelp();
                    return -1;
                }
            }

            else if (string(argv[i]) == "--iterations") {
                if (i < argc - 1 && !isOption(argv[i+1])) {
                    iterations = std::stoi(argv[i+1]);
                } else {
                    cout << "Missing iterations number" << endl;
                    printHelp();
                    return -1;
                }
            }

            else if (string(argv[i]) == "--all") {
                saveAll = true;
            }

            else if (string(argv[i]) == "--no-limit") {
                noLimit = true;
            }

            else if (string(argv[i]) == "--help") {
                printHelp();
                return 0;
            }

            else{
                cout << "Unknown option: " << &argv[i][2] << endl;
                printHelp();
                return -1;
            }
        }
    }

    if (iterations < 0) {
        cout << "Bad argument: Please input a positive number of iterations" << endl;
        printHelp();
        return -1;
    }

    if (inputFile == "") {
        cout << "Bad argument: Please provide an input file" << endl;
        printHelp();
        return -1;
    }

    if (saveAll && iterations > 20) {
        cout << "You are about to generate " << iterations << " files, are you sure you\
 want to continue ? (y/n)" << endl;

        char c = getchar();
        if (c != 'y') {
            cout << "Aborting..." <<endl;
            return 0;
        }
    }

    try {
        life::Board board(inputFile, noLimit);
        board.iterate(iterations, saveAll);
    } catch (const std::exception& e) {
        cout << "An error occured: " << endl << e.what() << endl;
        return -1;
    }

    return 0;
}