import argparse
import os
import subprocess
import sys

file_path = os.path.dirname(os.path.realpath(__file__))

PASS_FLAGS = [
    "clang",
    "-flegacy-pass-manager",
    "-Xclang",
    "-load",
    "-Xclang",
    file_path+"/../lib/libBugInsertion.so" 
]

CFLAGS = PASS_FLAGS+sys.argv[1:]
CFLAGS.append(file_path+"/../lib/libInsert.a")
print(CFLAGS)
subprocess.run(CFLAGS)
