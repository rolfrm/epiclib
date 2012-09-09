import pycompile as pyc
import sys
print sys.argv

pyc.Compile("start_demo","","obj",["Graphics/GraphicsCore.cpp","demo.cpp","Physics/PhysicsCore.cpp","Math/Math.cpp","Physics/PhysicsObject.cpp","Physics/Polygon.cpp","Physics/Constraint.cpp","Physics/SeperatingAxis.cpp","Physics/Quadtree.cpp","Physics/QuadDetection.cpp"],["glfw","IL","GL","GLEW"],0,3,pyc.CPPOX,[],
            ["./","Physics"])
