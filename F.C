#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>

using namespace std;
#include "PipeRegister.h"
#include "PipeReg.h"
#include "F.h"


/*
 * F constructor
 *
 * initialize the F pipelined register
*/
F::F()
{
    predPC = new PipeReg();
}

/* return the predPC pipelined register */
PipeReg * F::getpredPC()
{
    return predPC;
}

/*
 * dump
 *
 * outputs the current values of the F pipeline register
*/
void F::dump()
{
    cout << "F: " << "predPC: " << hex << setw(3) << setfill('0')
         << predPC->getOutput() << endl;
}
