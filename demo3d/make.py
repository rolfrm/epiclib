import pycompile as pyc
import sys
import subprocess as subproc
import time
t = "demo.cpp"


pyc.Compile("start","","obj",["octree_demo.cpp","Polygon.cpp","Camera.cpp"],["epiclib"],3,0,pyc.CPPOX,["../"],["../"])
