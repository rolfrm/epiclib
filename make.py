import pycompile as pyc
import sys
print sys.argv
t = "demo.cpp"
pyc.CompileSharedLibrary("libepiclib.so","","so_obj",["Graphics/GraphicsCore.cpp","Physics/PhysicsCore.cpp","Math/Math.cpp","Physics/PhysicsObject.cpp","Physics/Polygon.cpp","Physics/Constraint.cpp","Physics/SeperatingAxis.cpp","Physics/Quadtree.cpp","Physics/QuadDetection.cpp"],["glfw","IL","GL","GLEW"],0,3,pyc.CPPOX,[],
            ["./","Physics"])
