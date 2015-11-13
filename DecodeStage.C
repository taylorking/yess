
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
#include "ExecuteStage.h"
#include "MemoryStage.h"
#include "DecodeStage.h"

bool DecodeStage::doClockLow(PipeRegister ** pregs, Stage ** stages) {
    D * dreg = (D*)pregs[DREG];
    E * ereg = (E*)pregs[EREG];
    W * wreg = (W*)pregs[WREG];
    M * mreg = (M*)pregs[MREG];
    ExecuteStage * e_stage = (ExecuteStage*)stages[ESTAGE];
    MemoryStage * m_stage = (MemoryStage*)stages[MSTAGE];
    uint64_t stat = dreg->getstat()->getOutput();
    uint64_t icode = dreg->geticode()->getOutput();
    uint64_t ifun = dreg->getifun()->getOutput();
    uint64_t valC = dreg->getvalC()->getOutput();
    uint64_t dstE = d_dstE(icode, dreg->getrB());
    uint64_t dstM = d_dstM(icode, dreg->getrA());
    uint64_t srcA = d_srcA(icode, dreg->getrA());
    uint64_t srcB = d_srcB(icode, dreg->getrB());
    uint64_t valA = d_valA(e_stage, m_stage, mreg, wreg, srcA);
    uint64_t valB = d_valB(e_stage, m_stage, mreg, wreg, srcB);
    setEInput(ereg, stat, icode, ifun, valC, valA, valB, dstE, dstM, srcA, srcB);
    return false;
}

void DecodeStage::doClockHigh(PipeRegister ** pregs) {
    E * ereg = (E*)pregs[EREG];
    ereg->getstat()->normal();
    ereg->geticode()->normal();
    ereg->getifun()->normal();
    ereg->getvalC()->normal();
    ereg->getvalA()->normal();
    ereg->getvalB()->normal();
    ereg->getdstE()->normal();
    ereg->getdstM()->normal();
    ereg->getsrcA()->normal();
    ereg->getsrcB()->normal();
}
// Set the value of the E pipe Register
void DecodeStage::setEInput(E * ereg, uint64_t stat, uint64_t icode, uint64_t ifun,
                            uint64_t valC, uint64_t valA, uint64_t valB, uint64_t dstE, uint64_t dstM, uint64_t srcA, uint64_t srcB)
{
    ereg->getstat()->setInput(stat);
    ereg->geticode()->setInput(icode);
    ereg->getifun()->setInput(ifun);
    ereg->getvalC()->setInput(valC);
    ereg->getvalA()->setInput(valA);
    ereg->getvalB()->setInput(valB);
    ereg->getdstE()->setInput(dstE);
    ereg->getdstM()->setInput(dstM);
    ereg->getsrcA()->setInput(srcA);
    ereg->getsrcB()->setInput(srcB);
}

// Get srcA
uint64_t DecodeStage::d_srcA(uint64_t icode, PipeReg * rA) {
    if(icode == IRRMOVQ || icode == IRMMOVQ || icode == IOPQ || icode == IPUSHQ) {
        return rA->getOutput();
    }

    if(icode == IPOPQ || icode == IRET) {
        return RSP;
    }

    return RNONE;
}
// Get srcB
uint64_t DecodeStage::d_srcB(uint64_t icode, PipeReg * rB) {
    if(icode == IOPQ || icode == IRMMOVQ || icode == IMRMOVQ) {
        return rB->getOutput();
    }

    if(icode == IPUSHQ || icode == IPOPQ || icode == ICALL || icode == IRET) {
        return RSP;
    }

    return RNONE;
}

// Get dstE
uint64_t DecodeStage::d_dstE(uint64_t icode, PipeReg * rB) {
    if(icode == IRRMOVQ || icode == IIRMOVQ || icode ==  IOPQ) {
        return rB->getOutput();
    }

    if(icode == IPUSHQ || icode == IPOPQ || icode == ICALL || icode == IRET) {
        return RSP;
    }

    return RNONE;
}
uint64_t DecodeStage::d_dstM(uint64_t icode, PipeReg * rA) {
    if(icode == IMRMOVQ || icode == IPOPQ) {
        return rA->getOutput();
    }

    return RNONE;
}
// Get valA
uint64_t DecodeStage::d_valA(ExecuteStage * e_stage, MemoryStage * m_stage, M * m_reg, W * w_reg, uint64_t d_srcA) {
    uint64_t e_dstE = e_stage->get_dstE();
    uint64_t M_dstE = m_reg->getdstE()->getOutput();
    uint64_t M_dstM = m_reg->getdstM()->getOutput();
    uint64_t W_dstE = w_reg->getdstE()->getOutput();
    uint64_t W_dstM = w_reg->getdstM()->getOutput();
    if(d_srcA == RNONE) {
        return 0;
    }
    // If the src is the same as e_dstE, we're gonna forward
    if (d_srcA == e_dstE) {
        return e_stage->get_valE();
    }

    // if the source is the same as M_dstE we're gonna forward
    if (d_srcA == M_dstM) {
        return m_stage->get_valM();
    }
    if (d_srcA == M_dstE) {
        return m_reg->getvalE()->getOutput();
    }

    if (d_srcA == W_dstE) {
        return w_reg->getvalE()->getOutput();
    }
    if (d_srcA == W_dstM) {
        return w_reg->getvalM()->getOutput();
    }

    // if we fall through, we're just gonna do a normal read from register
    RegisterFile * reg = RegisterFile::getInstance();
    return reg->readRegister(d_srcA);
}

uint64_t DecodeStage::d_valB(ExecuteStage * e_stage, MemoryStage * m_stage, M * m_reg, W * w_reg, uint64_t d_srcB) {
    uint64_t e_dstE = e_stage->get_dstE();
    uint64_t M_dstE = m_reg->getdstE()->getOutput();
    uint64_t M_dstM = m_reg->getdstM()->getOutput();
    uint64_t W_dstE = w_reg->getdstE()->getOutput();
    uint64_t W_dstM = w_reg->getdstM()->getOutput();
    if(d_srcB == RNONE) {
        return 0;
    }

    if (d_srcB == e_dstE) {
        return e_stage->get_valE();
    }

    if (d_srcB == M_dstE) {
        return m_reg->getvalE()->getOutput();
    }

    if (d_srcB == M_dstM) {
        return m_stage->get_valM();
    }
    if (d_srcB == W_dstE) {
        return w_reg->getvalE()->getOutput();
    }
    if (d_srcB == W_dstM) {
        return w_reg->getvalM()->getOutput();
    }

    RegisterFile * reg = RegisterFile::getInstance();
    return reg->readRegister(d_srcB);
}
