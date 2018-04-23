import numpy
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from tqdm import tqdm
from collections import namedtuple
import random
import sys
import math
import time
import copy

def printf(format, *args):
    sys.stdout.write(format % args)

def showforest(nx,ny,t):
    for i in range(ny):
        for j in range(nx):
            if tnew[i][j].STATE == 'F':
                printf('\033[41m'"%c "'\033[0m',tnew[i][j].STATE)
            elif tnew[i][j].STATE == '^':
                printf('\033[42m'"%c "'\033[0m',tnew[i][j].STATE)
            elif tnew[i][j].STATE == '.':
                printf('\033[100m'"%c "'\033[0m',tnew[i][j].STATE)
            else:
                printf('\033[0m'"%c "'\033[0m',tnew[i][j].STATE)
        printf("\n")
    printf('\x1b[2J\x1b[H')
#    time.sleep(1)
    
def showforest_persist(nx,ny,t):
    for i in range(ny):
        for j in range(nx):
            if tnew[i][j].STATE == 'F':
                printf('\033[41m'"%c "'\033[0m',tnew[i][j].STATE)
            elif tnew[i][j].STATE == '^':
                printf('\033[42m'"%c "'\033[0m',tnew[i][j].STATE)
            elif tnew[i][j].STATE == '.':
                printf('\033[100m'"%c "'\033[0m',tnew[i][j].STATE)
            else:
                printf('\033[0m'"%c "'\033[0m',tnew[i][j].STATE)
        printf("\n")

fig = plt.figure()


prob = 0.2
NX = 30
NY = 30
generations = 10
Forest = namedtuple('Forest', 'STATE B I D')
tnew = []

for i in range(NY):
    new = []
    for j in range(NX):
        #Node = Forest('E',random.random(),random.random(),random.random())
        Node = Forest(' ',0.5,0.95,random.random())
        new.append(Node)
    tnew.append(new)


# Fill forest with trees
Dinit = 0.85
for i in range(1,NY-1):
    for j in range(1,NX-1):
        if Dinit - tnew[i][j].D > 0:
            tnew[i][j] = tnew[i][j]._replace(STATE='^')


#Start a fire in the middle of the grid */
# Constant source fire */ //	tnew[nx/2][ny/2].B = 1;
tnew[NY//2][NX//2] = tnew[NY//2][NX//2]._replace(STATE = 'F');


"""
generation = 0
ims=[]

"""
t = []
t = list(tnew)
for i in tqdm(range(generations)):
    t = copy.deepcopy(tnew)
    print(id(t),id(tnew),id(t[0]),id(tnew[0]),id(new),id(t[0][0]))
    for i in range(1,NY-1):
        for j in range(1,NX-1):
            
            #If a cell is burning see if it continues burning
            #otherwise the fire goes out. 
            if t[i][j].STATE == 'F':
                if t[i][j].B < random.random():
                    tnew[i][j] = tnew[i][j]._replace(STATE='.')
            # If cell is unburnt but has burning neighbors see
            # if cell ignites.
            if t[i][j].STATE == '^':    
                # Either a corner neighbor possibly ignites cell
                # or side neighbor;
                # Corner neighbor influence is suppressed but 1/sqrt(2)
                if 0.5 > random.random():
                    if (t[i+1][j+1].STATE=='F')or(t[i-1][j+1].STATE=='F')or(t[i+1][j-1].STATE=='F')or(t[i-1][j-1].STATE=='F'):
                        if t[i][j].I/math.sqrt(2.) > random.random():
                            tnew[i][j] = tnew[i][j]._replace(STATE='F') 
                else :    
                    if (t[i+1][j].STATE == 'F')or(t[i][j+1].STATE == 'F')or(t[i][j-1].STATE == 'F')or(t[i-1][j].STATE == 'F') :
                            tnew[i][j] = tnew[i][j]._replace(STATE='F') 
            showforest(NX,NY,tnew)

                    
showforest_persist(NX,NY,t)


