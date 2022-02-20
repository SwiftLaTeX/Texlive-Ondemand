import pykpathsea_xetex
import os
fp = open("dump2.txt")
for line in fp:
    line = line.strip()
    base = os.path.basename(line)
    if line.endswith(".woff"):
        ans = pykpathsea_xetex.find_file(base, 36)
        if ans:
            print(line)
