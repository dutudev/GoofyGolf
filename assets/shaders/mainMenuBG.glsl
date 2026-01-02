#version 330

in vec2 fragTexCoord;
uniform sampler2D texture0;
uniform float time;

out vec4 fragColor;

const float screenHeight = 600;
const float screenWidth = 800;

void main(){
    vec2 timeVec = vec2(time, time);
    //timeVec.y *= screenWidth / screenHeight;
    vec2 uv = (fragTexCoord) * vec2(22, 1) + timeVec;
    uv.x *= screenWidth / screenHeight;
    uv *= 10;
    uv = fract(uv);
    uv *= vec2(0.045, 1);
    //fragColor = vec4(uv.x, uv.y, 0, 1);
    fragColor = texture(texture0, uv);
}