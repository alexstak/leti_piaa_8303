import os

DIRECTORY = "Tests2"

tests = os.listdir(DIRECTORY)

for i in range(len(tests)):
    testPath = DIRECTORY + "/" + tests[i]
    print("Test №", i+1, ": ", testPath, sep="")
    print(*open(testPath, "r").readlines(), sep="")
    os.system("./lab4 < " + testPath)