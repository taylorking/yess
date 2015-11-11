
class Loader
{
private:
    bool loaded;   //set to true if a file is successfully loaded into memory
    ifstream inf;  //input file handle
    int32_t * lastMemLocation;
    void loadFile();
    bool checkFileName(char * fileName);
    bool checkFile(char * fileName);
    bool checkFileExists(char * fileName);
    void loadLine(string line, bool * error);
    int32_t stringNumToHex(string line);
public:
    Loader(int argc, char * argv[]);
    bool isLoaded();
};
