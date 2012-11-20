attribute vec3 vertPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 camera;
uniform vec3 camPos;
varying vec3 localPos;
varying vec3 direction;
void main(void) {
     vec4 modelPos =  model * vec4(vertPos,1);
     //vec3 camPos = vec3(camera[3][0],camera[3][1],camera[3][2]);
     gl_Position = projection * camera * modelPos;
     localPos = vertPos + 1.0 ;
     direction = modelPos.xyz - camPos.xyz;
}