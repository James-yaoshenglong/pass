with open("../data/blockTempResult.txt", "r") as f:
    while True:
        line1 = f.readline()
        if not line1:
            break
        li1 = line1.strip().split(" ")
        if li1[0] == "the":
            blockNum = int(li1[-1])
            line2 = f.readline()
            li2 = line2.strip().split(" ")
            if li2[0] == "the":
                continue
            elif li2[0] == "enter":
                print(li2[-1], "is in block", blockNum)
        elif li1[0] == "enter":
            print(li1[-1], "is not in any block")
        else:
            continue
    