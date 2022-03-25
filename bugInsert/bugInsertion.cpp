//=============================================================================
// FILE:
//    markBBL.cpp
//
// DESCRIPTION:
//    Instrument a mark function at the start and the end of a BBL
//    to mark a BBL's position in a function.
//    Implement use Leggacy Pass Manager
//    
// USAGE:
//      opt -load libMarkBBL.so -bugInsertion <input-llvm-file>
//      or
//      clang -Xclang -load -Xclang libBugInsertion.so <input-c-file> libinsert.a  
//
// ACKNOWLEDGE: llvm-tutor
//=============================================================================
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <cstring>
#include <cstdio>

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Pass.h"
#include "bug.h" 

using namespace llvm;


//-----------------------------------------------------------------------------
// markBBL implementation
//-----------------------------------------------------------------------------
namespace {
  typedef struct{
    std::string function;
    int block;
    int pos;
    int step;
    int chunk;
    int loop;
    int p;
    int q;
  } Bug;

  std::map<std::string, std::map<int, Bug> > function_list;


  void insertStack(Function &F, std::map<int, Bug> & block_list) {
    //get the context
    LLVMContext &context = F.getParent()->getContext();
    int i=0;
    for (BasicBlock& bb : F)
    {
      // here is nearly same as in the clang compiler which process the inline assembely
      if(block_list.find(i) != block_list.end()){
        Bug bug = block_list.find(i)->second;
        char myasm[1024];
        switch(bug.pos){
          case 1 : sprintf(myasm, bofRegAsm, "rax", bug.step, bug.chunk, bug.loop, bug.p, bug.q);
            break;
          case 2 : sprintf(myasm, bofRegAsm, "rbx", bug.step, bug.chunk, bug.loop, bug.p, bug.q);
            break;
          case 3 : sprintf(myasm, bofRegAsm, "rcx", bug.step, bug.chunk, bug.loop, bug.p, bug.q);
            break;
          case 4 : sprintf(myasm, bofRegAsm, "rdx", bug.step, bug.chunk, bug.loop, bug.p, bug.q);
            break;
          default : sprintf(myasm, bofStackAsm, bug.pos-5, bug.step, bug.chunk, bug.loop, bug.p, bug.q);
            break;
        }
        char myconstraint[256]="i1";
        FunctionType *VoidFunTy = FunctionType::get(Type::getVoidTy(context), {}, false);
        CallInst *Result = CallInst::Create(InlineAsm::get(VoidFunTy, myasm, myconstraint, true));
        BasicBlock& insertTarget = bb;
        Result->insertBefore(dyn_cast<Instruction>(insertTarget.begin()));
      }
      i++;
    }
  }


  // This method implements what the pass does
  void visitor(Function &F) {
      if(function_list.find(F.getName().str()) != function_list.end()){
        insertStack(F, function_list.find(F.getName().str())->second);
      }
  }


  // Legacy PM implementation
  struct BugInsertion : public FunctionPass {
    static char ID;
    BugInsertion() : FunctionPass(ID) {
      char buf[50];
      std::ifstream file;
      file.open("../data/bugInfo.txt");
      Bug bug;
      if(file.is_open()){
        while(file >> bug.function >> bug.block >> bug.pos>> bug.step>> bug.chunk>> bug.loop>> bug.p>> bug.q){
          if(function_list.find(bug.function) != function_list.end()){
            function_list.find(bug.function)->second[bug.block] = bug;
          }
          else{
            std::map<int, Bug> block_list;
            block_list[bug.block] = bug;
            function_list[bug.function] = block_list;
          }
        }
      }
    }
    // Main entry point - the name conveys what unit of IR this is to be run on.
    bool runOnFunction(Function &F) override {
      visitor(F);
      // Doesn't modify the input unit of IR, hence 'false'
      return false;
    }
  };

} // namespace


//-----------------------------------------------------------------------------
// Legacy PM Registration
//-----------------------------------------------------------------------------
// The address of this variable is used to uniquely identify the pass. The
// actual value doesn't matter.
char BugInsertion::ID = 0;

// This is the core interface for pass plugins. It guarantees that 'opt' will
// recognize LegacyHelloWorld when added to the pass pipeline on the command
// line, i.e.  via '--legacy-hello-world'
static RegisterPass<BugInsertion>
    X("bugInsertion", "Insert Bug Pass",
      true, // This pass doesn't modify the CFG => true
      false // This pass is not a pure analysis pass => false
    );

// register pass to clang
static void registerMyPass(const PassManagerBuilder &,
                           llvm::legacy::PassManagerBase &PM) {
  PM.add(new BugInsertion());
}
static RegisterStandardPasses
    RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                   registerMyPass);