
class D : public PipeRegister
{
private:
    PipeReg * stat;
    PipeReg * icode;
    PipeReg * ifun;
    PipeReg * rA;
    PipeReg * rB;
    PipeReg * valC;
    PipeReg * valP;
public:
    D();
    PipeReg * getstat();
    PipeReg * geticode();
    PipeReg * getifun();
    PipeReg * getrA();
    PipeReg * getrB();
    PipeReg * getvalC();
    PipeReg * getvalP();
    void dump();
};
