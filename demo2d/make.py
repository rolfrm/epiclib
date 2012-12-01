import pycompile as pyc
import sys
import subprocess as subproc
import time



pyc.Compile("start","","obj",["main2.cpp"],["epiclib"],3,0,pyc.CPPOX,["../"],["../"])
