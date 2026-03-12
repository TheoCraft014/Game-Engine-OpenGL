precision mediump float;
uniform samplerCube u_texture0;
varying vec3 v_coord;
vec4 color;
void main()
{
	color = textureCube(u_texture0, v_coord);
	if(color.a < 0.1)
		discard;
	gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
	gl_FragColor = color;
}
