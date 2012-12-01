attribute vec2 pos;
varying vec2 uv;
uniform vec2 position;
uniform vec2 size;
uniform vec2 uvSize;
uniform vec2 uvPos;
void main(){
     uv = pos * uvSize + uvPos;
     gl_Position = vec4((2.0*(pos*size + position) - 1.0),0,1);
}