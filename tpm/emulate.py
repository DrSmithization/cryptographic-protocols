#!/usr/bin/env python

import random
import tpm

K, N, L = (4, 20, 5)

def main():
    tpm1 = tpm.TreeParityMachine(K, N, L)
    tpm2 = tpm.TreeParityMachine(K, N, L)

    totalRounds = 0
    successfullRounds = 0
    unsyncWeights = 1
    
    inp = [[0] * N] * K
    while unsyncWeights > 0:
        totalRounds += 1
        for i in range(0, K):
            for j in range(0, N):
                inp[i][j] = 2 * random.randint(0, 1) - 1
                
        if tpm1.Analyze(inp) == tpm2.Analyze(inp):
            unsyncWeights = 0
            successfullRounds += 1
            tpm1.LearnHebbian(inp)
            tpm2.LearnHebbian(inp)

            for i in range(0, tpm1.hiddenNeurons):
                for j in range(0, tpm1.inputVectorSize):
                    if tpm1.weights[i][j] != tpm2.weights[i][j]:
                        unsyncWeights += 1

            if totalRounds % 1000 == 0:
                print "total={0}, successfull={1}, unsync={2}".format(totalRounds, successfullRounds, unsyncWeights)
    print "after total={0} and success={1}".format(totalRounds, successfullRounds)

if __name__ == "__main__":
    main()

