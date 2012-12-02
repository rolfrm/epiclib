import pycompile as pyc
import sys
import subprocess as subproc
import time



pyc.Compile("start","","obj",["main2.cpp","fltkgui.cpp"],["epiclib","fltk"],3,0,pyc.CPPOX,["../"],["../"])

