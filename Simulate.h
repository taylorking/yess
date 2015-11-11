class Simulate
{
private:
    PipeRegister ** piperegs;
    Stage ** stages;
public:
    Simulate();
    void run();
    bool doClockLow();
    void doClockHigh();
    void dumpPipeRegs();
};
