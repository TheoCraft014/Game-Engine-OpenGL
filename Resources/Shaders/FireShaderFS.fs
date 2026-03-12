precision mediump float;
uniform sampler2D u_texture0; //displacement texture
uniform sampler2D u_texture1; //fire texture
uniform sampler2D u_texture2; //fire mask
varying vec2 v_uv;
vec4 color;

uniform float r;
uniform float R;
uniform vec3 fogColor;
uniform vec3 cameraPos;
varying vec4 fragPos;

uniform float u_Time;
uniform float u_DispMax;
void main()
{
	vec2 disp = texture2D(u_texture0, vec2(v_uv.x, v_uv.y + u_Time)).rg;
	vec2 offset = (2.0 * disp - 1.0) * u_DispMax; // [-DispMax, +DispMax]

	vec2 v_uv_displaced = v_uv + offset;
	vec4 c_fire = texture2D(u_texture1, v_uv_displaced);
	vec4 c_alpha = texture2D(u_texture2, v_uv);
	c_fire.a *= c_alpha.r;

	float dist = distance(cameraPos, fragPos.xyz);
	float alpha;
	alpha = clamp((dist - r)/(R - r), 0.0, 1.0);
	alpha = 0.0;
	color = alpha * vec4(fogColor, 1.0) + (1.0 - alpha) * c_fire;

	gl_FragColor = color;
}
