#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>

using namespace std;
#include "PipeRegister.h"
#include "RegisterFile.h"
#include "Instructions.h"
#include "PipeReg.h"
#include "E.h"
/*
 *  E constructor
 *
 *  initialize the E pipelined register
 *
*/
E::E()
{
    stat = new PipeReg();
    icode = new PipeReg(INOP);
    ifun = new PipeReg(FNONE);
    valC = new PipeReg();
    valA = new PipeReg();
    valB = new PipeReg();
    dstE = new PipeReg(RNONE);
    dstM = new PipeReg(RNONE);
    srcA = new PipeReg();
    srcB = new PipeReg();
}

/* return the stat pipelined register */
PipeReg * E::getstat()
{
    return stat;
}

/* return the icode pipelined register */
PipeReg * E::geticode()
{
    return icode;
}


/* return the ifun pipelined register */
PipeReg * E::getifun()
{
    return ifun;
}

/* return the valC pipelined register */
PipeReg * E::getvalC()
{
    return valC;
}

/* return the valA pipelined register */
PipeReg * E::getvalA()
{
    return valA;
}

/* return the valB pipelined register */
PipeReg * E::getvalB()
{
    return valB;
}

/* return the dstE pipelined register */
PipeReg * E::getdstE()
{
    return dstE;
}

/* return the dstM pipelined register */
PipeReg * E::getdstM()
{
    return dstM;
}

/* return the srcA pipelined register */
PipeReg * E::getsrcA()
{
    return srcA;
}

/* return the srcB pipelined register */
PipeReg * E::getsrcB()
{
    return srcB;
}

/*
 * dump
 *
 * outputs the current values of the E pipeline register
*/
void E::dump()
{
    cout << "E: stat: " << hex << setw(1) << setfill('0') << stat->getOutput()
         << " icode: " << hex << setw(1) << setfill('0') << icode->getOutput()
         << " ifun: " << hex << setw(1) << setfill('0') << ifun->getOutput()
         << " valC: " << hex << setw(16) << setfill('0') << valC->getOutput()
         << " valA: " << hex << setw(16) << setfill('0') << valA->getOutput() << endl
         << "E: valB: " << hex << setw(16) << setfill('0') << valB->getOutput()
         << " dstE: " << hex << setw(1) << setfill('0') << dstE->getOutput()
         << " dstM: " << hex << setw(1) << setfill('0') << dstM->getOutput()
         << " srcA: " << hex << setw(1) << setfill('0') << srcA->getOutput()
         << " srcB: " << hex << setw(1) << setfill('0') << srcB->getOutput() << endl;
}
