import pycompile as pyc
import sys
import subprocess as subproc
import time



pyc.Compile("start","","obj",["main.cpp","fltkgui.cpp","Color.cpp","ColorTree.cpp","infinidraw_functions.cpp","QuadtreeRenderer.cpp","SimpleEvents.cpp"],["epiclib","fltk","glfw"],3,0,pyc.CPPOX,["../"],["../../"])

