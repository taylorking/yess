CC = g++
CFLAGS = -g -c -Wall -std=c++0x 
OBJ = Tools.o RegisterFile.o Loader.o\
ConditionCodes.o Memory.o FetchStage.o Simulate.o yess.o W.o M.o F.o D.o E.o\
PipeReg.o FetchStage.o DecodeStage.o ExecuteStage.o MemoryStage.o WritebackStage.o
yess: $(OBJ)  
	$(CC) Tools.o Memory.o Loader.o ConditionCodes.o PipeReg.o RegisterFile.o F.o D.o E.o M.o W.o FetchStage.o DecodeStage.o ExecuteStage.o MemoryStage.o WritebackStage.o Simulate.o yess.o -o yess
.C.o:
	$(CC) $(CFLAGS) $< -o $@

Tools.o: Tools.h 
Memory.o: Memory.h Tools.h
Loader.o: Loader.C Loader.h Memory.h
ConditionCodes.o: ConditionCodes.h Tools.h
PipeReg.o: PipeReg.h
RegisterFile.o: RegisterFile.h Tools.h
F.o:PipeRegister.h Stage.h F.h Instructions.h PipeReg.h RegisterFile.h
D.o:PipeRegister.h Stage.h D.h Instructions.h PipeReg.h RegisterFile.h
E.o:PipeRegister.h Stage.h E.h Instructions.h PipeReg.h RegisterFile.h
M.o:PipeRegister.h Stage.h M.h Instructions.h PipeReg.h RegisterFile.h 
W.o:PipeRegister.h Stage.h W.h Instructions.h PipeReg.h RegisterFile.h
FetchStage.o: PipeRegister.h Stage.h FetchStage.h RegisterFile.h PipeReg.h Stage.h F.h D.h M.h W.h E.h Memory.h Tools.h 
DecodeStage.o: PipeRegister.h Stage.h DecodeStage.h MemoryStage.h ExecuteStage.h Instructions.h RegisterFile.h PipeReg.h F.h D.h M.h W.h E.h 
ExecuteStage.o: PipeRegister.h Stage.h RegisterFile.h PipeReg.h F.h D.h M.h W.h E.h Tools.h Instructions.h ConditionCodes.h ExecuteStage.h 
MemoryStage.o: PipeRegister.h Stage.h MemoryStage.h ExecuteStage.h RegisterFile.h PipeReg.h F.h D.h M.h W.h E.h 
WritebackStage.o: PipeRegister.h Stage.h WritebackStage.h RegisterFile.h Instructions.h PipeReg.h F.h D.h M.h W.h E.h 
Simulate.o: Simulate.h PipeReg.h PipeRegister.h Stage.h F.h D.h E.h M.h W.h FetchStage.h DecodeStage.h ExecuteStage.h MemoryStage.h WritebackStage.h Memory.h RegisterFile.h ConditionCodes.h 
yess.o: Debug.h Simulate.h PipeReg.h F.h D.h E.h M.h W.h FetchStage.h ExecuteStage.h MemoryStage.h WritebackStage.h Memory.h RegisterFile.h ConditionCodes.h 
#add the lines need to compile RegisterFile.C, Tools.C, ConditionCodes.C
#and Memory.C

clean:
	$(RM) count *.o
run:
	make clean
	make yess
	./run.sh

submit:
	make clean
	submit can 3481-lab8 *.C *.h makefile xorqfor.ys andqfor.ys subqfor.ys 
