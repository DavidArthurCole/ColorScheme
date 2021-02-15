#include <iostream>
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <array>
#include <sstream>
#include <random>
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
void printHelp();
int isHexOk(std::string hexIn);
//Prints options
void printOptions();
void setConsoleColor(int n);
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//Main runtime
int main(void) {
    //Buffer for int
    int validChoiceMax = CHOICES, filledArrayPos = 0, currentColor = 0, validInput = 0;
    std::string fillArray, choicesArray[MAX_SIZE] = { "0 - Rainbow", "1 - Master", "2 - Ordered", "3 - Random", "4 - Millionaire", "5 - Phoenix", "6 - Dragon", "7 - Bacon", "8 - Hex Randomizer", "9 - Hex Blend" };
    std::string intBuffer, input, output = "", colorArray[MAX_SIZE], secondInput, validHex[16];
    for (int i = 0; i <= 15; i++)
    {
        validHex[i] = "0123456789ABCDEF"[i];
    }

    printOptions();

    while (validInput == 0) {
        std::cout << "\nChoose an option:\n";
        //Acts as a time/delay buffer to wait for user input
        std::getline(std::cin, intBuffer);
        if (intBuffer[0] >= '0' && intBuffer[0] <= '9') {
            //Error checking for solely numerical values (between 0 and max);
            if (intBuffer.length() > 1)
            {
                std::cout << "Non-valid numerical input.\n";
            }
            else if (stoi(intBuffer) <= validChoiceMax) { validInput = 1; }
            else { std::cout << "Non-valid numerical input.\n"; }
        }
        else { std::cout << "Non-numeric input.\n"; }
    }
    std::string hexOne, hexTwo, hexBuffer;
    bool hexBufferOk = false;
    int hexReturnCode, codesEntered = 0;
    if (intBuffer == "9")
    {
        while (codesEntered < 2)
        {
            hexBufferOk = false;
            while (hexBufferOk == false)
            {
                std::string grammar;
                if (codesEntered == 0)
                {
                    grammar = "first";
                }
                else
                {
                    grammar = "second";
                }
                std::cout << "\nEnter the " << grammar << " hex value";
                setConsoleColor(8);
                std::cout << " (type 'help' for common hex codes)";
                setConsoleColor(15);
                std::cout << ":\n";
                std::getline(std::cin, hexBuffer);
                //Removes # from hex codes; won't be implemented until new features are added.
                if (hexBuffer.substr(0, 1) == "#")
                {
                    hexBuffer = hexBuffer.substr(1, (hexBuffer.length() - 1));
                }
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
            if (codesEntered == 0)
            {
                hexOne = hexBuffer;
            }
            else
            {
                hexTwo = hexBuffer;
            }
            codesEntered += 1;
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
    case 4: fillArray = "666eeefff"; break;
    case 5: fillArray = "4c6ef78"; break;
    case 6: fillArray = "55da22"; break;
    case 7: fillArray = "c6666"; break;
    case 8: {
        srand((unsigned int)time(NULL));
        for (int i = 0; i < input.length(); i++)
        {
            std::string rndHex = "";
            for (int i = 0; i <= 5; i++)
            {
                rndHex += validHex[(rand() % 15)];
            }
            output += "&#" + rndHex + input[i];
        }
    } break;
    case 9: {
        std::string hexVals[MAX_SIZE], hexOneRed = hexOne.substr(0, 2), hexOneGreen = hexOne.substr(2, 2), hexOneBlue = hexOne.substr(4, 2);
        std::string hexTwoRed = hexTwo.substr(0, 2), hexTwoGreen = hexTwo.substr(2, 2), hexTwoBlue = hexTwo.substr(4, 2);

        float hexOneRVal = intFromHex(hexOneRed), hexOneGVal = intFromHex(hexOneGreen), hexOneBVal = intFromHex(hexOneBlue);
        float hexTwoRVal = intFromHex(hexTwoRed), hexTwoGVal = intFromHex(hexTwoGreen), hexTwoBVal = intFromHex(hexTwoBlue);
        float difR = hexOneRVal - hexTwoRVal, difG = hexOneGVal - hexTwoGVal, difB = hexOneBVal - hexTwoBVal;
        int adjLength = 0;
        for (int i = 0; i < input.length(); i++)
        {
            if (input.substr(i, 1) != " ")
            {
                adjLength++;
            }
        }
        std::cout << "adjLength: " << adjLength;
        float steps = (adjLength - 1), resRed, resGreen, resBlue;

        for (float i = 0; i <= steps; i++)
        {
            float percent = float(i / (steps));
            resRed = hexOneRVal + round(percent * (hexTwoRVal - hexOneRVal));
            resGreen = hexOneGVal + round(percent * (hexTwoGVal - hexOneGVal));
            resBlue = hexOneBVal + round(percent * (hexTwoBVal - hexOneBVal));

            std::string hexToAdd = hexFromInt(resRed) + hexFromInt(resGreen) + hexFromInt(resBlue);
            std::for_each(hexToAdd.begin(), hexToAdd.end(), [](char& c)
                {c = std::toupper(c); });
            if (input[i] != ' ')
            {
                output += "&#" + hexToAdd + input[i];
            }
            else
            {
                output += " ";
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

void caseArrayFill(int& filledArrayPos, std::string(&colorArray)[MAX_SIZE], std::string fillArray) {
    filledArrayPos = (unsigned int)fillArray.size() - 1;
    for (int i = 0; i <= filledArrayPos; i++) { colorArray[i] = "&" + fillArray.substr(i, 1); }
}

int intFromHex(std::string hexIn)
{
    int intOut;
    std::stringstream ss;
    ss << std::hex << hexIn;
    ss >> intOut;
    return(intOut);
}

std::string hexFromInt(int intIn)
{
    std::string hexOut;
    std::stringstream ss2;
    ss2 << std::hex << intIn;
    ss2 >> hexOut;
    if (hexOut.length() != 2)
    {
        hexOut = "0" + hexOut;
    }
    return(hexOut);
}

int isHexOk(std::string hexIn)
{
    std::for_each(hexIn.begin(), hexIn.end(), [](char& c)
        {c = std::toupper(c); });
    if (hexIn == "HELP") //Print HELP coces
    {
        return(4);
    }
    if (hexIn.length() < 6) //Too short, add leading 0s
    {
        return(1);
    }
    else if (hexIn.length() > 6) //Too long, verify-
    {
        return(2);
    }
    else
    {
        int okCount = 0;
        for (int i = 0; i <= 5; i++)
        {
            for (int h = 0; h <= 16; h++)
            {
                if (hexIn[i] == "0123456789ABCDEF"[h])
                {
                    okCount++;
                }
            }
        }
        //Ok
        if (okCount == 6)
        {
            return(0);
        }
        else { return(3); }
    }
}

void printHelp()
{
    std::string helpColors[16] = { "Red", "Orange", "Yellow", "Lime", "Green", "Turquoise", "Light Blue", "Blue", "Dark Blue", "Violet", "Magenta", "Black", "Dark Gray", "Gray", "Light Gray", "White" };
    int helpColorsConsoleInt[16] = { 12, 6, 14, 10, 2, 3, 11, 9, 1, 5, 13, 8, 8, 8, 7, 15 };
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
        setConsoleColor(15);
    }
    std::cout << "   -----------------------" << "\n\n";
}

void setConsoleColor(int n) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, n);
}

void printOptions()
{
    const int CHOICES = 10;

    int colors[CHOICES][7] = {
        {12, 4, 6, 14, 10, 11 , 13}, //Rainbow
        {4, 12, 15, 15, 15, 15, 15}, //Master
        { 8, 1, 2, 3, 4, 5, 6}, //Ordered
        {((rand() % 15) + 1), ((rand() % 15) + 1), ((rand() % 15) + 1), ((rand() % 15) + 1), ((rand() % 15) + 1), ((rand() % 15) + 1), 15}, //Random
        {6, 14, 15, 6, 15, 15, 15}, //Millionaire
        {4, 12, 6, 14, 15, 7, 8}, //Phoenix
        {5, 13, 10, 2, 15, 15, 15}, //Dragon
        {12, 6, 15, 15, 15, 15, 15}, //Bacon
        {15, 15, 15, 15, 15, 15, 15}, //Hex Randomizer
        {15, 15, 15, 15, 15, 15, 15}, //Hex Blender
    };

    std::string textToPrint[CHOICES][7] = {
        {"R", "a", "i", "n", "b", "o", "w"},
        {"M", "a", "ster", "", "", "", ""},
        {"O", "r", "d", "e", "r", "e", "d"},
        {"R", "a", "n", "d", "o", "m", ""},
        {"Mil", "lio", "nai", "re", "", "", ""},
        {"P", "h", "o", "e", "n", "i", "x"},
        {"Dr", "a", "g", "on", "", "", ""},
        {"B", "acon", "", "", "", "", ""},
        {"Hex Randomizer", "", "", "", "", "", ""},
        {"Hex Blender", "", "", "", "", "", ""}
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
            setConsoleColor(colors[i][j]);
            std::cout << textToPrint[i][j];
        }
        for (int k = 0; k < extraSpaces[i]; k++) {
            std::cout << " ";
        }
        setConsoleColor(15);
        std::cout << "|\n";
    }
    std::cout << "   ----------------------" << "\n";
}