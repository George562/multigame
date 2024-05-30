uniform sampler2D overlay;
uniform sampler2D noise_png;

uniform float uTime;

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec2 Muv = vec2(uv.x, fract(uv.y - uTime));
	float noise = texture2D(noise_png, Muv).r;
	noise *= -100.;
	uv.y += 4. * pow(uv.x - 0.5, 2.) - 0.15;
	uv *= 100.;
    uv.y += noise;
	gl_FragColor = mix(vec4(0.97, 0.94, 0.94, 1.), vec4(1.), uv.y);
}
 