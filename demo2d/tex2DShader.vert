attribute vec2 pos;
varying vec2 uv;
uniform vec2 position;
uniform vec2 size;
void main(){
     uv = pos;
     gl_Position = vec4((2.0*(pos*size + position) - 1.0),0,1);
}