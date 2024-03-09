uniform sampler2D overlay;
uniform sampler2D noise_png;

uniform vec2 uResolution;
uniform float uTime;

float random (in vec2 st) {
    return fract(sin(
                     dot( st.xy, vec2(12.9898,78.233) )
                    ) * 43758.5453123 );
}

void main() {
    vec2 uv = gl_FragCoord.xy / uResolution;
    float offSpeed = 0.075;
    float noise = texture2D(noise_png, fract(uv + vec2(uTime * offSpeed, 0.))).r;
    // noise = noise1(uTime * offSpeed);
    vec2 offset = noise * vec2(0.2, 0.18);
    vec2 correctedOffset = vec2(0.09, 0.09);
    vec4 pixel = texture2D(overlay, fract(uv + offset - correctedOffset));

    gl_FragColor = pixel;
}
