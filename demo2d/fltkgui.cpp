#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Text_Display.H>
#include "../Math/GenericVector.h"



using namespace std;
//--------------------------------------------
Fl_Color_Chooser * c = NULL;
Fl_Slider * fs = NULL;
Fl_Text_Display * td = NULL;
Vec<float,4> getCurrentColor(){
	return vec<float>(c->r(),c->g(),c->b(),1.0);
}
#include <iostream>
#include <sstream>
#include <math.h>
int size = 1;

int getCurrentSize(){
  return size;
}
Fl_Window * winptr;
void setCurrentColor(Vec<unsigned char,4> color){
  Vec<float,4> col = color.As<float>() / 255.0;
  c->rgb(col[0],col[1],col[2]);
  Fl::lock();
  Fl::awake();
  winptr->redraw();
  Fl::unlock();
}

void slider_callback(Fl_Widget * w, void * data){
  std::stringstream str;
  double v = fs->value();
  v = pow(2.0,v*10);
  size = v;
  str << (int)v;
  td->buffer()->text(str.str().c_str());
}

//-------------------------------------------- 
int uimain() {

    Fl_Window win( 300,200,"Testing" );
    winptr = &win;
    win.begin();
    Fl_Color_Chooser chooser(0,0,150,150,"chooser");
    Fl_Slider slider(0,160,100,10,"slider");
    fs = &slider;
    Fl_Text_Display text(150,0,100,50);
    td = &text;
    td->buffer(new Fl_Text_Buffer());
    std::cout << td->buffer() << "\n";
    
    slider.type(FL_HORIZONTAL);
    slider.callback(slider_callback);
    win.end();
    c = &chooser;
    win.show();
    return Fl::run();
}

