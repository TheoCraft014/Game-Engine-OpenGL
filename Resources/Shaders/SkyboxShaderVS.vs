attribute vec3 a_posL;
varying vec3 v_coord;
uniform mat4 uMVP;
void main()
{
v_coord = a_posL;
vec4 posL = vec4(a_posL, 1.0);
gl_Position = uMVP * posL;
}

   