import pycompile as pyc
import sys
import subprocess as subproc
import time


pyc.Compile("start","","obj",["octree_demo.cpp","Polygon.cpp","Camera.cpp","CameraControl.cpp", "Tetragon.cpp", "Shader.cpp","GameObject.cpp","AABB.cpp","World.cpp"],["epiclib"],3,0,pyc.CPPOX,["../"],["../"])
