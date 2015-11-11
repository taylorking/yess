#include <cstdint>
#include "RegisterFile.h"
#include "PipeReg.h"
#include "PipeRegister.h"
#include "Stage.h"
#include "F.h"
#include "D.h"
#include "E.h"
#include "M.h"
#include "W.h"
#include "Instructions.h"
#include "Memory.h"
#include "Tools.h"
#include "FetchStage.h"
using namespace std;
bool FetchStage::doClockLow(PipeRegister ** pregs, Stage ** stages)
{
    F * freg = (F*)pregs[FREG];
    D * dreg = (D*)pregs[DREG];
    M * mreg = (M*)pregs[MREG];
    W * wreg = (W*)pregs[WREG];
    uint64_t icode = 0, stat = 0, ifun = 0, valC = 0, valP = 0;
    uint64_t rA = RNONE, rB = RNONE;
    // Call select PC to get f_pc
    int32_t f_pc = selectPC(freg, mreg, wreg);
    Memory * insMem = Memory::getInstance();
    // pass that to PC Increment
    bool error;
    // This byte is the icode and the ifun
    uint8_t instructionByte = insMem->getByte(f_pc, error);
    // seperate the bytes respective pieces
    icode = Tools::getBits(instructionByte, 4, 7);
    ifun = Tools::getBits(instructionByte, 0, 3);

    if(needsRegIds(icode)) {
        getRegIds(f_pc, &rA, &rB);
    }

    if(needsValC(icode)) {
        valC = buildValC(f_pc, needsRegIds(icode));
    }

    valP = PCincrement(f_pc, needsRegIds(icode), needsValC(icode));

    // pass that to predictPC
    uint64_t predPC = predictPC(icode, valC, valP);
    //after you write PredictPC, the value passed to setInput below
    //will be the value returned by PredictPC
    //set predPc in the pipeline register
    freg->getpredPC()->setInput(predPC);

    setDInput(dreg, stat, icode, ifun, rA, rB, valC, valP);
    return false;
}

void FetchStage::doClockHigh(PipeRegister ** pregs)
{
    F * freg = (F*)pregs[FREG];
    D * dreg = (D*)pregs[DREG];
    freg->getpredPC()->normal();
    dreg->getstat()->normal();
    dreg->geticode()->normal();
    dreg->getifun()->normal();
    dreg->getrA()->normal();
    dreg->getrB()->normal();
    dreg->getvalC()->normal();
    dreg->getvalP()->normal();
}

void FetchStage::setDInput(D * dreg, uint64_t stat, uint64_t icode,
                           uint64_t ifun, uint64_t rA, uint64_t rB,
                           uint64_t valC, uint64_t valP)
{
    dreg->getstat()->setInput(stat);
    dreg->geticode()->setInput(icode);
    dreg->getifun()->setInput(ifun);
    dreg->getrA()->setInput(rA);
    dreg->getrB()->setInput(rB);
    dreg->getvalC()->setInput(valC);
    dreg->getvalP()->setInput(valP);
}

uint64_t FetchStage::PCincrement(int32_t f_pc, bool needsRegIds, bool needsValC) {
    if(needsRegIds) {
        f_pc += 1;
    }

    if(needsValC) {
        f_pc += 8;
    }

    f_pc += 1;
    return f_pc;
}

uint64_t FetchStage::predictPC(uint64_t icode, uint64_t valC, uint64_t valP) {
    if(icode == IJXX || icode == ICALL) {
        return valC;
    }

    return valP;
}

uint64_t FetchStage::selectPC(F * freg, M * mreg, W * wreg) {
    uint64_t m_icode = mreg->geticode()->getOutput();
    uint64_t mCnd = mreg->getCnd()->getOutput();
    uint64_t w_icode = wreg->geticode()->getOutput();

    if(m_icode == IJXX && !mCnd) {
        return mreg->getvalA()->getOutput();
    }

    if(w_icode == IRET) {
        return wreg->getvalM()->getOutput();
    }

    return freg->getpredPC()->getOutput();
}
void FetchStage::getRegIds(int32_t f_pc, uint64_t * rA, uint64_t * rB) {
    Memory * mem = Memory::getInstance();
    bool error;
    // Get the register byte out of memory the register byte comes after the instruction code byte
    uint64_t rval = mem->getByte(f_pc + 1, error);
    // Split it into register a and register b
    uint8_t rAval = Tools::getBits(rval, 4, 7);
    uint8_t rBval = Tools::getBits(rval, 0, 3);
    // Dereference the pointers and change the values of rA and rB
    *rA = rAval;
    *rB = rBval;
}

uint64_t FetchStage::buildValC(int32_t f_pc, bool needsRegId) {
    Memory * mem = Memory::getInstance();
    int startOffset = f_pc + 1;

    if(needsRegId) {
        startOffset+=1;
    }

    bool error;
    uint8_t valC[8];

    for(int i = 0; i < 8; i++) {
        valC[i] = mem->getByte(startOffset + i, error);
    }

    return Tools::buildLong(valC);
}

bool FetchStage::needsRegIds(uint64_t icode) {
    return (icode == IRRMOVQ || icode == IOPQ || icode == IPUSHQ || icode == IPOPQ || icode == IIRMOVQ || icode == IRMMOVQ || icode == IMRMOVQ);
}
bool FetchStage::needsValC(uint64_t icode) {
    return (icode == IIRMOVQ || icode == IRMMOVQ || icode == IMRMOVQ || icode == IJXX || icode == ICALL);
}
