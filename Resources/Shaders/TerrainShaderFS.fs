precision mediump float;
uniform sampler2D u_texture0, u_texture1, u_texture2, u_texture3;
varying vec2 v_uv, v_uv2;
vec4 color;
vec3 c_blend, c_rock, c_grass, c_dirt, c_final;

uniform float r;
uniform float R;
uniform vec3 fogColor;
uniform vec3 cameraPos;
varying vec4 fragPos;

varying vec3 v_norm;
uniform vec3 c_Lamb;
uniform vec3 c_Lspec;
uniform vec3 c_Ldif;
uniform vec3 u_light_dir;
uniform float SpecPower;
uniform float ratio;
void main()
{
	c_dirt = texture2D(u_texture0, v_uv).rgb;
	c_rock = texture2D(u_texture1, v_uv).rgb;
	c_grass = texture2D(u_texture2, v_uv).rgb;
	c_blend = texture2D(u_texture3, v_uv2).rgb;
	c_blend = c_blend.r * c_rock+c_blend.g * c_grass+c_blend.b * c_dirt;
	c_final = c_blend;

	//vec3 colorAmb = c_final.rgb * c_Lamb;
	//vec3 Cdiff = c_final.rgb * c_Ldif * max(dot(normalize(v_norm), -normalize(u_light_dir)), 0.0);

	//vec3 Rf;
	//vec3 E = normalize(cameraPos - fragPos.xyz);
	//Rf = normalize(reflect(normalize(u_light_dir), normalize(v_norm)));
	//vec3 Cspec = c_Lspec * pow(max(dot(Rf , E), 0.0), SpecPower);

	//vec3 Cfinal = ratio * colorAmb + (1.0 - ratio) * (Cdiff + Cspec);

	float dist = distance(cameraPos, fragPos.xyz);
	float alpha;
	alpha = clamp((dist - r)/(R - r), 0.0, 1.0);
	//alpha = 0.5;
	vec4 color = alpha * vec4(fogColor, 1.0) + (1.0 - alpha) * vec4(c_final, 1.0);

	gl_FragColor = color;
}
