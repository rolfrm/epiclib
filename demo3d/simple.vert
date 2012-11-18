attribute vec3 localPosition;

uniform mat4 T;
varying vec3 pos;
void main(void) { 
     
    vec4 pos2 = T * vec4(localPosition / 2.0 + 0.5,1);
    gl_Position = pos2;
    pos = localPosition / 2.0 + 0.5;
}
