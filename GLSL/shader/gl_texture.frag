#version 420

uniform float co;
out vec4 FragmentColor;
void main()
 {
  FragmentColor=vec4(co,co,co,0.); 
}
