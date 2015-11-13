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
#include "WritebackStage.h"
#include "Memory.h"
#include "Instructions.h"
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
    uint64_t dstE = mreg->getdstE()->getOutput();
    uint64_t dstM = mreg->getdstM()->getOutput();
    uint64_t valA = mreg->getvalA()->getOutput();
    uint64_t valE = mreg->getvalE()->getOutput();
    m_valM = 0;
    uint64_t m_addr = mem_addr(icode, valA, valE);
    bool error;
    if(mem_read(icode)) {
        Memory * mem = Memory::getInstance();
        m_valM = mem->getLong(m_addr, error);
    }
    if(mem_write(icode)) {
        Memory * mem = Memory::getInstance();
        mem->putLong(valA, m_addr, error);
    }
    setWInput(wreg, stat, icode, valE, m_valM, dstE, dstM);
    return false;
}

void MemoryStage::setWInput(W * wreg, uint64_t stat, uint64_t icode, uint64_t valE, uint64_t valM, uint64_t dstE, uint64_t dstM) {
    wreg->getstat()->setInput(stat);
    wreg->geticode()->setInput(icode);
    wreg->getvalE()->setInput(valE);
    wreg->getdstE()->setInput(dstE);
    wreg->getdstM()->setInput(dstM);
    wreg->getvalM()->setInput(valM);
}

uint64_t MemoryStage::mem_addr(uint64_t icode, uint64_t m_valA, uint64_t m_valE) {
    if(icode == IRMMOVQ || icode == IPUSHQ || icode == ICALL || icode == IMRMOVQ) {
        return m_valE;
    }
    if(icode == IPOPQ || icode == IRET) {
        return m_valA;
    }
    return 0;
}

uint64_t MemoryStage::get_valM() {
    return m_valM;
}

bool MemoryStage::mem_read(uint64_t icode) {
    return (icode == IMRMOVQ || icode == IPOPQ || icode == IRET);
}

bool MemoryStage::mem_write(uint64_t icode) {
    return (icode == IRMMOVQ || icode == IPUSHQ || icode == ICALL);
}
