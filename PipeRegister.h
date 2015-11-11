
//these can be used as indices into an array of PipeRegisters
#define FREG 0
#define DREG 1
#define EREG 2
#define MREG 3
#define WREG 4

//number of PipeRegisters
#define NUMPIPEREGS 5

class PipeRegister
{
public:
    //dump method is implemented in the classes that descend
    //from PipeRegister
    //
    //dump is abstract
    //virtual makes it polymorphic
    virtual void dump() = 0;
};


