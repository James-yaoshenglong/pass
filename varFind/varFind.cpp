/*! @file
 *  This is an example of the PIN tool that demonstrates some basic PIN APIs 
 *  and could serve as the starting point for developing your first PIN tool
 */

#include "pin.H"
#include <cstdio>
#include <ios>
#include <iostream>
#include <fstream>
#include "sha256.h"
#include <stdio.h>
#include <vector>
using std::cerr;
using std::endl;
using std::string;

/* ================================================================== */
// Global variables
/* ================================================================== */

UINT64 insCount    = 0; //number of dynamically executed instructions
UINT64 bblCount    = 0; //number of dynamically executed basic blocks
UINT64 threadCount = 0; //total number of threads, including main thread
// ADDRINT targetAddr;
std::vector<ADDRINT> targetAddrs;

bool flag = true;

FILE* output;
std::istream* in;

/* ===================================================================== */
// Command line switches
/* ===================================================================== */
KNOB< string > KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "", "specify file name for MyPinTool output");

KNOB< BOOL > KnobCount(KNOB_MODE_WRITEONCE, "pintool", "count", "1",
                       "count instructions, basic blocks and threads in the application");

// KNOB< string > KnobAddr(KNOB_MODE_WRITEONCE, "pintool", "a", "", "specify the addr want to instrument");

KNOB< string > KnobInputFile(KNOB_MODE_WRITEONCE, "pintool", "i", "", "specify file name for MyPinTool input");

/* ===================================================================== */
// Utilities
/* ===================================================================== */

/*!
 *  Print out help message.
 */
INT32 Usage()
{
    cerr << "This tool prints out the number of dynamically executed " << endl
         << "instructions, basic blocks and threads in the application." << endl
         << endl;

    cerr << KNOB_BASE::StringKnobSummary() << endl;

    return -1;
}

/* ===================================================================== */
// Analysis routines
/* ===================================================================== */

/*!
 * Increase counter of the executed basic blocks and instructions.
 * This function is called for every basic block when it is about to be executed.
 * @param[in]   numInstInBbl    number of instructions in the basic block
 * @note use atomic operations for multi-threaded applications
 */



VOID getReg(ADDRINT rax, ADDRINT rbx, ADDRINT rcx, ADDRINT rdx, ADDRINT rsp)
{
    char arr[17];
    fprintf(output, "rax :0x%lx\n", rax);
    memcpy(arr, &rax, 8);
    arr[8] = '\0';
    getNullPosition(arr, 8, output);
    fprintf(output, "rbx :0x%lx\n", rbx);
    memcpy(arr, &rbx, 8);
    arr[8] = '\0';
    getNullPosition(arr, 8, output);
    fprintf(output, "rcx :0x%lx\n", rcx);
    memcpy(arr, &rcx, 8);
    arr[8] = '\0';
    getNullPosition(arr, 8, output);
    fprintf(output, "rdx :0x%lx\n", rdx);
    memcpy(arr, &rdx, 8);
    arr[8] = '\0';
    getNullPosition(arr, 8, output);
    // *out << std::hex <<"rsp :0x" << rsp <<endl;

    for(int step = 1; step<=16; step<<=1){
        for(int i=0; i<256; i+=step){
            // UINT8 value;
            PIN_SafeCopy(arr, (void*)(rsp+i), sizeof(UINT8)*step);
            // *out <<"[rsp+"<<i<<"] :0x" << std::hex << (int)value <<endl; //here c++ uint8 is char, so output should transfer it to int
            fprintf(output, "[rsp+ %d ] : with %d byte%d\n", i, step, (int)arr[0]);
            getNullPosition(arr, step, output);
        }
    }
    // *out << endl;
    fprintf(output, "\n");
}


/* ===================================================================== */
// Instrumentation callbacks
/* ===================================================================== */

/*!
 * Insert call to the CountBbl() analysis routine before every basic block 
 * of the trace.
 * This function is called every time a new trace is encountered.
 * @param[in]   trace    trace to be instrumented
 * @param[in]   v        value specified by the tool in the TRACE_AddInstrumentFunction
 *                       function call
 */
VOID Trace(TRACE trace, VOID* v)
{
    for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
    {
        // Insert a call to CountBbl() before every basic bloc, passing the number of instructions
        for (INS ins = BBL_InsHead(bbl); ins != BBL_InsTail(bbl); ins = INS_Next(ins)){
            // *out << std::hex <<INS_Address(ins) << endl;
            for(auto targetAddr : targetAddrs){
                if(INS_Address(ins) == targetAddr){
                    INS_InsertCall(BBL_InsTail(bbl), IPOINT_BEFORE, (AFUNPTR)getReg, 
                    IARG_REG_VALUE, LEVEL_BASE::REG_RAX, 
                    IARG_REG_VALUE, LEVEL_BASE::REG_RBX, 
                    IARG_REG_VALUE, LEVEL_BASE::REG_RCX,
                    IARG_REG_VALUE, LEVEL_BASE::REG_RDX,
                    IARG_REG_VALUE, LEVEL_BASE::REG_RSP, IARG_END);
                    cerr << "inserted" <<endl;
                    // *out << "inserted at 0x" << std::hex << targetAddr<<endl<<std::dec;
                    fprintf(output, "inserted at 0x%lx\n", targetAddr);
                    break;
                }
            }
        }
    }
}

/*!
 * Increase counter of threads in the application.
 * This function is called for every thread created by the application when it is
 * about to start running (including the root thread).
 * @param[in]   threadIndex     ID assigned by PIN to the new thread
 * @param[in]   ctxt            initial register state for the new thread
 * @param[in]   flags           thread creation flags (OS specific)
 * @param[in]   v               value specified by the tool in the 
 *                              PIN_AddThreadStartFunction function call
 */
VOID ThreadStart(THREADID threadIndex, CONTEXT* ctxt, INT32 flags, VOID* v) { threadCount++; }

VOID test(){

}

VOID Image(IMG img, VOID *v){
    RTN targetRTN = RTN_FindByName(img, "doSomething");
    // *out << "enter img insertion" <<endl;
    if(RTN_Valid(targetRTN)){
        // *out << RTN_Name(targetRTN) <<endl;
        RTN_Open(targetRTN);
        RTN_InsertCall(targetRTN, IPOINT_BEFORE, (AFUNPTR)test, IARG_END);
        RTN_Close(targetRTN);
    }
    // *out << "end img insertion" <<endl;
}






/*!
 * Print out analysis results.
 * This function is called when the application exits.
 * @param[in]   code            exit code of the application
 * @param[in]   v               value specified by the tool in the 
 *                              PIN_AddFiniFunction function call
 */
VOID Fini(INT32 code, VOID* v)
{
    // *out << "===============================================" << endl;
    // *out << "MyPinTool analysis results: " << endl;
    // *out << "Number of instructions: " << insCount << endl;
    // *out << "Number of basic blocks: " << bblCount << endl;
    // *out << "Number of threads: " << threadCount << endl;
    // *out << "===============================================" << endl;
    fclose(output);
}

/*!
 * The main procedure of the tool.
 * This function is called when the application image is loaded but not yet started.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments, 
 *                              including pin -t <toolname> -- ...
 */
int main(int argc, char* argv[])
{
    // Initialize PIN library. Print help message if -h(elp) is specified
    // in the command line or the command line is invalid
    if (PIN_Init(argc, argv))
    {
        return Usage();
    }

    string fileName = KnobOutputFile.Value();

    if (!fileName.empty())
    {
        // out = new std::ofstream(fileName.c_str());
        // sha256 = Sha256(out);
        output = fopen(fileName.c_str(), "w");
        
    }

    string inputFileName = KnobInputFile.Value();

    if(!inputFileName.empty()){
        in = new std::ifstream(inputFileName.c_str());
    }
    else{
        return 1;
    }

    ADDRINT addr;

    while(*in >> std::hex >>addr){
        targetAddrs.push_back(addr);
    }

    if (KnobCount)
    {
        PIN_InitSymbols();

        // IMG_AddInstrumentFunction(Image, 0);

        // Register function to be called to instrument traces
        TRACE_AddInstrumentFunction(Trace, 0);

        // Register function to be called for every thread before it starts running
        PIN_AddThreadStartFunction(ThreadStart, 0);

        // Register function to be called when the application exits
        PIN_AddFiniFunction(Fini, 0);
    }

    // std::stringstream ss;
    // ss << std::hex << KnobAddr.Value();
    // ss >> targetAddr;
    // cerr << std::hex<< targetAddr <<endl;

    cerr << "===============================================" << endl;
    cerr << "This application is instrumented by MyPinTool" << endl;
    if (!KnobOutputFile.Value().empty())
    {
        cerr << "See file " << KnobOutputFile.Value() << " for analysis results" << endl;
    }
    cerr << "===============================================" << endl;

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
