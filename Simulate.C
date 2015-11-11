/*
 * Simulate class
 *
 * The Simulate class contains objects to represent the FetchStage, DecodeStage,
 * ExecuteStage, MemoryStage, and Writeback Stages. These classes contain the
 * methods to simulate the combinational logic performed by the PIPE machine.
 * In addition, the Simulate class contains F, D, E, M, and W objects
 * to represent the pipelined registers that provide the input and
 * accept the output of these stages.
*/

#include <iomanip>
#include <iostream>

using namespace std;

#include "PipeReg.h"
#include "PipeRegister.h"
#include "F.h"
#include "D.h"
#include "E.h"
#include "M.h"
#include "W.h"
#include "Stage.h"
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include "DecodeStage.h"
#include "FetchStage.h"
#include "WritebackStage.h"
#include "Simulate.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"

/*
 * Simulate constructor
 *
 * creates instances of each data member
*/
Simulate::Simulate()
{
    /* PIPE stages */
    stages = new Stage * [NUMSTAGES];
    stages[FSTAGE] = new FetchStage();
    stages[DSTAGE] = new DecodeStage();
    stages[ESTAGE] = new ExecuteStage();
    stages[MSTAGE] = new MemoryStage();
    stages[WSTAGE] = new WritebackStage();

    piperegs = new PipeRegister * [NUMPIPEREGS];

    /* pipelined registers */
    piperegs[FREG] = new F();
    piperegs[DREG] = new D();
    piperegs[EREG] = new E();
    piperegs[MREG] = new M();
    piperegs[WREG] = new W();
}

/*
 * run
 *
 * Simulate the stages of the PIPE machine until a halt is executed.
*/
void Simulate::run()
{
    int cycle = 0;
    Memory * mem = Memory::getInstance();
    ConditionCodes * cc = ConditionCodes::getInstance();
    RegisterFile * rf = RegisterFile::getInstance();
    bool stop = false;

    while (!stop)
    {
        stop = doClockLow();
        doClockHigh();

        /* dump the values of the pipelined registers, Condition Codes, */
        /* Register File, and Memory */
        cout << "\nAt end of cycle " << dec << cycle << ":" << endl;
        dumpPipeRegs();
        cc->dump();
        rf->dump();
        mem->dump();
        cycle++;
    }
}

/*
 * doClockLow
 *
 * When the clock is low, the stages use their inputs
 * to calculate the outputs.
*/
bool Simulate::doClockLow()
{
    bool stop;

    //going through the stages in reverse order helps to
    //simulate the parallel behavior of the hardware
    stop = stages[WSTAGE]->doClockLow(piperegs, stages);
    stages[MSTAGE]->doClockLow(piperegs, stages);
    stages[ESTAGE]->doClockLow(piperegs, stages);
    stages[DSTAGE]->doClockLow(piperegs, stages);
    stages[FSTAGE]->doClockLow(piperegs, stages);
    return stop;
}

/*
 * doClockHigh
 *
 * When the clock rises, the pipelined registers are updated.
 * The register file is also updated.
 * The code to update the pipelined registers is in the
 * pipe stage class.
*/
void Simulate::doClockHigh()
{
    //get the WritebackStage to update the register file
    stages[WSTAGE]->doClockHigh(piperegs);

    //get the MemoryStage to update the W register
    stages[MSTAGE]->doClockHigh(piperegs);

    //get the ExecuteStage to update the M register
    stages[ESTAGE]->doClockHigh(piperegs);

    //get the DecodeStage to update the E register
    stages[DSTAGE]->doClockHigh(piperegs);

    //get the FetchStage to update the F and D registers
    stages[FSTAGE]->doClockHigh(piperegs);
}

/*
 * dumpPipeRegs
 *
 * Display the contents of the pipelined registers
*/
void Simulate::dumpPipeRegs()
{
    piperegs[FREG]->dump();
    piperegs[DREG]->dump();
    piperegs[EREG]->dump();
    piperegs[MREG]->dump();
    piperegs[WREG]->dump();
}
