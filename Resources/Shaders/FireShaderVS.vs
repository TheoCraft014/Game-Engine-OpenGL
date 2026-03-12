attribute vec3 a_posL;
attribute vec2 a_uv;
varying vec2 v_uv;
varying vec4 fragPos;

uniform mat4 uMVP;

uniform vec3 u_color;
varying vec3 v_color;
void main()
{
    v_uv = a_uv;
    vec4 posL = vec4(a_posL, 1.0);
    fragPos = uMVP * posL;
    gl_Position = uMVP * posL;

}
   