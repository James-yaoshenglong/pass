import random

with open("../data/varTempResult.txt", "r") as inF:
    block = []
    result = dict()
    while True:
        line = inF.readline()
        if not line :
            break
        if line.split(" ")[0] == "inserted":
            addr = line.split(" ")[2].strip()
            line = inF.readline()
            while line != "\n":
                pos = 0
                step = 8
                if line.split(" ")[0] == "rax":
                    pos = 1
                elif line.split(" ")[0] == "rbx":
                    pos = 2
                elif line.split(" ")[0] == "rcx":
                    pos = 3
                elif line.split(" ")[0] == "rdx":
                    pos = 4
                elif line.split(" ")[0] == "[rsp+":
                    pos = 5+int(line.split(" ")[1])
                    step = int(line.split(" ")[5])
                line = inF.readline()
                chunk = line.split(" ")[1].strip()
                line = inF.readline()
                while line.split(" ")[0] == "the":
                    loop = line.split(" ")[5]
                    p = line.split(" ")[7]
                    q = line.split(" ")[8]
                    block.append((addr, pos, step, chunk, loop, p, q))
                    line = inF.readline()
            result[addr] = block
for key in result:
    # addr, pos, step, chunk, loop, p, q = random.choice(result[key])
    addr, pos, step, chunk, loop, p, q = result[key][0]
    print(addr, pos, step, chunk, loop, p, q)