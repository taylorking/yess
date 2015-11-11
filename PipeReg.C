#include <cstdint>
#include "PipeReg.h"

/*
 * PipeReg constructor
 *
 * initialize the PipeReg
 * default value is 0 if no parameter is given
*/
PipeReg::PipeReg(int state)
{
    input = 0;
    this->state = state;
}

/*
 * setInput
 *
 * @param value to be stored in input data field
 *
*/
void PipeReg::setInput(uint64_t input)
{
    this->input = input;
}

/*
 * getOutput
 *
 * @return the state data field
*/
uint64_t PipeReg::getOutput()
{
    return state;
}

/*
 * normal
 *
 * simulates the normal control signal applied
 * to a pipelined register by setting state
 * to the input
*/
void PipeReg::normal()
{
    state = input;
}

