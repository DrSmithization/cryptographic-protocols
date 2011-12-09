#!/usr/bin/env python

import random

class TreeParityMachine:
    def __init__(self, hiddenNeurons, inputVectorSize, maxWeight):
        self.hiddenNeurons = hiddenNeurons
        self.inputVectorSize = inputVectorSize
        self.maxWeight = maxWeight

        self.sigmas = [0] * hiddenNeurons
        self.weights = []

        for i in range(0, hiddenNeurons):
            self.weights.append(
                [2 * random.randint(0, 1) - 1 for j in range(0, inputVectorSize)]
            )

    def Key(self):
        bits = self.hiddenNeurons * self.inputVectorSize
        w = [0] * (bits / 8 + (1 if bits % 8 > 0 else 0))
        byteNumber = 0
        bitNumber = 0

        for i in range(0, self.hiddenNeurons):
            for j in range(0, self.inputVectorSize):
                w[byteNumber] |= (1 if self.weights[i][j] > 0 else 0) * (2 << bitNumber)
                bitNumber += 1
                if bitNumber == 8:
                    byteNumber += 1
                    bitNumber = 0

        return w

    def Analyze(self, inp):
        for i in range(0, self.hiddenNeurons):
            self.sigmas[i] = 0
            for j in range(0, self.inputVectorSize):
                self.sigmas[i] += self.weights[i][j] * inp[i][j]
            self.sigmas[i] = 1 if self.sigmas[i] > 0 else -1

        self.tau = 1
        for i in range(0, self.hiddenNeurons):
            self.tau *= self.sigmas[i]
        
        return self.tau

    def LearnHebbian(self, inp):
        for i in range(0, self.hiddenNeurons):
            if self.tau == self.sigmas[i]:
                for j in range(0, self.inputVectorSize):
                    self.weights[i][j] += self.sigmas[i] * inp[i][j]
                    self.weights[i][j] = min(self.weights[i][j], self.maxWeight)
                    self.weights[i][j] = max(self.weights[i][j], -self.maxWeight)

