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
//      opt -load libMarkBBL.so -markBBL <input-llvm-file>
//      or
//      clang -Xclang -load -Xclang libMarkBBL.so <input-c-file> libinsert.a  
//
// ACKNOWLEDGE: llvm-tutor
//=============================================================================
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <string>
#include <unistd.h>

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Pass.h" 

using namespace llvm;


//-----------------------------------------------------------------------------
// markBBL implementation
//-----------------------------------------------------------------------------
namespace {
  std::unordered_set<std::string> function_list;


  void insertStack(Function &F) {
    //get the markBBL function
    LLVMContext &context = F.getParent()->getContext();
    std::vector<Type*> num(1, Type::getInt32Ty(context));
    FunctionType *type = FunctionType::get(Type::getVoidTy(context), num, false);
    Value *srcFun = F.getParent()->getOrInsertFunction("markBBL", type).getCallee();

    //insert before and after each BBL
    if (Function *fun = dyn_cast<Function>(srcFun)) {
      int i=0;
      for (BasicBlock& bb : F)
      {
        Instruction *beginInst = dyn_cast<Instruction>(bb.begin());
        Instruction *endInst = dyn_cast<Instruction>(std::prev(bb.end()));
        Value *blocNum = ConstantInt::get(context, APInt(32, i));
        CallInst *instS = CallInst::Create(fun, blocNum);
        instS->insertBefore(beginInst);
        CallInst *instE = CallInst::Create(fun, blocNum);
        instE->insertBefore(endInst);
        i++;
      }
    }
  }


  // This method implements what the pass does
  void visitor(Function &F) {
      if(function_list.find(F.getName().str()) != function_list.end()){
        insertStack(F);
      }
  }


  // Legacy PM implementation
  struct MarkBBL : public FunctionPass {
    static char ID;
    std::ifstream file;
    MarkBBL() : FunctionPass(ID) {
      char buf[50];
      file.open("../data/functions.txt");
      std::string temp;
      if(file.is_open()){
        while(file >> temp){
          function_list.insert(temp);
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
char MarkBBL::ID = 0;

// This is the core interface for pass plugins. It guarantees that 'opt' will
// recognize LegacyHelloWorld when added to the pass pipeline on the command
// line, i.e.  via '--legacy-hello-world'
static RegisterPass<MarkBBL>
    X("markBBL", "Mark BBL Pass",
      true, // This pass doesn't modify the CFG => true
      false // This pass is not a pure analysis pass => false
    );

// register pass to clang
static void registerMyPass(const PassManagerBuilder &,
                           llvm::legacy::PassManagerBase &PM) {
    PM.add(new MarkBBL());
}
static RegisterStandardPasses
    RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                   registerMyPass);