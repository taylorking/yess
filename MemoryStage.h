class MemoryStage: public Stage {
private:
    void setWInput(W * wreg, uint64_t stat, uint64_t icode, uint64_t valE, uint64_t valM, uint64_t dstE, uint64_t dstM);
public:
    bool doClockLow(PipeRegister ** pregs, Stage ** stages);
    void doClockHigh(PipeRegister ** pregs);
};

