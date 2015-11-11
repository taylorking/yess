
class M : public PipeRegister
{
private:
    PipeReg * stat;
    PipeReg * icode;
    PipeReg * Cnd;
    PipeReg * valE;
    PipeReg * valA;
    PipeReg * dstE;
    PipeReg * dstM;
public:
    M();
    PipeReg * getstat();
    PipeReg * geticode();
    PipeReg * getCnd();
    PipeReg * getvalE();
    PipeReg * getvalA();
    PipeReg * getdstE();
    PipeReg * getdstM();
    void dump();
};
