uniform sampler2D overlay;
uniform sampler2D noise_png;

uniform float uTime;
uniform vec2  uPosition;

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st), f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Mix 4 coorners percentages
    return mix(mix(a, b, smoothstep(0.0, 1.0, f.x)),
				mix(c, d, smoothstep(0.0, 1.0, f.x)),
				smoothstep(0.0, 1.0, f.y));
}

void main() {
    vec2 uv = vec2(gl_TexCoord[0].x, 1. - gl_TexCoord[0].y);
	vec4 pixel = texture2D(overlay, uv);
    vec2 Muv = (gl_TexCoord[0].xy + uPosition) * 8. + vec2(0., uTime * 3.);
	uv.y -= noise(Muv);
	gl_FragColor = gl_Color * vec4(pixel.rgb, smoothstep(-0.05, 0.05, uv.y));
}
