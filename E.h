
class E : public PipeRegister
{
private:
    PipeReg * stat;
    PipeReg * icode;
    PipeReg * ifun;
    PipeReg * valC;
    PipeReg * valA;
    PipeReg * valB;
    PipeReg * dstE;
    PipeReg * dstM;
    PipeReg * srcA;
    PipeReg * srcB;
public:
    E();
    PipeReg * getstat();
    PipeReg * geticode();
    PipeReg * getifun();
    PipeReg * getvalC();
    PipeReg * getvalA();
    PipeReg * getvalB();
    PipeReg * getdstE();
    PipeReg * getdstM();
    PipeReg * getsrcA();
    PipeReg * getsrcB();
    void dump();
};
