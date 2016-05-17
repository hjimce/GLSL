#version 150//哥哥
// A texture is expected as program attribute
uniform sampler2D Texture;

// The vertex shader fill feed this input
in vec2 fragTexCoord;

// The final color
out vec4 FragmentColor;

void main() {
  // Lookup the color in Texture on coordinates given by fragTexCoord
  vec3 pColor = texture(Texture, fragTexCoord.xy).rgb;
  if (fragTexCoord.xy.x>0.5)
  {

	float h= 0.3*pColor.x + 0.59*pColor.y + 0.11*pColor.z;
	FragmentColor=vec4(h,h,h,0.); 
  }
  else
  {
	FragmentColor=texture(Texture, fragTexCoord.xy);
  }
  
  
}
