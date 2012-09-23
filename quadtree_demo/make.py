import pycompile as pyc
import sys
import subprocess as subproc
import time
t = "demo.cpp"

pyc.Compile("start","","obj", ["demo.cpp","quadtree_demo.cpp", "quadtree.cpp"], ["epiclib"],3,0,pyc.CPPOX,["../"],["../"],)
