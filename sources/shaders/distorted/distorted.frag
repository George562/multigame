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
    vec2 offset = (noise - 0.5) * vec2(0.2, 0.18); // making noise from [0; 1] to [-0.5; 0.5]
    vec4 pixel = texture2D(overlay, fract(uv + offset));

    gl_FragColor = pixel;
}
