#version 320 es
precision mediump float;
in vec2 varyvpos;
in vec2 varytexpos;
out vec4 FragColor;

uniform float time;
uniform int pointerX;
uniform int pointerY;
uniform sampler2D txt1;
uniform sampler2D txt2;

void main(){
    float x=(float(varyvpos.x));

    int calc=varyvpos.x*varyvpos.x+varyvpos.y*varyvpos.y>(time)?1:0;

    FragColor = vec4(abs(sin(time+varyvpos.y)),abs(cos(time+x)),0.7,0); 
    FragColor = mix(texture(txt1,varytexpos),texture(txt2,varytexpos),float(calc));
}