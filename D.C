#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>

using namespace std;
#include "PipeRegister.h"
#include "Instructions.h"
#include "RegisterFile.h"
#include "PipeReg.h"
#include "D.h"

/*
 * D constructor
 *
 * initialize the D pipelined register
 */
D::D()
{
    stat = new PipeReg();
    icode = new PipeReg(INOP);
    ifun = new PipeReg(FNONE);
    rA = new PipeReg(RNONE);
    rB = new PipeReg(RNONE);
    valC = new PipeReg();
    valP = new PipeReg();
}

/* return the stat pipelined register */
PipeReg * D::getstat()
{
    return stat;
}

/* return the icode pipelined register */
PipeReg * D::geticode()
{
    return icode;
}

/* return the ifun pipelined register */
PipeReg * D::getifun()
{
    return ifun;
}

/* return the rA pipelined register */
PipeReg * D::getrA()
{
    return rA;
}

/* return the rB pipelined register */
PipeReg * D::getrB()
{
    return rB;
}

/* return the valC pipelined register */
PipeReg * D::getvalC()
{
    return valC;
}

/* return the valP pipelined register */
PipeReg * D::getvalP()
{
    return valP;
}

/*
 * dump
 *
 * outputs the current values of the D pipeline register
*/
void D::dump()
{
    cout << "D: " << "stat: " << hex << setw(1) << setfill('0') << stat->getOutput()
         << " icode: " << hex << setw(1) << setfill('0') << icode->getOutput()
         << " ifun: " << hex << setw(1) << setfill('0') << ifun->getOutput()
         << " rA: " << hex << setw(1) << setfill('0') << rA->getOutput()
         << " rB: " << hex << setw(1) << setfill('0') << rB->getOutput()
         << " valC: " << hex << setw(16) << setfill('0') << valC->getOutput()
         << " valP: " << hex << setw(3) << setfill('0') << valP->getOutput() << endl;

}
