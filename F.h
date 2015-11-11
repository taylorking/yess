
class F : public PipeRegister {
private:
    PipeReg * predPC;
public:
    F();
    PipeReg * getpredPC();
    void dump();
};
