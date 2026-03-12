attribute vec3 a_posL;
attribute vec2 a_uv;
varying vec2 v_uv;
varying vec4 fragPos;

uniform mat4 uM;
uniform mat4 uMVP;

uniform vec3 u_color;
varying vec3 v_color;

attribute vec3 a_norm;
varying vec3 v_norm;
void main()
{
vec4 v_norm4;
v_norm = (uM * vec4(normalize(a_norm), 0.0)).xyz;

v_uv = a_uv;
v_color = u_color;
vec4 posL = vec4(a_posL, 1.0);
fragPos = uM * posL;
gl_Position = uMVP * posL;

}
   