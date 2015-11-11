
class WritebackStage: public Stage {
public:
    bool doClockLow(PipeRegister ** pregs, Stage ** stages) ;
    void doClockHigh(PipeRegister ** pregs);
};

