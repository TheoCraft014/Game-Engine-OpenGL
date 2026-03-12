precision mediump float;
uniform sampler2D u_texture0;
varying vec2 v_uv;
vec4 color;
vec4 color_i;

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
	color_i = texture2D(u_texture0, v_uv);
	if(color_i.a < 0.1)
		discard;

	vec3 colorAmb = color_i.rgb * c_Lamb;
	vec3 Cdiff = color_i.rgb * c_Ldif * max(dot(normalize(v_norm), -normalize(u_light_dir)), 0.0);

	vec3 Rf;
	vec3 E = normalize(cameraPos - fragPos.xyz);
	Rf = normalize(reflect(normalize(u_light_dir), normalize(v_norm)));
	vec3 Cspec = c_Lspec * pow(max(dot(Rf , E), 0.0), SpecPower);

	vec3 Cfinal = ratio * colorAmb + (1.0 - ratio) * (Cdiff + Cspec);

	float dist = distance(cameraPos, fragPos.xyz);
	float alpha;
	alpha = clamp((dist - r)/(R - r), 0.0, 1.0);
	color = alpha * vec4(fogColor, 1.0) + (1.0 - alpha) * vec4(Cfinal, color_i.a);

	gl_FragColor = color;
}
