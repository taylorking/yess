
class FetchStage: public Stage
{
private:
    void setDInput(D * dreg, uint64_t stat, uint64_t icode, uint64_t ifun,
                   uint64_t rA, uint64_t rB,
                   uint64_t valC, uint64_t valP);
    uint64_t predictPC(uint64_t icode, uint64_t valC, uint64_t valP);
    uint64_t selectPC(F * freg, M * mreg, W * wreg);
    uint64_t PCincrement(int32_t f_pc, bool needsRegIds, bool needsValC);
    void getRegIds(int32_t f_pc , uint64_t * rA, uint64_t * rB);
    uint64_t buildValC(int32_t f_pc, bool needsRegIds);
    bool needsRegIds(uint64_t icode);
    bool needsValC(uint64_t icode);
public:
    bool doClockLow(PipeRegister ** pregs, Stage ** stages);
    void doClockHigh(PipeRegister ** pregs);
};
