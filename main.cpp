#include <iostream>
#include <string>
#include <vector>
#include <random>

namespace Constants {
    constexpr char comma = ',';
    constexpr char decimal = '.';
    constexpr char minus = '-';
}

class controlFlags {
public:
    bool errorInProgram{};
    bool programRunning{ true };

    char tryAgain() {
        std::cout << "Do you wish to order another list of values? \nType r to restart program or any other"
                     " key to exit:" << std::endl;
        std::string temp;
        std::getline(std::cin >> std::ws, temp);
        return static_cast<char>(std::toupper(static_cast<unsigned char>(temp[0])));
    }

    void programReset() {
        errorInProgram = false;
    }
};

class inputParser {
private:
    std::string input;

    void stringGetter() {
        std::cout << "\nPlease input list of values separated by commas. No need to use [] at the start or end"
                     "of array:" << std::endl;
        std::getline(std::cin >> std::ws, input);
    }

    bool isMinus(char a) {
        return a == Constants::minus;
    }

    bool isValidSymbol(char a) {
        return(a == Constants::comma || a == Constants::decimal || a == Constants::minus);
    }

    void removeDuplicatesAndCatchErrors(controlFlags &f) {
        auto length = input.size();
        for (int i = 0; i < length; i++) {
            if(isspace(input[i])) {
                input.erase(i, 1);
                --i;
                --length;
            }

            if (isdigit(input[i]))
                continue;
            else {
                if(isValidSymbol(input[i])){
                    if (i > 0 && input[i] == input[i-1]) {
                        input.erase(i-1, 1);
                        --length;
                        --i;
                    }
                }
                else {
                    f.errorInProgram = true;
                    std::cout << "ERROR: In valid symbol used. Restarting program..." << std::endl;
                }
            }
        }
    }

    void parseToVector(std::vector<float>& vec, controlFlags &f) {
        if (!f.errorInProgram) {
            bool decimalUsed = false;
            auto it = input.begin();
            std::string temp;
            while (it != input.end()) {
                if(*it == Constants::comma) {
                    if (!temp.empty()) {
                        auto tempFloat = std::stof(temp);
                        vec.push_back(tempFloat);
                        temp.clear();
                        decimalUsed = false;
                    }
                } else {
                    if (isdigit(*it) || isMinus(*it) )
                        temp.push_back(*it);
                    else if (!decimalUsed) {
                        temp.push_back(*it);
                        decimalUsed = true;
                    }
                }
                ++it;
            }
            if (!temp.empty()) {
                auto tempFloat = std::stof(temp);
                vec.push_back(tempFloat);
                temp.clear();
            }
        }
        else
            return;
    }

public:
    inputParser(std::vector<float>& vec, controlFlags &f) {
        stringGetter();
        removeDuplicatesAndCatchErrors(f);
        parseToVector(vec, f);
    }
};

class randomQuickSort {
private:
    static int randomNumber(int min, int max) {
        if (min <= max) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(min, max);
            return distrib(gen);
        }
        else {
            std::cout << "Random number generator not working. Returning -1" << std::endl;
            return -1;
        }
    }

    static int partition(std::vector<float> &vec, int start, int end) {
        float pivot = vec[end];

        int partitionIndex = start;
        for (int i = start; i < end; i++) {
            if (vec[i] <= pivot) {
                std::swap(vec[i], vec[partitionIndex]);
                ++partitionIndex;
            }
        }
        std::swap(vec[end], vec[partitionIndex]);
        return partitionIndex;
    }

    static int randomisedPartition(std::vector<float> &vec, int start, int end) {
        int pivotIndex = randomNumber(start, end);
        std::swap(vec[pivotIndex], vec[end]);
        return partition(vec, start, end);
    }

    static void QuickSort (std::vector<float> &vec, int start, int end) {
        if (start < end) {
            int pIndex {randomisedPartition(vec, start, end)};

            QuickSort(vec,start, pIndex-1);
            QuickSort(vec, pIndex+1, end);
        }
    }

public:
    static void Sort(std::vector<float> &vec) {
        int start = 0;
        int end = static_cast<int>(vec.size() - 1);
        QuickSort(vec, start, end);
    }
};

class printer {
public:
    static void print(std::vector<float> &vec) {
                int count {0};
        std::cout << "[";
        std::cout << vec[count++];
        auto size = vec.size();
        while (count < size) {
            std::cout << ", " << vec[count];
            ++count;
        }
        std::cout << "]" << std::endl;
    }
};

int main() {
    std::cout << "Read README for information of limitations on program and important information." << std::endl;

    controlFlags programControl;
    std::vector<float> container;

    while(programControl.programRunning) {
        inputParser input(container, programControl);

        if (!programControl.errorInProgram) {
            randomQuickSort::Sort(container);
            printer::print(container);
        }

        // Does the user want to run another operation?
        if (programControl.tryAgain() == 'R') {
            container.clear();
            programControl.programReset();
        }
        else
            programControl.programRunning = false;

    }

    return 0;
}
