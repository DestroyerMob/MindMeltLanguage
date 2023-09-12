#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

int bytes[65536];
unsigned int pointer = 0;

int readNum(std::string program, int& i) {
    std::stringstream numStr;

    while (isdigit(program.at(i))) {
        numStr << program.at(i);
        if (i < program.size() - 1) {
            i++;
        } else {
            break;
        }
    }

    i--;

    return std::stoi(numStr.str());
}

void runProgram(std::string program) {
    for (int i = 0; i < program.size(); i++) {
        if (program.at(i) == '>') {
            pointer++;
        } else if (program.at(i) == '<') {
            pointer--;
        } else if (program.at(i) == '+') {
            if (i + 1 < program.size()) {
                if (isdigit(program.at(i + 1))) {
                    i++;
                    bytes[pointer] += readNum(program, i);
                } else {
                    bytes[pointer]++;
                }
            } else {
                bytes[pointer]++;
            }
        } else if (program.at(i) == '-') {
            if (i + 1 < program.size()) {
                if (isdigit(program.at(i + 1))) {
                    i++;
                    bytes[pointer] -= readNum(program, i);
                } else {
                    bytes[pointer]--;
                }
            } else {
                bytes[pointer]--;
            }
        } else if (program.at(i) == '.') {
            std::cout << char(bytes[pointer]);
        }
    }
}

int main(int argc, char * argv[]) {

    if (argc != 2) {
        std::cout << "Incorrect usage" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    if (!filename.ends_with(".mm")) {
        std::cout << "Incorrect file type" << std::endl;
        return 1;
    }

    std::fstream filein(filename, std::ios::in);

    std::stringstream program;

    std::string temp;

    filein >> temp;

    while (!filein.fail()) {
        program << temp;
        filein >> temp;
    }
    runProgram(program.str());
    std::cout << std::endl;

    return 0;
}