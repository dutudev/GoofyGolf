#version 330

in vec2 fragTexCoord;
uniform float time;

out vec4 fragColor;

const float screenHeight = 600;
const float screenWidth = 800;

void main(){
    vec2 timeVec = vec2(time, time);
    //timeVec.y *= screenWidth / screenHeight;
    vec2 uv = fragTexCoord + timeVec;
    uv.x *= screenWidth / screenHeight;
    uv *= 10;
    uv = fract(uv);
    float f = step(0.5, uv.x) + step(0.5, uv.y);
    f -= step(0.5, f - 0.6) * 5.0;
    f = clamp(f, 0.0, 1.0);
    vec3 col = vec3(83.0 / 255.0, 141 / 255.0, 34 / 255.0) * f + vec3(115.0 / 255.0, 169.0 / 255.0, 66.0 / 255.0) * step(f, 0.5);
    fragColor = vec4(col.x, col.y, col.z, 1.0);
}