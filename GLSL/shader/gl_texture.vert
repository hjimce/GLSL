#version 420
// The inputs will be fed by the vertex buffer objects
in vec2 Position;
in vec2 TexCoord;

// This will be passed to the fragment shader
out vec2 fragTexCoord;

void main() {
  // Copy the input to the fragment shader
  fragTexCoord = vec2(TexCoord)+vec2(0.5,0.5);
  //fragTexCoord = (Position + 1.0) * 0.5;
  // Calculate the final position on screen
  // Note the visible portion of the screen is in <-1,1> range for x and y coordinates
  gl_Position = vec4(Position, 0.0, 1.0);
}
