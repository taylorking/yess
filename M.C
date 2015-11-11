#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>

using namespace std;
#include "PipeRegister.h"
#include "RegisterFile.h"
#include "Instructions.h"
#include "PipeReg.h"
#include "M.h"
/*
 * M constructor
 *
 * initialize the M pipelined register
*/
M::M()
{
    stat = new PipeReg();
    icode = new PipeReg(INOP);
    Cnd = new PipeReg();
    valE = new PipeReg();
    valA = new PipeReg();
    dstE = new PipeReg(RNONE);
    dstM = new PipeReg(RNONE);
}

/* return the stat pipelined register */
PipeReg * M::getstat()
{
    return stat;
}

/* return the icode pipelined register */
PipeReg * M::geticode()
{
    return icode;
}

/* return the Cnd pipelined register */
PipeReg * M::getCnd()
{
    return Cnd;
}

/* return the valE pipelined register */
PipeReg * M::getvalE()
{
    return valE;
}

/* return the valA pipelined register */
PipeReg * M::getvalA()
{
    return valA;
}

/* return the dstE pipelined register */
PipeReg * M::getdstE()
{
    return dstE;
}

/* return the dstM pipelined register */
PipeReg * M::getdstM()
{
    return dstM;
}

/*
 * dump
 *
 * outputs the current values of the M pipeline register
*/
void M:: dump()
{
    cout << "M: stat: " << hex << setw(1) << setfill('0') << stat->getOutput()
         << " icode: " << hex << setw(1) << setfill('0') << icode->getOutput()
         << " Cnd: " << hex << setw(1) << setfill('0') << Cnd->getOutput()
         << " valE: " << hex << setw(16) << setfill('0') << valE->getOutput()
         << " valA: " << hex << setw(16) << setfill('0') << valA->getOutput()
         << " dstE: " << hex << setw(1) << setfill('0') << dstE->getOutput()
         << " dstM: " << hex << setw(1) << setfill('0') << dstM->getOutput() << endl;
}
