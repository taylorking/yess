#include <stdint.h>
#include "PipeReg.h"
#include "PipeRegister.h"
#include "RegisterFile.h"
#include "Stage.h"
#include "F.h"
#include "D.h"
#include "E.h"
#include "M.h"
#include "W.h"
#include "Instructions.h"
#include "WritebackStage.h"

bool WritebackStage::doClockLow(PipeRegister ** pregs, Stage ** stages) {
    W * wreg = (W*)pregs[WREG];
    uint64_t icode = wreg->geticode()->getOutput();

    if(icode == IHALT) {
        return true;
    }

    uint64_t dstE = wreg->getdstE()->getOutput();
    RegisterFile * reg = RegisterFile::getInstance();
    uint64_t valE = wreg->getvalE()->getOutput();
    uint64_t valM = wreg->getvalM()->getOutput();
    uint64_t dstM = wreg->getdstM()->getOutput();
    reg->writeRegister(valE, dstE);
    if(mem_read(icode)) {
      reg->writeRegister(valM, dstM);
    }
    return false;
}


bool WritebackStage::mem_read(uint64_t icode) {
  return (icode == IMRMOVQ || icode == IPOPQ || icode == IRET);
}
void WritebackStage::doClockHigh(PipeRegister ** pregs) {

}

