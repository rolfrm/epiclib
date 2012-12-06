uniform sampler2D tex;
uniform vec4 color;
varying vec2 uv;
void main(){
     
     gl_FragColor = texture2D(tex,uv) * color;
}