uniform sampler2D overlay;
uniform sampler2D noise_png;

uniform float uTime;

void main() {
    float offSpeed = 0.075;
    float offSet = offSpeed * uTime;
    float noise = texture2D(noise_png, fract(4. * (gl_TexCoord[0].xy + vec2(sin(offSet) + offSet, 0.)))).r;

    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);
    gl_FragColor = gl_Color * vec4(pixel.rgb * (0.1 / pow(noise, 2.) + 0.3), pixel.a);
}
