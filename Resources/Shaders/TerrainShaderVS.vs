attribute vec3 a_posL; //pozitia
vec3 c_blend;
uniform sampler2D u_texture3;

uniform vec3 u_color; //culoarea cetii
varying vec3 v_color;

attribute vec2 a_uv;
varying vec2 v_uv;
attribute vec2 a_uv2;
varying vec2 v_uv2;

uniform vec3 u_height;
uniform mat4 uMVP;
uniform mat4 uM; //matricea model
varying vec4 fragPos;

attribute vec3 a_norm;
varying vec3 v_norm;
void main()
{
//vec4 v_norm4;
//v_norm = (uM * vec4(normalize(a_norm), 0.0)).xyz;

v_uv = a_uv;
v_uv2 = a_uv2;

vec4 posL = vec4(a_posL, 1.0);
vec4 pos_nou = vec4(a_posL, 1.0);
fragPos = uM * posL;

c_blend = texture2D(u_texture3, a_uv2).rgb;
pos_nou.y += c_blend.r * u_height.r  + c_blend.g * u_height.g + c_blend.b * u_height.b;

v_color = u_color;
gl_Position = uMVP * pos_nou;

}
   