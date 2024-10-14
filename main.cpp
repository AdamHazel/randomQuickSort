#include <iostream>
#include <string>
#include <vector>
#include <random>

namespace Constants {
    constexpr char comma = ',';
    constexpr char decimal = '.';
    constexpr char minus = '-';
}

/**
 * @class controlFlags controls the running of the programming (including recording if an error has occurred)
 */
class controlFlags {
public:
    bool errorInProgram{};
    bool programRunning{ true };

    /**
     * @brief used to ask user if they wish to run another round of sorting
     * @return char indicating answer
     */
    char tryAgain() {
        std::cout << "Do you wish to order another list of values? \nType r to restart program or any other"
                     " key to exit:" << std::endl;
        std::string temp;
        std::getline(std::cin >> std::ws, temp);
        return static_cast<char>(std::toupper(static_cast<unsigned char>(temp[0])));
    }

    void errorReset() {
        errorInProgram = false;
    }
};

/**
 * class inputToVector is used to take in input, check for errors, convert values to floats and place in vector
 */
class inputToVector {
private:
    std::string input;

    /**
     * @brief gets string from user
     */
    void stringGetter() {
        std::cout << "\nPlease input list of values separated by commas. No need to use [] at the start or end"
                     "of array:" << std::endl;
        std::getline(std::cin >> std::ws, input);
    }

    /**
     * @brief checks if character is a minus sign
     * @param a - a character
     * @return bool
     */
    bool isMinus(char a) {
        return a == Constants::minus;
    }

    /**
     * @brief checks if character is a valid symbol
     * @param a - a character
     * @return bool
     */
    bool isValidSymbol(char a) {
        return(a == Constants::comma || a == Constants::decimal || a == Constants::minus);
    }

    /**
     * @brief removes duplicates and flags basic errors
     * @param f - reference to an object of class controlFlags
     */
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

    /**
     * @brief converts values given in string input to floats and places them in the referenced vector
     * @param vec - reference to a vector of floats
     * @param f - reference to an object of class controlFlags
     */
    void parseToVector(std::vector<float>& vec, controlFlags &f) {
        if (!f.errorInProgram) {
            bool decimalUsed = false;
            auto it = input.begin();
            std::string temp;
            while (it != input.end()) {
                if(*it == Constants::comma) {
                    if (!temp.empty()) {
                        if (temp.size() == 1 && isMinus(temp[0])) {
                            f.errorInProgram = true;
                            std::cout << "ERROR: Minus sign with no value..." << std::endl;
                            return;
                        }
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
            if (!temp.empty()){
                if (temp.size() == 1 && isMinus(temp[0]))
                    return;
                auto tempFloat = std::stof(temp);
                vec.push_back(tempFloat);
                temp.clear();
            }
        }
        else
            return;
    }

public:
    /**
     * @brief Constructor for class
     * @param vec - reference to a vector of floats
     * @param f - reference to an object of class controlFlags
     */
    inputToVector(std::vector<float>& vec, controlFlags &f) {
        stringGetter();
        removeDuplicatesAndCatchErrors(f);
        parseToVector(vec, f);
    }
};

/**
 * @class randomQuickSort - includes functions needed to sort an unsorted set of floats. Functions are static so that
 *                          it is not necessary to create an object of this class to sort the vector
 */
class randomQuickSort {
private:
    /**
     * @brief generate a random number to be used in randomisedPartition (choosing of a random pivot)
     * @param min
     * @param max
     * @return returns a number within the range of min and max
     */
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

    /**
     * @brief partitions unsorted list
     * @param vec - reference to a vector of floats
     * @param min
     * @param max
     * @ returns index on the list is partitioned
     */
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

    /**
     * @brief Responsible choosing a random pivot and then running partition function
     * @param vec - reference to a vector of floats
     * @param min
     * @param max
     * @ returns index on the list is partitioned
     */
    static int randomisedPartition(std::vector<float> &vec, int start, int end) {
        int pivotIndex = randomNumber(start, end);
        std::swap(vec[pivotIndex], vec[end]);
        return partition(vec, start, end);
    }

    /**
     * @brief Sorts unsorted set of floats
     * @param vec - reference to a vector of floats
     * @param min
     * @param max
     */
    static void QuickSort (std::vector<float> &vec, int start, int end) {
        if (start < end) {
            int pIndex {randomisedPartition(vec, start, end)};

            QuickSort(vec,start, pIndex-1);
            QuickSort(vec, pIndex+1, end);
        }
    }

public:
    /**
     * @brief function used to access classes functionality
     * @param vec - reference to a vector of floats
     */
    static void Sort(std::vector<float> &vec) {
        int start = 0;
        int end = static_cast<int>(vec.size() - 1);
        QuickSort(vec, start, end);
    }
};

/**
 * @class printer - prints the elements of a vector of floats
 */
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
        inputToVector input(container, programControl);

        if (!programControl.errorInProgram) {
            randomQuickSort::Sort(container);
            printer::print(container);
        }

        // Does the user want to run another operation?
        if (programControl.tryAgain() == 'R') {
            container.clear();
            programControl.errorReset();
        }
        else
            programControl.programRunning = false;
    }

    return 0;
}
