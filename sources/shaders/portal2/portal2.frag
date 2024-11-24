uniform sampler2D overlay;
uniform sampler2D noise_png;
uniform float uTime; // Time in seconds since load

void main() {
    vec2 uv = gl_TexCoord[0].xy; 
    uv.x = (uv.x - 1.) * 1.18 + 1.;

    vec2 Muv = vec2(uv.x + 0.8 * uTime, uv.y);
	float noise = 0.25 * texture2D(noise_png, fract(Muv)).r;
	noise *= 1. - 0.5 * pow(uv.y - 0.5, 2.) - uv.x;

    vec2 offset = vec2(noise, 0.);
    vec4 pixel = texture2D(overlay, uv + offset);

    gl_FragColor = pixel;
}
