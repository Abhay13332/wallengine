#version 320 es
layout(location=0) in vec2 pos;
layout(location=1) in vec2 texpos;

out vec2 varyvpos;
out vec2 varytexpos;


void main(){
    gl_Position = vec4(pos,0,1.0);
    varyvpos=pos;
    varytexpos=(texpos+vec2(1))*0.5;
}