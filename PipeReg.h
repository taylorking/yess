
class PipeReg
{
private:
    uint64_t input;   //current input to the register
    uint64_t state;   //current state (output)
public:
    PipeReg(int state = 0);
    void setInput(uint64_t input);
    uint64_t getOutput();
    void normal();
};



