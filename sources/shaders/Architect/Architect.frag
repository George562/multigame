uniform sampler2D overlay;

uniform float uTime;      // Time in seconds since load

vec3 pallete(float t) {
    vec3 a = vec3(0.731, 1.098, 0.192);
    vec3 b = vec3(0.358, 1.090, 0.657);
    vec3 c = vec3(1.077, 0.360, 0.328);
    vec3 d = vec3(0.965, 2.265, 0.837);

    return a + b * cos(6.28318 * (c * t + d));
}

void main() {
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);

    gl_FragColor = gl_Color * vec4(mix(pixel.xyz, pallete(uTime * 0.1), clamp(abs(sin(uTime * 2.)), 0.7, 1.)), pixel.w);
}
