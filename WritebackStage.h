
class WritebackStage: public Stage {
private:
    bool mem_read(uint64_t icode); 
public:
    bool doClockLow(PipeRegister ** pregs, Stage ** stages) ;
    void doClockHigh(PipeRegister ** pregs);
    void valM_toReg(uint64_t valM, uint64_t dstM);
};

