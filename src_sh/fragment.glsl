#version 460 core
precision mediump float;
in vec2 varyvpos;
in vec2 varytexpos;
out vec4 FragColor;

uniform float time;
uniform int pointerX;
uniform int pointerY;
uniform sampler2D txt1;
uniform sampler2D txt2;
float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}
void main(){
    float x=(float(varyvpos.x));

    // int calc=(varyvpos.x*varyvpos.x+varyvpos.y*varyvpos.y>(time)?1:0);
    
    // // FragColor = vec4(abs(sin(time+varyvpos.y)),abs(cos(time+x)),0.7,0); 
    // FragColor = mix(texture(txt1,varytexpos),texture(txt2,varytexpos),float(calc));


    vec3 msdf_sample = texture(txt1, varytexpos).rgb;
    float distance = median(msdf_sample.r, msdf_sample.g, msdf_sample.b);
    float screenPxDistance = 4.0 * (distance - 0.5);
    float coverage = clamp(screenPxDistance + 0.5, 0.0, 1.0);
    FragColor = vec4(vec3(coverage), 1.0);
    //    FragColor=texture(txt1,varytexpos);


}