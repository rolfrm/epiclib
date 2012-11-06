
#include<list>
#include <chrono>
#include <map>

#include "../Graphics/GraphicsCore.hpp"
#include "../Graphics/Program.hpp"
#include "../Graphics/VertexBufferObject.hpp"
#include "../Utils/Timing.h"
#include "../Utils/Debug.h"
#include "Polygon.h"
#include "Camera.h"
#include "../Math/CommonMatrixOperations.h"
#include "../Events/input_events.h"
#include "CameraControl.h"
#include "../Utils/SharedPtr.h"
#include "Shader.h"
#include "Tetragon.h"
#include "GameObject.h"
#include "World.h"

template<typename T>
double D(T v){
  return (double) v;
}

class Context{
public:
  Context(){
    float funnyShapeData[] = {
      -1.0, -1.0, -1.0,  
      1.0, -1.0, -1.0,  
      1.0, 1.0, -1.0,  
      -1.0, 1.0, -1.0, 
      -1.0, -1.0, 1.0,  
      -1.0, 1.0, 1.0, 
      1.0, 1.0, 1.0,  
      1.0, -1.0, 1.0
    };
    float planeData[] = {
      -1.0,0,-1.0, 
      -1.0,0,1.0,
      1.0,0,1.0, 
      1.0,0,-1.0 };
    float plane2DData[] = {
      -1.0,-1.0, 
      1.0,-1.0,
      1.0,1.0, 
      -1.0,1.0};
    float boxData[] = {
      -1.0, -1.0, -1.0,
      1.0, -1.0, -1.0,  
      1.0, 1.0, -1.0,  
      -1.0, 1.0,-1.0,
      
      -1.0, -1.0, 1.0,  
      -1.0, 1.0, 1.0, 
      1.0, 1.0, 1.0,  
      1.0, -1.0, 1.0,
      
      -1.0,-1.0,-1.0,
      1.0,-1.0,-1.0,
      1.0,-1.0,1.0,
      -1.0,-1.0,1.0,

      -1.0,1.0,-1.0,
      1.0,1.0,-1.0,
      1.0,1.0,1.0,
      -1.0,1.0,1.0,

      1.0,-1.0,-1.0,
      1.0,1.0,-1.0,
      1.0,1.0,1.0,
      1.0,-1.0,1.0,

      -1.0,-1.0,-1.0,
      -1.0,1.0,-1.0,
      -1.0,1.0,1.0,
      -1.0,-1.0,1.0
    };
    
    float stars[30000];

    for(int i = 0; i < 30000; i+=3){
      double za = D(rand() % 10000) / 1000.0 ;
      double xa = D(rand() % 10000) / 1000.0 ;
      double x = sin(za);
      double y = cos(za)*cos(xa);
      double z = sin(xa);
      stars[i] = x*100000;
      stars[i+1] = y*100000;
      stars[i+2] = z*100000;
    }

    
    FunnyShape = VBO(funnyShapeData,8,3,VBODrawType::Static);
    Plane = VBO(planeData,4,3,VBODrawType::Static);
    Plane2D = VBO(plane2DData,4,2,VBODrawType::Static);
    Stars = VBO(stars,10000,3,VBODrawType::Static);
    BoxShape = VBO(boxData,24,3,VBODrawType::Static);
    Smilie = Texture::FromFile("smilie.png",Interpolation::Linear,
			       TextureWrap::Repeat,PixelFormat::RGB);
    Grass = Texture::FromFile("grass.png",Interpolation::Linear,
			       TextureWrap::Repeat,PixelFormat::RGB);
    StarColors = Texture::FromFile("star_colors.png",Interpolation::Nearest,
				   TextureWrap::Repeat, PixelFormat::RGB);
  }
  VBO FunnyShape;
  VBO Plane;
  VBO Plane2D;
  VBO Stars;
  VBO BoxShape;
  Texture Smilie;
  Texture Grass;
  Texture StarColors;
};

class GameObjectTest: public GameObject{
public:
  GameObjectTest(Context & context){
    Polygon pgon;
    pgon.Load(context.BoxShape,0);

    Texgon tgon;
    tgon.Load(context.StarColors,0);
    Tetragon tetra(TRSMatrix(vec(0.0f,0.0f,0.0f),
			     vec(0.0f,0.0f,0.0f),
			     vec(1.f,1.0f,1.0f)),
		   pgon,tgon);
    Tetragon * last = &tetra;
    Tetra = tetra;
    aabb = AABBMass(vec(0.0,0.0,0.0),vec(1.0,1.0,1.0),3.0);
  }

protected:
  void Update(World & world){
    double impulse = -16.0;
    
    Tetragon * tetra = &Tetra;
 
  }

};


int main_test(){
  Context context;
  StopWatch swatch;
  World world;
  CameraControl ev;
  glPointSize(2.0);
  ev.cam.position = vec(0.0f,0.0f,25.0f);
  mouse_move_spawner.register_listener(&ev);
  key_event_handler.register_listener(&ev);
  
  FlatShader3D flat;
  flat.SetProjection(ProjectionMatrix(0.01,0.01,0.01,200000.0));
  
  GameObject * go = new GameObjectTest(context);
  world.InsertObject(go);

  go = new GameObjectTest(context);
  go->aabb.pos[1] += 10;
  world.InsertObject(go);

  go = new GameObjectTest(context);
  go->aabb.pos[0] += 10;
  
  go->aabb.pos[1] += 10;
  go->GravityBound = false;
  world.InsertObject(go);
  
  go = new GameObjectTest(context);
  go->aabb.pos[0] += 10;
  
  world.InsertObject(go);
  
  VBO stars = context.Stars;
  while(true){
    auto goList = world.GetNearbyObjects(vec(0.0,0.0,0.0),5000000.0);
    world.PhysicsUpdate(vec(0.0,0.0,0.0),500000,0.1);

    for(auto go : goList){
      go.Get()->DoUpdate(world);
    }

    //ev.cam.position = go->aabb.pos.As<float>() + vec(0.0f,-2.0f,0.0f);
    print(go->aabb.pos);
    swatch.Reset();
    swatch.Start();
    ClearBuffer(vec(0.0f,0.0f,0.0f,1.0f));
    Matrix<float,4> cameraMatrix = ev.GetCamera().getTransformMatrix();
    /*cameraMatrix[3][0] = 0;
    cameraMatrix[3][1] = 0;
    cameraMatrix[3][2] = 0;
    */
    flat.SetCamera(cameraMatrix);   
    
    flat.SetModelView(Matrix<float,4>::Eye());
    context.StarColors.Bind(0);
    stars.BindBuffer(0);
    VertexBufferObject::DrawBuffers(DrawMethod::Points,100);
    flat.SetCamera(ev.GetCamera().getTransformMatrix());   
    goList = world.GetNearbyObjects(vec(0.0,0.0,0.0),5000000.0);
    for(auto go: goList){
      go.Get()->Draw(flat);
    }
    SwapBuffers();
    Sleep(1/60.0 - swatch.ElapsedSeconds());
  }
}

int main(){
  initOpenGL(400,400);
  init_events();
  
  main_test();
  std::cout << "Testing mem corruption\n";
  int * i = new int[12345];
  delete i;
  std::cout << "OK\n";
  return 0;

}
