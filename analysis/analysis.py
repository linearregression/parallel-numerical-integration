#! /usr/bin/env python
# -*- coding: utf-8 -*-

__author__ = 'bachmanm'

import matplotlib.pyplot as plt
import fileinput
import numpy as np

values = np.empty((3, 4), float)
ps = [1, 2, 4, 8]
for (i, n) in enumerate([int(1e6), int(1e7), int(1e8)]):
    for (j, p) in enumerate(ps):
        filename = "results" + str(p) + "-" + str(n) + ".txt"
        numbers = map(float, fileinput.input(filename))
#        print np.mean(numbers), np.std(numbers)*100/np.mean(numbers)
        values[i][j] = np.mean(numbers)

speedups = np.empty((3,4), float)
for i, v in enumerate(values):
    speedups[i] = map(lambda x : values[i][0]/x, v)

plt.plot(ps, speedups[0])
plt.plot(ps, speedups[1], "r-")
plt.plot(ps, speedups[2], "g-")
plt.plot(ps, [1,2,4,8], "y-")
plt.show()

#plt.plot(ps, values[0])
#plt.plot(ps, values[1], "r-")
#plt.plot(ps, values[2], "g-")
#plt.show()

#
#plt.plot(numbers)
#plt.ylabel('some numbers')
#plt.show()
