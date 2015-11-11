class ExecuteStage: public Stage
{
private:
    uint64_t dstE, valE;
    uint64_t aluA(uint64_t e_icode, uint64_t e_valA, uint64_t e_valC);
    uint64_t aluB(uint64_t e_icode, uint64_t e_valB);
    uint64_t alufun(uint64_t e_icode, uint64_t e_ifun);
    bool set_cc(uint64_t e_icode);
    uint64_t e_dstE(uint64_t e_icode, uint64_t e_dstE, uint64_t e_cnd);
    void setMInput(M * mreg, uint64_t stat, uint64_t icode, uint64_t valE, uint64_t valA, uint64_t dstE, uint64_t dstM);
    uint64_t exec_alu(uint64_t alu_fun, uint64_t aluA, uint64_t aluB, bool setcc);
public:
    bool doClockLow(PipeRegister ** pregs, Stage ** stages) ;
    void doClockHigh(PipeRegister ** pregs);
    uint64_t get_dstE();
    uint64_t get_valE();
};


