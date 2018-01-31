#version 330 
     
in vec2 UV;

out vec4 color;
     
uniform sampler2D myTextureSampler;

     
     
void main(){

vec4 temp = vec4(texture2D(myTextureSampler, UV).rgb, 1.0f); //nakladanie fragmentow tekstury na trojkaty
color = temp;
     
}

