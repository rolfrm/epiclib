import pycompile as pyc
import sys
import subprocess as subproc
import time
t = "demo.cpp"

pyc.CompileSharedLibrary("libepiclib.so","","so_obj",["Graphics/GraphicsCore.cpp","Graphics/Program.cpp","Graphics/VertexBufferObject.cpp","Physics/PhysicsCore.cpp","Math/Math.cpp","Physics/PhysicsObject.cpp","Physics/Polygon.cpp","Physics/Constraint.cpp","Physics/SeperatingAxis.cpp","Physics/Quadtree.cpp","Physics/QuadDetection.cpp","Events/event_init.c", "Utils/ndtree.cpp", "Utils/Timing.cpp", "Graphics/oglCppWrap.cpp"],["glfw","IL","GL","GLEW"],3,0,pyc.CPPOX,[],
            ["./","Physics"])
pyc.Compile("start","","obj", ["demo.cpp"], [],3,0,pyc.CPPOX,[],["./"],["libepiclib.so"])
