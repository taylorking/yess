#include <stdint.h>
#include "PipeReg.h"
#include "PipeRegister.h"
#include "Stage.h"
#include "Tools.h"
#include "Instructions.h"
#include "F.h"
#include "D.h"
#include "E.h"
#include "M.h"
#include "W.h"
#include "ExecuteStage.h"
#include "RegisterFile.h"
#include "ConditionCodes.h"

void ExecuteStage::doClockHigh(PipeRegister ** pregs) {
    M * mreg = (M*)pregs[MREG];
    mreg->getstat()->normal();
    mreg->geticode()->normal();
    mreg->getCnd()->normal();
    mreg->getvalE()->normal();
    mreg->getvalA()->normal();
    mreg->getdstE()->normal();
    mreg->getdstM()->normal();
}

bool ExecuteStage::doClockLow(PipeRegister ** pregs, Stage ** stages) {
    M * mreg = (M*)pregs[MREG];
    E * ereg = (E*)pregs[EREG];
    uint64_t stat = ereg->getstat()->getOutput();
    uint64_t icode = ereg->geticode()->getOutput();
    uint64_t ifun = ereg->getifun()->getOutput();
// Set these globally so they can be accessed outside the stage
    dstE = ereg->getdstE()->getOutput();
    uint64_t e_dstM = ereg->getdstM()->getOutput();
    uint64_t e_valA = ereg->getvalA()->getOutput();
    uint64_t e_valB = ereg->getvalB()->getOutput();
    uint64_t e_valC = ereg->getvalC()->getOutput();
    uint64_t e_alufun = alufun(icode, ifun);
    uint64_t e_aluA = aluA(icode, e_valA, e_valC);
    uint64_t e_aluB = aluB(icode, e_valB);
    valE = exec_alu(e_alufun, e_aluA, e_aluB, set_cc(icode));

    setMInput(mreg, stat, icode, valE, e_valA, dstE, e_dstM);
    return false;
}

void ExecuteStage::setMInput(M * mreg, uint64_t stat, uint64_t icode, uint64_t valE, uint64_t valA, uint64_t dstE, uint64_t dstM) {
    mreg->getstat()->setInput(stat);
    mreg->geticode()->setInput(icode);
    mreg->getvalE()->setInput(valE);
    mreg->getvalA()->setInput(valA);
    mreg->getdstE()->setInput(dstE);
    mreg->getdstM()->setInput(dstM);
}

// Get the ifun of the operation
uint64_t ExecuteStage::alufun(uint64_t e_icode, uint64_t e_ifun) {
    if(e_icode == IOPQ) {
        return e_ifun ;
    }

    return ADDQ;
}

// Do the ALU function
uint64_t ExecuteStage::exec_alu(uint64_t alufun, uint64_t aluA, uint64_t aluB, bool setcc) {
    bool overflow = false;
    uint64_t result;

    switch(alufun) {
    case ADDQ:
        overflow = Tools::addOverflow(aluA,aluB);
        result = aluA + aluB;
        break;

    case SUBQ:
        result = aluB - aluA;
        overflow = Tools::subOverflow(aluB, aluA);
        break;

    case XORQ:
        result = aluA ^ aluB;
        break;

    case ANDQ:
        result = aluA & aluB;
        break;
    }

    // Set the condition codes
    if(setcc) {
        ConditionCodes * codes = ConditionCodes::getInstance();
        codes->setConditionCode((result == 0), ZF);
        codes->setConditionCode(Tools::sign(result), SF);
        codes->setConditionCode(overflow, OF);
    }

    return result;
}

// Get the value for aluA
uint64_t ExecuteStage::aluA(uint64_t e_icode, uint64_t e_valA, uint64_t e_valC) {
    if(e_icode == IRRMOVQ || e_icode == IOPQ) {
        return e_valA;
    }

    if(e_icode == IIRMOVQ || e_icode == IRMMOVQ || e_icode == IMRMOVQ) {
        return e_valC;
    }

    // I'm returning an unsigned int on this one and it's a -8. Will change in the future as this gets implemented.
    if(e_icode == ICALL || e_icode == IPUSHQ) {
        return -8;
    }

    if(e_icode == IRET || e_icode == IPOPQ) {
        return 8;
    }

    return 0;
}
// Get the value for aluB?
uint64_t ExecuteStage::aluB(uint64_t e_icode, uint64_t e_valB) {
    if(e_icode == IRMMOVQ || e_icode == IMRMOVQ || e_icode == IOPQ || e_icode == IPUSHQ || e_icode == IRET || e_icode == IPOPQ) {
        return e_valB;
    }

    if(e_icode == IRRMOVQ || e_icode == IIRMOVQ) {
        return 0;
    }

    return 0;
}
// Update the condition codes ?
bool ExecuteStage::set_cc(uint64_t e_icode) {
    return (e_icode == IOPQ);
}
// Do we have a destE register?
uint64_t ExecuteStage::e_dstE(uint64_t e_icode, uint64_t e_dstE, uint64_t e_cnd) {
    if(e_icode == IRRMOVQ && !e_cnd) {
        return RNONE;
    }

    return e_dstE;
}
// Return the global members so the decode stage can get these values for forwarding.
uint64_t ExecuteStage::get_dstE() {
    return dstE;
}
uint64_t ExecuteStage::get_valE() {
    return valE;
}
