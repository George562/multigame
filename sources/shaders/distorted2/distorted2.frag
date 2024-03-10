uniform sampler2D overlay;
uniform sampler2D noise_png;

uniform float uTime;

float random(in vec2 st) {
    return fract(sin(
                     dot( st.xy, vec2(12.9898,78.233) )
                    ) * 43758.5453123 );
}

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec2 polarUV = vec2(0.5) - uv;
    float offSpeed = 0.075;
    float s = sin(uTime * offSpeed), c = cos(uTime * offSpeed);
    float noise = texture2D(noise_png, fract(polarUV * mat2(c, -s, s, c))).r;
    vec2 offset = vec2(noise - 0.5); // making noise from [0; 1] to [-0.5; 0.5]
    vec4 pixel = texture2D(overlay, fract(gl_TexCoord[0].xy + offset));

    gl_FragColor = pixel;
}
