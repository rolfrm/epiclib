attribute vec2 position;
uniform int width;
uniform int height;
uniform float x;
uniform float y;
uniform float s;
void main(){
     //float aspectRatio = float(width)/float(height);
     vec2 normalizedPosition = (position*2.0)*s + vec2(x,y) - 1;
     gl_Position = vec4(normalizedPosition, 0, 1);

}