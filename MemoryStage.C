#include <stdint.h>
#include "PipeReg.h"
#include "PipeRegister.h"
#include "Stage.h"
#include "F.h"
#include "D.h"
#include "E.h"
#include "M.h"
#include "W.h"
#include "ExecuteStage.h"
#include "MemoryStage.h"

void MemoryStage::doClockHigh(PipeRegister ** pregs) {
    W * wreg = (W*)pregs[WREG];
    wreg->getstat()->normal();
    wreg->geticode()->normal();
    wreg->getvalE()->normal();
    wreg->getvalM()->normal();
    wreg->getdstE()->normal();
    wreg->getdstM()->normal();
}
bool MemoryStage::doClockLow(PipeRegister ** pregs, Stage ** stages) {
    M * mreg = (M*)pregs[MREG];
    W * wreg = (W*)pregs[WREG];
    uint64_t stat = mreg->getstat()->getOutput();
    uint64_t icode = mreg->geticode()->getOutput();
    uint64_t valE = mreg->getvalE()->getOutput();
    uint64_t dstE = mreg->getdstE()->getOutput();
    uint64_t dstM = mreg->getdstM()->getOutput();
    setWInput(wreg, stat, icode, valE, 0, dstE, dstM);
    return false;
}

void MemoryStage::setWInput(W * wreg, uint64_t stat, uint64_t icode, uint64_t valE, uint64_t valM, uint64_t dstE, uint64_t dstM) {
    wreg->getstat()->setInput(stat);
    wreg->geticode()->setInput(icode);
    wreg->getvalE()->setInput(valE);
    wreg->getvalM()->setInput(valM);
    wreg->getdstE()->setInput(dstE);
    wreg->getdstM()->setInput(dstM);
}
