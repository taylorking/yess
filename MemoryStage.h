class MemoryStage: public Stage {
private:
    uint64_t m_valM;
    void setWInput(W * wreg, uint64_t stat, uint64_t icode, uint64_t valE, uint64_t valM, uint64_t dstE, uint64_t dstM);
    uint64_t mem_addr(uint64_t icode, uint64_t m_valA, uint64_t m_valE);
    bool mem_read(uint64_t icode);
    bool mem_write(uint64_t icode);

public:
    bool doClockLow(PipeRegister ** pregs, Stage ** stages);
    void doClockHigh(PipeRegister ** pregs);
    uint64_t get_valM();
};

