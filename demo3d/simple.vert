attribute vec3 localPosition;

uniform mat4 Projection;
uniform mat4 Modelview;
 
void main(void) { 
    gl_Position = Projection * Modelview * vec4(localPosition,1);
    //gl_Position = vec4(localPosition.xy,0,1);
}