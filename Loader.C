/**
 * Names:
 * Team:
*/
#include <iostream>
#include <fstream>
#include <string.h>
#include <ctype.h>
#include <sstream>
using namespace std;

#include "Loader.h"
#include "Memory.h"
#include "Tools.h"
//first column in file is assumed to be 0
#define ADDRBEGIN 2   //starting column of 3 digit hex address 
#define ADDREND 4     //ending column of 3 digit hext address
#define DATABEGIN 7   //starting column of data bytes
#define COMMENT 28    //location of the '|' character 
#define ASCII_OFFSET 48
#define COLON 5
#define SPACE 6
/**
 * Loader constructor
 * Opens the .yo file named in the command line arguments, reads the contents of the file
 * line by line and loads the program into memory.  If no file is given or the file doesn't
 * exist or the file doesn't end with a .yo extension or the .yo file contains errors then
 * loaded is set to false.  Otherwise loaded is set to true.
 *
 * @param argc is the number of command line arguments passed to the main; should
 *        be 2
 * @param argv[0] is the name of the executable
 *        argv[1] is the name of the .yo file
 */
Loader::Loader(int argc, char * argv[])
{
    loaded = false;

    // The user has not supplied a file name.
    if(argc < 2) {
        return;
    }

    if(!checkFile(argv[1])) {
        return;
    }

    loadFile();
    //start by writing a method that opens the file (checks whether it ends with a .yo and
    //whether the file successfully opens; if not, return without loading)

    //next write a simple loop that reads the file line by line and prints it out

    //next, add a method that will write the data in the line to memory (call that from within
    //your loop)

    //Finally, add code to check for errors in the input line.
    //When your code finds an error, you need to print an error message and return.
    //Since your output has to be identical to your instructor's, use this cout to print the
    //error message.  Change the variable names if you use different ones.
    //  cout << "Error on line " << dec << lineNumber
    //       << ": " << line << endl;
}

void Loader::loadFile() {
    string line;
    int linenumber = 1;
    bool error;
    int initialMem = -1;
    lastMemLocation = &initialMem;

    while(getline(inf, line)) {
        loadLine(line, &error);

        if(error) {
            printf("Error on line %d: %s\n", linenumber, line.c_str());
            return;
        }

        linenumber++;
    }

    loaded = true;
}

void Loader::loadLine(string line, bool * error) {
    bool iscomment = true;

    if(line[COMMENT] != '|') {
        *error = true;
        return;
    }

    for(int i = ADDRBEGIN; i < COMMENT; i++) {
        if(line[i] != ' ') {
            iscomment = false;
            break;
        }
    }

    if(iscomment) {
        return;
    }

    if(line[1] != 'x' || line[0] != '0') {
        *error = true;
        return;
    }

    string address = "";

    for(int i = ADDRBEGIN; i <= ADDREND; i++) {
        if(!isxdigit(line[i]))  {
            *error = true;
            return;
        }

        address += line[i];
    }

    if(line[COLON] != ':') {
        *error = true;
        return;
    }

    // Convert the address to hexadecimal
    int32_t baseAddr = stringNumToHex(address);
    int numBits = 0;
    bool donereading = false;

    if(line[DATABEGIN] == ' ') {
        donereading = true;
    }

    for(int i = DATABEGIN; i < COMMENT; i++) {
        if((line[i] != ' ') != donereading) {
            if(donereading) {
                continue;
            }

            if(!isxdigit(line[i])) {
                *error = true;
                return;
            }

            numBits++;

            if(line[i + 1] == ' ') {
                donereading = true;
            }
        } else {
            *error = true;
            return;
        }
    }

    if(numBits % 2 != 0) {
        *error = true;
        return;
    }

    int32_t byteCount = numBits / 2;

    if(byteCount > 0 && line[SPACE] != ' ') {
        *error = true;
        return;
    }

    if(baseAddr <= *lastMemLocation) {
        *error = true;
        return;
    }

    if(baseAddr + byteCount > MEMSIZE) {
        *error = true;
        return;
    }

    Memory * mem = Memory::getInstance();

    for(int i = 0; i < byteCount; i++) {
        // Build the byte as a string
        string byte = "";
        byte += line[DATABEGIN + (i * 2)];
        byte += line[DATABEGIN + ((i * 2) + 1)];
        // Convert it into an int
        int byteVal = stringNumToHex(byte);
        // Load it into memory
        mem->putByte(byteVal, baseAddr + i, *error);
        // Update the last memory location
        *lastMemLocation = baseAddr + i;
    }
}
int32_t Loader::stringNumToHex(string num) {
    istringstream iss(num);
    int32_t addrNum;
    iss >> hex >> addrNum;
    return addrNum;
}

bool Loader::checkFile(char * fileName) {
    return checkFileName(fileName) && checkFileExists(fileName);
}

bool Loader::checkFileName(char * fileName) {
    uint32_t strLength = strlen(fileName);
    uint32_t offset = strLength -3;
    char * p = fileName;
    p += offset;
    char match[] = ".yo\n";
    return strcmp(p, match);
}

bool Loader::checkFileExists(char * fileName) {
    inf.open(fileName);
    return inf.good();
}
/**
 * isLoaded
 * returns the value of the loaded data member; loaded is set by the constructor
 *
 * @return value of loaded (true or false)
 */
bool Loader::isLoaded()
{
    return loaded;
}


//You'll need to add more helper methods to this file.  Don't put all of your code in the
//Loader constructor.  When you add a method here, add the prototype to Loader.h in the private
//section.
