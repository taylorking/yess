class W: public PipeRegister
{
private:
    PipeReg * stat;
    PipeReg * icode;
    PipeReg * valE;
    PipeReg * valM;
    PipeReg * dstE;
    PipeReg * dstM;
public:
    W();
    PipeReg * getstat();
    PipeReg * geticode();
    PipeReg * getvalE();
    PipeReg * getvalM();
    PipeReg * getdstE();
    PipeReg * getdstM();
    void dump();
};
