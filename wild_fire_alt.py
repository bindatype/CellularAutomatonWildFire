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
    time.sleep(1)
    
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

NX = 150
NY = 150
generations = 100
Forest = namedtuple('Forest', 'STATE B I D')
tnew = []

for i in range(NY):
    new = []
    for j in range(NX):
        #Node = Forest('E',random.random(),random.random(),random.random())
        Node = Forest(' ',0.75,0.99,random.random())
        new.append(Node)
    tnew.append(new)


# Fill forest with trees
Dinit = 1
for i in range(1,NY-1):
    for j in range(1,NX-1):
        if Dinit - tnew[i][j].D > 0:
            tnew[i][j] = tnew[i][j]._replace(STATE='^')


#Start a fire in the middle of the grid */
# Constant source fire */ //	tnew[nx/2][ny/2].B = 1;
tnew[NY//2][NX//2] = tnew[NY//2][NX//2]._replace(STATE = 'F');


for step in range(generations):
    t = copy.deepcopy(tnew)
    for i in range(1,NY-1):
        for j in range(1,NX-1):
            if t[i][j].STATE == 'F':
                # probe local neighborhood of burning cells for unburnt cells to ignite
                for li in range(-1,2):
                    for lj in range(-1,2):
                        if not (li == 0 and lj == 0):
                            if t[i+li][j+lj].STATE == '^':
                                if abs(li+lj) == 1:    
                                    if t[i+li][j+lj].I > random.random():
                                        tnew[i+li][j+lj] = tnew[i+li][j+lj]._replace(STATE='F')
                                else:
                                    if t[i+li][j+lj].I/(3.*math.sqrt(2.)) > random.random():
                                        tnew[i+li][j+lj] = tnew[i+li][j+lj]._replace(STATE='F')
                # if cell is burning see if it goes out. 
                if t[i][j].B < random.random():
                    tnew[i][j] = tnew[i][j]._replace(STATE='.')
    if step % 10 == 0:
        showforest(NX,NY,tnew)

                    
showforest_persist(NX,NY,t)


