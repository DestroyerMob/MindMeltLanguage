#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

int bytes[65536];
unsigned int pointer = 0;
int value[65536];

enum {
 // Place traps here
};

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

void setStoredValue(std::string program, int& index, int newValue) {

    int storedIndex = 0;

    if (isdigit(program.at(index))) {
        storedIndex = readNum(program, index);
        value[storedIndex] = newValue;
    }

}

int getStoredValue(std::string program, int& index) {

    int storedValue = 0;

    if (isdigit(program.at(index))) {
        int temp = readNum(program, index);
        storedValue = value[temp];
    }

    return storedValue;

}

void runTrap(int trapnum) {

}

void runProgram(std::string program) {
    for (int i = 0; i < program.size(); i++) {
        if (program.at(i) == '>') {
            if (i + 1 < program.size()) {
                if (isdigit(program.at(i + 1))) {
                    i++;
                    pointer += readNum(program, i);
                } else if (program.at(i + 1) == '$') {
                    i += 2;
                    pointer += getStoredValue(program, i);
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
                    i += 2;
                    pointer -= getStoredValue(program, i);
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
                    i += 2;
                    bytes[pointer] += getStoredValue(program, i);
                }  else if (program.at(i + 1) == '\'') {
                    i += 2;
                    char ch = readChar(program, i);
                    bytes[pointer] += int(ch);
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
                    i += 2;
                    bytes[pointer] -= getStoredValue(program, i);
                }  else if (program.at(i + 1) == '\'') {
                    i += 2;
                    char ch = readChar(program, i);
                    bytes[pointer] -= int(ch);
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
                    i += 2;
                    bytes[pointer] *= getStoredValue(program, i);
                }  else if (program.at(i + 1) == '\'') {
                    i += 2;
                    char ch = readChar(program, i);
                    bytes[pointer] *= int(ch);
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
                    i += 2;
                    bytes[pointer] /= getStoredValue(program, i);
                }  else if (program.at(i + 1) == '\'') {
                    i += 2;
                    char ch = readChar(program, i);
                    bytes[pointer] /= int(ch);
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
                    i += 2;
                    bytes[pointer] %= getStoredValue(program, i);
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
                    i++;
                    int temp = getStoredValue(program, i);
                    if (bytes[pointer] == temp) {
                        findTag(program, i, temp);
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
                            i++;
                            int temp = getStoredValue(program, i);
                            findTag(program, i, temp);
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
                            i++;
                            int temp = getStoredValue(program, i);
                            findTag(program, i, temp);
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
                            i++;
                            int temp = getStoredValue(program, i);
                            findTag(program, i, temp);
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
                            i++;
                            int temp = getStoredValue(program, i);
                            findTag(program, i, temp);
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
                            i++;
                            int temp = getStoredValue(program, i);
                            findTag(program, i, temp);
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
                            i++;
                            int temp = getStoredValue(program, i);
                            findTag(program, i, temp);
                        }
                    }   
                } else if (str == "nzp") {
                    i++;
                    i++;
                    if (isdigit(program.at(i))) {
                        int tagNum = readNum(program, i);
                        findTag(program, i, tagNum);
                    } else if (program.at(i) == '$') {
                        i++;
                        int temp = getStoredValue(program, i);
                        findTag(program, i, temp);
                    }   
                }
            }
        } else if (program.at(i) == '$') {
            i++;
            setStoredValue(program, i, bytes[pointer]);
        } else if (program.at(i) == '=') {
            i++;
            if (isdigit(program.at(i))) {
                bytes[pointer] = readNum(program, i);
            } else if (program.at(i) == '$') {
                i++;
                bytes[pointer] = getStoredValue(program, i);
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
            } else if (program.at(i) == '^') {
                i++;
                if (program.at(i) == '$') {
                    i++;
                    int temp = getStoredValue(program, i);
                    for (int j = 0; j < temp; j++) {
                        std::cout << char(bytes[pointer]);
                        pointer++;
                    }
                }
            }
        } else if (program.at(i) == '@') {
            i++;
            if (isdigit(program.at(i))) {
                pointer = readNum(program, i);
            } else if (program.at(i) == '$') {
                i++;
                pointer = getStoredValue(program, i);
            }
        } else if (program.at(i) == '&') {
            std::string line;
            getline(std::cin, line);
            int j = 0;
            for (int k = 0; k < line.size(); k++) {
                j++;
                bytes[pointer] = line.at(k);
                pointer++;
            }
            bytes[pointer] = j;
        } else if (program.at(i) == '#') {
            std::string line;
            getline(std::cin, line);
            bytes[pointer] = std::atoi(line.c_str());
        } else if (program.at(i) == '~') {
            i++;
            if (isdigit(program.at(i))) {
                int tagNum = readNum(program, i);
                int tempIndex = i;
                findTag(program, i, tagNum);
                bytes[pointer] = tempIndex;
            } else if (program.at(i) == '$') {
                i++;
                int temp = getStoredValue(program, i);
                int tempIndex = i;
                findTag(program, i, temp);
                bytes[pointer] = tempIndex;
            }
        } else if (program.at(i) == '`') {
            i++;
            if (isdigit(program.at(i))) {
                i = readNum(program, i);
            } else if (program.at(i) == '$') {
                i++;
                i = getStoredValue(program, i);
            }
        } else if (program.at(i) == '|') {
            i++;
            if (isdigit(program.at(i))) {
                int trapnum = readNum(program, i);
                runTrap(trapnum);
            } else if (program.at(i) == '$') {
                i++;
                int trapnum = getStoredValue(program, i);
                runTrap(trapnum);
            }
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

    runProgram(program.str());
    std::cout << std::endl;

    return 0;
}