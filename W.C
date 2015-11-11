#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>

using namespace std;
#include "PipeRegister.h"
#include "RegisterFile.h"
#include "Instructions.h"
#include "PipeReg.h"
#include "W.h"

/*
 * W constructor
 *
 * initialize the W pipelined register
*/
W::W()
{
    stat = new PipeReg();
    icode = new PipeReg(INOP);
    valE = new PipeReg();
    valM = new PipeReg();
    dstE = new PipeReg(RNONE);
    dstM = new PipeReg(RNONE);
}

/* return the stat pipelined register */
PipeReg * W::getstat()
{
    return stat;
}

/* return the icode pipelined register */
PipeReg * W::geticode()
{
    return icode;
}

/* return the valE pipelined register */
PipeReg * W::getvalE()
{
    return valE;
}

/* return the valM pipelined register */
PipeReg * W::getvalM()
{
    return valM;
}

/* return the dstE pipelined register */
PipeReg * W::getdstE()
{
    return dstE;
}

/* return the dstM pipelined register */
PipeReg * W::getdstM()
{
    return dstM;
}

/*
 * dump
 *
 * outputs the current values of the W pipeline register
*/
void W:: dump()
{
    cout << "W: stat: " << hex << setw(1) << setfill('0') << stat->getOutput()
         << " icode: " << hex << setw(1) << setfill('0') << icode->getOutput()
         << " valE: " << hex << setw(16) << setfill('0') << valE->getOutput()
         << " valM: " << hex << setw(16) << setfill('0') << valM->getOutput()
         << " dstE: " << hex << setw(1) << setfill('0') << dstE->getOutput()
         << " dstM: " << hex << setw(1) << setfill('0') << dstM->getOutput() << endl;
}

