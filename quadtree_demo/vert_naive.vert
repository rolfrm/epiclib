attribute vec2 vert;
uniform vec2 pos;
uniform vec2 scale;
uniform vec2 cameraPosition;
uniform vec2 cameraScale;

void main(){ 
     vec2 screenCoords = ((vert*scale + pos) + cameraPosition)/cameraScale;
     gl_Position = vec4(screenCoords,1.0,1.0);

}