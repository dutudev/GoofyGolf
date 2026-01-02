#version 330

in vec2 fragTexCoord;

out vec4 fragColor;

const float screenHeight = 600;
const float screenWidth = 800;

uniform float progress;

void main(){
	vec2 uv = fragTexCoord;
	
	uv -= vec2(0.5);
	uv.x *= screenWidth / screenHeight;
	float  v = length(uv);
	v = step(v, 1.0 - progress);
	fragColor = vec4(0.0, 0.0 , 0.0, 1.0 - v) ;
}