//these #defines can be used as indices in an array of stages
#define FSTAGE 0
#define DSTAGE 1
#define ESTAGE 2
#define MSTAGE 3
#define WSTAGE 4
//five stages: FetchStage, DecodeStage, ExecuteStage,
//             MemoryStage, WritebackStage
#define NUMSTAGES 5
class Stage
{
public:
    //abstract methods implemented in the descendant classes
    //virtual makes these methods polymorphic
    virtual bool doClockLow(PipeRegister ** pregs, Stage ** stages) = 0;
    virtual void doClockHigh(PipeRegister ** pregs) = 0;
};


