#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

int bytes[65536];
unsigned int pointer = 0;

int readNum(std::string program, int& index) {
    std::stringstream numStr;

    while (isdigit(program.at(index))) {
        numStr << program.at(index);
        if (index < program.size() - 1) {
            index++;
        } else {
            break;
        }
    }

    index--;

    return std::stoi(numStr.str());
}

char readChar(std::string program, int& index) {
    std::stringstream sStr;

    while (program.at(index) != '\'') {
        sStr << program.at(index);
        if (index < program.size() - 1) {
            index++;
        } else {
            break;
        }
    }

    index--;

    return sStr.str().at(0);
}

std::string readString(std::string program, int& index) {
    std::stringstream sStr;

    while (program.at(index) != '\"') {
        sStr << program.at(index);
        if (index < program.size() - 1) {
            index++;
        } else {
            break;
        }
    }

    index--;

    return sStr.str();
}

void findTag(std::string program, int& index, int tag) {
    int tempIndex = index;
    for (int i = 0; i < program.size(); i++) {
        if (program.at(i) == ':') {
            i++;
            index = i;
            int tagNum = readNum(program, index);
            if (tagNum == tag) {
                return;
            }
        }
    }
    index = tempIndex;
}

void runProgram(std::string program, int value) {
    for (int i = 0; i < program.size(); i++) {
        if (program.at(i) == '>') {
            if (i + 1 < program.size()) {
                if (isdigit(program.at(i + 1))) {
                    i++;
                    pointer += readNum(program, i);
                } else if (program.at(i + 1) == '$') {
                    i++;
                    pointer += value;
                } else {
                    pointer++;
                }
            } else {
                pointer++;
            }
        } else if (program.at(i) == '<') {
            if (i + 1 < program.size()) {
                if (isdigit(program.at(i + 1))) {
                    i++;
                    pointer -= readNum(program, i);
                } else if (program.at(i + 1) == '$') {
                    i++;
                    pointer -= value;
                } else {
                    pointer--;
                }
            } else {
                pointer--;
            }
        } else if (program.at(i) == '+') {
            if (i + 1 < program.size()) {
                if (isdigit(program.at(i + 1))) {
                    i++;
                    bytes[pointer] += readNum(program, i);
                } else if (program.at(i + 1) == '$') {
                    i++;
                    bytes[pointer] += value;
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
                } else if (program.at(i + 1) == '$') {
                    i++;
                    bytes[pointer] -= value;
                } else {
                    bytes[pointer]--;
                }
            } else {
                bytes[pointer]--;
            }
        } else if (program.at(i) == '*') {
            if (i + 1 < program.size()) {
                if (isdigit(program.at(i + 1))) {
                    i++;
                    bytes[pointer] *= readNum(program, i);
                } else if (program.at(i + 1) == '$') {
                    i++;
                    bytes[pointer] *= value;
                } else {
                    bytes[pointer] *= 2;
                }
            }
        } else if (program.at(i) == '/') {
            if (i + 1 < program.size()) {
                if (isdigit(program.at(i + 1))) {
                    i++;
                    bytes[pointer] /= readNum(program, i);
                } else if (program.at(i + 1) == '$') {
                    i++;
                    bytes[pointer] /= value;
                } else {
                    bytes[pointer] /= 2;
                }
            }
        } else if (program.at(i) == '%') {
            if (i + 1 < program.size()) {
                if (isdigit(program.at(i + 1))) {
                    i++;
                    bytes[pointer] %= readNum(program, i);
                } else if (program.at(i + 1) == '$') {
                    i++;
                    bytes[pointer] %= value;
                } else {
                    bytes[pointer] %= 2;
                }
            }
        } else if (program.at(i) == '.') {
            std::cout << char(bytes[pointer]);
        } else if (program.at(i) == '!') {
            std::cout << bytes[pointer];
        } else if (program.at(i) == ',') {
            std::string line;
            getline(std::cin, line);
            bytes[pointer] = int(line.at(0));
        } else if (program.at(i) == '?') {
            i++;
            if (program.at(i) == '=') {
                i++;
                if (isdigit(program.at(i))) {
                    int tagNum = readNum(program, i);
                    if (tagNum == bytes[pointer]) {
                        findTag(program, i, tagNum);
                    }
                } else if (program.at(i) == '$') {
                    if (bytes[pointer] == value) {
                        findTag(program, i, value);
                    }
                }
            } else if (program.at(i) == '\"') {
                i++;
                std::string str = readString(program, i);
                if (str == "n") {
                    i++;
                    i++;
                    if (isdigit(program.at(i))) {
                        int tagNum = readNum(program, i);
                        if (bytes[pointer] < 0) {
                            findTag(program, i, tagNum);
                        }
                    } else if (program.at(i) == '$') {
                        if (bytes[pointer] < 0) {
                            findTag(program, i, value);
                        }
                    }              
                } else if (str == "p") {
                    i++;
                    i++;
                    if (isdigit(program.at(i))) {
                        int tagNum = readNum(program, i);
                        if (bytes[pointer] > 0) {
                            findTag(program, i, tagNum);
                        }
                    } else if (program.at(i) == '$') {
                        if (bytes[pointer] > 0) {
                            findTag(program, i, value);
                        }
                    }              
                } else if (str == "z") {
                    i++;
                    i++;
                    if (isdigit(program.at(i))) {
                        int tagNum = readNum(program, i);
                        if (bytes[pointer] == 0) {
                            findTag(program, i, tagNum);
                        }
                    } else if (program.at(i) == '$') {
                        if (bytes[pointer] == 0) {
                            findTag(program, i, value);
                        }
                    }              
                } else if (str == "np") {
                    i++;
                    i++;
                    if (isdigit(program.at(i))) {
                        int tagNum = readNum(program, i);
                        if (bytes[pointer] != 0) {
                            findTag(program, i, tagNum);
                        }
                    } else if (program.at(i) == '$') {
                        if (bytes[pointer] != 0) {
                            findTag(program, i, value);
                        }
                    }   
                } else if (str == "nz") {
                    i++;
                    i++;
                    if (isdigit(program.at(i))) {
                        int tagNum = readNum(program, i);
                        if (bytes[pointer] <= 0) {
                            findTag(program, i, tagNum);
                        }
                    } else if (program.at(i) == '$') {
                        if (bytes[pointer] <= 0) {
                            findTag(program, i, value);
                        }
                    }   
                } else if (str == "zp") {
                    i++;
                    i++;
                    if (isdigit(program.at(i))) {
                        int tagNum = readNum(program, i);
                        if (bytes[pointer] >= 0) {
                            findTag(program, i, tagNum);
                        }
                    } else if (program.at(i) >= '$') {
                        if (bytes[pointer] <= 0) {
                            findTag(program, i, value);
                        }
                    }   
                } else if (str == "nzp") {
                    i++;
                    i++;
                    if (isdigit(program.at(i))) {
                        int tagNum = readNum(program, i);
                        findTag(program, i, tagNum);
                    } else if (program.at(i) == '$') {
                        findTag(program, i, value);
                    }   
                }
            }
        } else if (program.at(i) == '$') {
            value = bytes[pointer];
        } else if (program.at(i) == '=') {
            i++;
            if (isdigit(program.at(i))) {
                bytes[pointer] = readNum(program, i);
            } else if (program.at(i) == '$') {
                bytes[pointer] = value;
            } else if (program.at(i) == '\"') {
                i++;
                std::string str = readString(program, i);
                int j = 0;
                for (int k = 0; k < str.size(); k++) {
                    j++;
                    bytes[pointer] = str.at(k);
                    pointer++;
                }
                bytes[pointer] = j;
            } else if (program.at(i) == '\'') {
                i++;
                char ch = readChar(program, i);
                bytes[pointer] = int(ch);
            }
        } else if (program.at(i) == ';') {
            i++;
            if (isdigit(program.at(i))) {
                int k = readNum(program, i);
                for (int j = 0; j < k; j++) {
                    std::cout << char(bytes[pointer]);
                    pointer++;
                }
            } else if (program.at(i) == '$') {
                for (int j = 0; j < value; j++) {
                    std::cout << char(bytes[pointer]);
                    pointer++;
                }
            }
        } else if (program.at(i) == '@') {
            i++;
            if (isdigit(program.at(i))) {
                pointer = readNum(program, i);
            } else if (program.at(i) == '$') {
                pointer = value;
            }
        } else if (program.at(i) == '#') {
            std::string line;
            getline(std::cin, line);
            int j = 0;
            for (int k = 0; k < line.size(); k++) {
                j++;
                bytes[pointer] = line.at(k);
                pointer++;
            }
            bytes[pointer] = j;
        } else if (program.at(i) == '&') {
            std::string line;
            getline(std::cin, line);
            bytes[pointer] = std::atoi(line.c_str());
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

    getline(filein, temp);

    while (!filein.fail()) {
        program << temp;
        getline(filein, temp);
    }

    int value = 0;

    runProgram(program.str(), value);
    std::cout << std::endl;

    return 0;
}