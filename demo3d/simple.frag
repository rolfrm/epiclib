uniform sampler2D tex;
uniform vec4 Color;
varying vec3 pos;
uniform float y;
void main(){
     
    gl_FragColor = vec4(texture2D(tex,pos.xz).rgb,1);
}    
