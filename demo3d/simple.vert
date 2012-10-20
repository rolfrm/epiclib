attribute vec3 localPosition;

uniform mat4 T;

void main(void) { 
    gl_Position = T * vec4(localPosition,1);

}