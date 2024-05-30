uniform sampler2D overlay;
uniform sampler2D noise_png;

uniform vec2 uResolution;
uniform float uTime;

float random(in vec2 st) {
    return fract(sin(
                     dot( st.xy, vec2(12.9898,78.233) )
                    ) * 43758.5453123 );
}

void main() {
    vec2 uv = gl_FragCoord.xy / uResolution;
    vec2 Muv = vec2(uv.x, uv.y - uTime);
	float noise = texture(noise_png, Muv).y;
	noise *= -100.;
	uv.y += 4. * pow(uv.x - 0.5, 2.) - 0.15;
	uv *= 100.;
    uv.y += noise;
	gl_FragColor = mix(vec4(0.97, 0.94, 0.94, 1.), vec4(1.), uv.y);
}
 