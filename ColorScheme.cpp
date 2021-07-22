#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <array>
#include <sstream>
#include <random>
#include <cmath>
#include <algorithm>

//COLOR LIBRARY - https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c
const int DARK_BLUE = 1, DARK_GREEN = 2, CYAN = 3, DARK_RED = 4, MAGENTA = 5, GOLD = 6, DULL_WHITE = 7, GRAY = 8, BLUE = 9, LIME = 10, LIGHT_BLUE = 11, RED = 12, PINK = 13, YELLOW = 14, WHITE = 15;

// Maximum size for a color scheme, can be edited
const int MAX_SIZE = 32;
//Number of choices
const int CHOICES = 10;
//Function declarations
void caseArrayFill(int& filledArrayPos, std::string(&colorArray)[MAX_SIZE], std::string fillArray);
//For use in hex blender, returns the int value of a hex string
int intFromHex(std::string hex);
//Opposite of ^^^^
std::string hexFromInt(int intVal);
//Prints common hex codes
void printHelp();
//Verifies hex codes
int isHexOk(std::string hexIn);
//Prints options
void printOptions();
void setConsoleColor(int n);
bool is_number(const std::string& s);
std::string blend(std::string hexOne, std::string hexTwo, std::string input);
std::string hexCodesArray[6];
//Main runtime
int main(void) {
    //Buffer for int
    int validChoiceMax = CHOICES, filledArrayPos = 0, currentColor = 0, validInput = 0;
    std::string fillArray, intBuffer, input, output = "", colorArray[MAX_SIZE], secondInput;
    char validHex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    printOptions();

    while (validInput == 0) {
        std::cout << "\nChoose an option:\n";
        //Acts as a time/delay buffer to wait for user input
        std::getline(std::cin, intBuffer);
        if (intBuffer[0] >= '0' && intBuffer[0] <= '9') {
            //Error checking for solely numerical values (between 0 and max);
            if (intBuffer.length() > 1) std::cout << "Non-valid numerical input.\n";
            else if (stoi(intBuffer) <= validChoiceMax) validInput = 1;
            else std::cout << "Non-valid numerical input.\n";
        }
        else { std::cout << "Non-numeric input.\n"; }
    }
    std::string hexOne, hexTwo, hexBuffer;
    bool hexBufferOk = false;
    int codesEntered = 0;
    int howManyCodes;
    if (intBuffer == "9")
    {

        //Tracks user's inputs per how many codes they want to blend
        bool validNumOfCodes = false;   
        std::string stringHowManyCodes;

        while (!validNumOfCodes) {

            std::cout << "How many colors are you blending? Default: 2\n";
            //Get input
            std::getline(std::cin, stringHowManyCodes);

            //Parse input, no input defaults to 2 codes
            if (stringHowManyCodes == "") {
                howManyCodes = 2;
                validNumOfCodes = true;
            }
            else if (is_number(stringHowManyCodes)) {
                howManyCodes = std::stoi(stringHowManyCodes);
                validNumOfCodes = true;
            }
            else {
                std::cout << "Your input could not be recognized, please enter a numeric value.\n";
            }
        }

        std::string grammarArray[] = { "first", "second", "third", "fourth", "fifth", "sixth"};

        while (codesEntered < howManyCodes)
        {
            hexBufferOk = false;
            std::string grammar;
            while (hexBufferOk == false)
            {
                //Gets the correct grammar for the prompt
                if (codesEntered <= 9) {
                   grammar = grammarArray[codesEntered];
                }
                //Prevents overflow
                else {
                    grammar = "next";
                }
                

                std::cout << "\nEnter the " << grammar << " hex value";
                setConsoleColor(GRAY);
                std::cout << " (type 'help' for common hex codes)";
                setConsoleColor(WHITE);
                std::cout << ":\n";
                std::getline(std::cin, hexBuffer);
                //Removes # from hex codes; won't be implemented until new features are added.
                if (hexBuffer.substr(0, 1) == "#") hexBuffer = hexBuffer.substr(1, (hexBuffer.length() - 1));
                int hexReturnCode = isHexOk(hexBuffer);
                switch (hexReturnCode)
                {
                case 0:
                    hexBufferOk = true;
                    break;
                case 1:
                    std::cout << "The hex you entered is too short, add leading zeros, or verify the value.\n";
                    break;
                case 2:
                    std::cout << "The hex you entered is too long, verify the value.\n";
                    break;
                case 3:
                    std::cout << "The hex code entered contained non-hex characters [0-9, A-F]. Verify the value.\n";
                    break;
                case 4:
                    std::cout << "\n";
                    printHelp();
                    break;
                default:
                    std::cout << "An unknown error has occured, please report this.\n";
                    break;
                }
            }
            //Stores the hex code in an array for use later
            hexCodesArray[codesEntered] = hexBuffer;
            ++codesEntered;
        }
    }
    std::cout << "\nEnter an input:\n";
    //Needs to get space separated characters
    std::getline(std::cin, input);
    switch (stoi(intBuffer)) {
        case 0: fillArray = "c46eab9d5"; break;//Rainbow
        case 1: fillArray = "4cffff"; break; //Master
        case 2:
        case 3: fillArray = "0123456789abcdef"; break; //Ordered or Random
        case 4: fillArray = "666eeefff"; break; //Millionaire
        case 5: fillArray = "4c6ef78"; break; //Phoenix
        case 6: fillArray = "55da22"; break; //Dragon
        case 7: fillArray = "c6666"; break; //Bacon
        case 8: { //Hex Randomizer
            srand((unsigned int)time(NULL));
            for (int i = 0; i < input.length(); i++) {
                std::string rndHex = "";
                for (int i = 0; i <= 5; i++) rndHex += validHex[(rand() % 15)];
                output += "&#" + rndHex + input[i];
            }
        } break;
        case 9: { //Hex blender

            //This is good code...
            if (howManyCodes == 2) output = blend(hexCodesArray[0], hexCodesArray[1], input);
            else {
                //Will store the separated strings to blend
                std::string inputSeparated[5];

                //Gets the input length
                int inputLength = input.length();

                int midPoints = howManyCodes - 2;

                int startPoint = 0;
                int endPoint = inputLength - 1;

                // If the number of midpoints is even
                if (midPoints % 2 == 0) {

                    int midpoint1, midpoint2, midpoint3, midpoint4;

                    switch (midPoints) {
                        case 2:
                            midpoint1 = startPoint + 2;
                            midpoint2 = endPoint - 2;
                            
                            while (((midpoint2 - midpoint1) - midpoint1) >= 2) {
                                midpoint1 += 1;
                                midpoint2 -= 1;
                            }

                            inputSeparated[0] = input.substr(startPoint, (int)((double)midpoint1 + 1.0));
                            inputSeparated[1] = input.substr(midpoint1, (int)((double)midpoint2 - (double)midpoint1 + 1.0));
                            inputSeparated[2] = input.substr(midpoint2, (int)((double)endPoint - (double)midpoint2 + 1.0));

                            break;
                        case 4:

                            midpoint1 = startPoint + 2;
                            midpoint2 = midpoint1 + 2;
                            midpoint4 = endPoint - 2;
                            midpoint3 = midpoint4 - 2;

                            while (std::abs((midpoint2 - midpoint1) - (midpoint3 - midpoint2)) >= 4) {
                                midpoint1 += 1;
                                midpoint2 += 2;
                                midpoint4 -= 1;
                                midpoint3 -= 2;
                            }

                            inputSeparated[0] = input.substr(startPoint, (int)(midpoint1 + 1.0));
                            inputSeparated[1] = input.substr(midpoint1, (int)((double)midpoint2 - midpoint1 + 1));
                            inputSeparated[2] = input.substr(midpoint2, (int)((double)midpoint3 - midpoint2 + 1));
                            inputSeparated[3] = input.substr(midpoint3, (int)((double)midpoint4 - midpoint3 + 1));
                            inputSeparated[4] = input.substr(midpoint4, (int)((double)endPoint - midpoint4 + 1));

                            break;
                    }

                    std::cout << "midpoint1: " << midpoint1 << "\n";
                    std::cout << "midpoint2: " << midpoint2 << "\n";
                    std::cout << "midpoint3: " << midpoint3 << "\n";
                    std::cout << "midpoint4: " << midpoint4 << "\n";
                    

                }
                // If the number of midpoints is odd
                else {

                    int midpoint1, midpoint2, midpoint3;

                    switch (midPoints) {

                        case 1:

                            if (input.length() % 2 == 1) {
                                midpoint1 = endPoint / 2;
                            }
                            else {
                                midpoint1 = (endPoint + 1) / 2;
                            }

                            inputSeparated[0] = input.substr(startPoint, (int)(midpoint1 + 1.0));
                            inputSeparated[1] = input.substr(midpoint1, (int)((double)endPoint - midpoint1 + 1));

                            break;

                        case 3:

                            //Calculate center midpoint (midpoint2)
                            if (input.length() % 2 == 1) {
                                midpoint2 = endPoint / 2;
                            }
                            else {
                                midpoint2 = (endPoint + 1) / 2;
                            }

                            //Calculate midpoint1
                            if (midpoint2 % 2 == 0) {
                                midpoint1 = (midpoint2 / 2);
                            }
                            else {
                                midpoint1 = ((midpoint2 + 1) / 2);
                            }

                            //Calculate midpoint3
                            if ((endPoint - midpoint2) % 2 == 1) {
                                midpoint3 = (endPoint - (midpoint2 / 2));
                            }
                            else {
                                midpoint3 = (endPoint - ((midpoint2 + 1) / 2));
                            }

                            inputSeparated[0] = input.substr(startPoint, (int)(midpoint1 + 1.0));
                            inputSeparated[1] = input.substr(midpoint1, (int)((double)midpoint2 - midpoint1 + 1));
                            inputSeparated[2] = input.substr(midpoint2, (int)((double)midpoint3 - midpoint2 + 1));
                            inputSeparated[3] = input.substr(midpoint3, (int)((double)endPoint - midpoint3 + 1));

                            break;
                    }
                }

                for (int i = 0; i <= 4; i++) {
                    if (inputSeparated[i] != "") {
                        if (i == 0) {
                            output += blend(hexCodesArray[i], hexCodesArray[i + 1], inputSeparated[i]);
                        }
                        else {
                            std::string temp = blend(hexCodesArray[i], hexCodesArray[i + 1], inputSeparated[i]);
                            output += temp.substr(9, temp.length() - 9);
                        }
                    }
                }
            }
            
        } break;
    }
    if (std::stoi(intBuffer) != 8 && std::stoi(intBuffer) != 9)
    {
        caseArrayFill(filledArrayPos, colorArray, fillArray);
        //Calls the randomization function
        srand((unsigned int)time(NULL));
        //Run until the entire input has been handled
        for (unsigned int i = 0; i < input.length(); i++, currentColor++) {
            //Deals with anything besides the random scheme
            if (stoi(intBuffer) <= 2 || stoi(intBuffer) >= 4) {
                //Handles looping the scheme back to the beginning
                if (currentColor == (filledArrayPos + 1)) { currentColor = 0; }
                //Will not add color codes to space characters
                if (input[i] != ' ') {
                    // Appends the color code of place (currentColor) in colorArray,
                    // as well as the next character in the string to the output
                    output += colorArray[currentColor] + input[i];
                    //Cycles to the next color in the scheme
                }
                else { output += ' '; currentColor--; }
            }
            else if (stoi(intBuffer) == 3) {
                //Does not add a color code to space characters
                if (input[i] != ' ') {
                    // Appends the color code of place (currentColor) in colorArray,
                    // as well as the next character in the string to the output
                    output += colorArray[(rand() % 15)] + input[i];
                }
                else { output += ' '; currentColor--; }
            }
        }
    }
    //Opens and empties the clipboard
    OpenClipboard(0);
    EmptyClipboard();
    // Defines the allocated memory size, and if the string
    // is not a proper size, will exit the function
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, output.size() + 1);
    if (!hg) { CloseClipboard(); return(0); }
    //Copies the string to memory
    memcpy(GlobalLock(hg), output.c_str(), output.size() + 1);
    GlobalUnlock(hg);
    //Writes memString to clipboard
    SetClipboardData(CF_TEXT, hg);
    //Closes clipboard and frees the memory
    CloseClipboard();
    GlobalFree(hg);
    main();
    return(0);
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void caseArrayFill(int& filledArrayPos, std::string(&colorArray)[MAX_SIZE], std::string fillArray) {
    filledArrayPos = (unsigned int)fillArray.size() - 1;
    for (int i = 0; i <= filledArrayPos; i++) { colorArray[i] = "&" + fillArray.substr(i, 1); }
}

int intFromHex(std::string hexIn) {
    int intOut;
    std::stringstream ss;
    ss << std::hex << hexIn;
    ss >> intOut;
    return(intOut);
}

std::string hexFromInt(int intIn) {
    std::string hexOut;
    std::stringstream ss2;
    ss2 << std::hex << intIn;
    ss2 >> hexOut;
    if (hexOut.length() != 2){
        hexOut = "0" + hexOut;
    }
    return(hexOut);
}

int isHexOk(std::string hexIn) {
    std::for_each(hexIn.begin(), hexIn.end(), [](char& c)
        {c = std::toupper(c); });
    if (hexIn == "HELP") return(4); //Print HELP codes
    else if (hexIn.length() < 6) return(1); //Too short, add leading 0s
    else if (hexIn.length() > 6) return(2); //Too long, verify
    else
    {
        int okCount = 0;
        for (int i = 0; i <= 5; i++)
        {
            for (int h = 0; h <= 16; h++)
            {
                if (hexIn[i] == "0123456789ABCDEF"[h]) okCount++;
            }
        }
        //Ok
        if (okCount == 6) return(0);
        else return(3);
    }
}

void printHelp() {
    std::string helpColors[16] = { "Red", "Orange", "Yellow", "Lime", "Green", "Turquoise", "Light Blue", "Blue", "Dark Blue", "Violet", "Magenta", "Black", "Dark Gray", "Gray", "Light Gray", "White" };
    int helpColorsConsoleInt[16] = { RED, GOLD, YELLOW, LIME, DARK_GREEN, CYAN, LIGHT_BLUE, BLUE, DARK_BLUE, MAGENTA, PINK, GRAY, GRAY, GRAY, DULL_WHITE, WHITE };
    std::string helpColorsHex[16] = { "FF0000", "FF8C00", "FFEE00", "77FF00", "22FF00", "00FFBB", "00FFFF", "0099FF", "0022FF", "8800FF", "FF00FF", "000000", "474747", "7A7A7A", "ADADAD", "FFFFFF" };

    for (int i = 0; i < 16; i++)
    {
        std::cout << "   -----------------------" << "\n";
        setConsoleColor(helpColorsConsoleInt[i]);
        std::cout << "   | " << helpColors[i];
        for (int j = helpColors[i].length(); j < 11; j++) {
            std::cout << " ";
        }
        std::cout << "| " << helpColorsHex[i] << " |\n";
        setConsoleColor(WHITE);
    }
    std::cout << "   -----------------------" << "\n\n";
}

void setConsoleColor(int n) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
}

void printOptions() {
    const int CHOICES = 10;

    int colors[CHOICES][7] = {
        {RED, DARK_RED, GOLD, YELLOW, LIME, LIGHT_BLUE , PINK}, //Rainbow
        {DARK_RED, RED}, //Master
        { GRAY, DARK_BLUE, DARK_GREEN, CYAN, DARK_RED, MAGENTA, GOLD}, //Ordered
        {((rand() % 15) + 1), ((rand() % 15) + 1), ((rand() % 15) + 1), ((rand() % 15) + 1), ((rand() % 15) + 1), ((rand() % 15) + 1)}, //Random
        {GOLD, YELLOW, WHITE, GOLD}, //Millionaire
        {DARK_RED, RED, GOLD, YELLOW, WHITE, DULL_WHITE, GRAY}, //Phoenix
        {MAGENTA, PINK, LIME, DARK_GREEN}, //Dragon
        {RED, GOLD}, //Bacon
        {WHITE}, //Hex Randomizer
        {WHITE}, //Hex Blender
    };

    std::string textToPrint[CHOICES][7] = {
        {"R", "a", "i", "n", "b", "o", "w"},
        {"M", "a", "ster"},
        {"O", "r", "d", "e", "r", "e", "d"},
        {"R", "a", "n", "d", "o", "m"},
        {"Mil", "lio", "nai", "re"},
        {"P", "h", "o", "e", "n", "i", "x"},
        {"Dr", "a", "g", "on"},
        {"B", "acon"},
        {"Hex Randomizer"},
        {"Hex Blender"}
    };

    int extraSpaces[CHOICES] = {
        {8}, //Rainbow
        {9}, //Master
        {8}, //Ordered
        {9}, //Random
        {4}, //Millionaire
        {8}, //Phoenix
        {9}, //Dragon
        {10}, //Bacon
        {1}, //Hex randomizer
        {4} //Hex blender
    };

    //Prints choices
    for (int i = 0; i < CHOICES; i++) {
        std::cout << "   ----------------------" << "\n";
        std::cout << "   | " << std::to_string(i) << " | ";
        int difToPrint = (sizeof(textToPrint[i]) / sizeof(int));
        for (int j = 0; j < 7; j++) {
            //If the color array at index is empty, set color to white (default)
            if (colors[i][j] == 0) setConsoleColor(WHITE);
            //Otherwise, use the color in the array
            else setConsoleColor(colors[i][j]);
            //Print the text for that color
            std::cout << textToPrint[i][j];
        }
        for (int k = 0; k < extraSpaces[i]; k++) {
            //Prints extra spaces to line up text
            std::cout << " ";
        }
        setConsoleColor(WHITE);
        std::cout << "|\n";
    }
    std::cout << "   ----------------------" << "\n";
}

std::string blend(std::string hexOne, std::string hexTwo, std::string input) {

    std::string output;

    //Grabs sections of each hex code RRGGBB
    std::string hexOneRed = hexOne.substr(0, 2), hexOneGreen = hexOne.substr(2, 2), hexOneBlue = hexOne.substr(4, 2);
    std::string hexTwoRed = hexTwo.substr(0, 2), hexTwoGreen = hexTwo.substr(2, 2), hexTwoBlue = hexTwo.substr(4, 2);

    float hexOneRVal = intFromHex(hexOneRed), hexOneGVal = intFromHex(hexOneGreen), hexOneBVal = intFromHex(hexOneBlue);
    float hexTwoRVal = intFromHex(hexTwoRed), hexTwoGVal = intFromHex(hexTwoGreen), hexTwoBVal = intFromHex(hexTwoBlue);
    
    int adjLength = 0;
    for (int i = 0; i < input.length(); i++) {
        if (input.substr(i, 1) != " ") adjLength++;
    }
    float steps = (adjLength - 1), resRed, resGreen, resBlue;

    for (float i = 0; i <= steps; i++) {
        float percent = float(i / (steps));
        resRed = hexOneRVal + round(percent * (hexTwoRVal - hexOneRVal));
        resGreen = hexOneGVal + round(percent * (hexTwoGVal - hexOneGVal));
        resBlue = hexOneBVal + round(percent * (hexTwoBVal - hexOneBVal));

        std::string hexToAdd = hexFromInt(resRed) + hexFromInt(resGreen) + hexFromInt(resBlue);
        std::for_each(hexToAdd.begin(), hexToAdd.end(), [](char& c)
            {c = std::toupper(c); });
        if (input[i] != ' ') output += "&#" + hexToAdd + input[i];
        else output += " ";
    }

    return output;
}