uniform sampler2D tex;
uniform vec4 Color;
varying vec3 pos;
uniform float y;
void main(){
     
    gl_FragColor = texture2D(tex,pos.xy);
}    
