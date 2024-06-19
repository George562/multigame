uniform sampler2D overlay;
uniform sampler2D noise_png;

uniform float uTime;

void main() {
    vec2 uv = vec2(gl_TexCoord[0].x, 1. - gl_TexCoord[0].y);
    vec2 Muv = vec2(uv.x, fract(uv.y - uTime));
	float noise = texture2D(noise_png, Muv).r;
	noise *= -100.;
	uv.y += 4. * pow(uv.x - 0.5, 2.) - 0.15;
	uv *= 100.;
    uv.y += noise;
	gl_FragColor = gl_Color * mix(vec4(0.045, 0.03, 0.02, 1.), vec4(0.), uv.y);
}
