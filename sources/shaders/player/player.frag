uniform sampler2D overlay;

uniform vec2 uResolution;  // Canvas size (width, height)
uniform vec2 uMouse;       // mouse position in screen pixels

uniform vec2 uPlayerPosition;

void main() {
    vec2 uv = (gl_FragCoord.xy - uResolution.xy / 2.) / uResolution.xy;
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);

    vec2 playerPosition = vec2(uPlayerPosition.x, uResolution.y - uPlayerPosition.y);

    vec2 lightPosition = gl_FragCoord.xy - playerPosition;
    vec2 normal = vec2(uMouse.x, uResolution.y - uMouse.y) - playerPosition;
    float intensity = smoothstep(0.65, 0.85, dot(normalize(normal), normalize(lightPosition)));
    intensity = clamp(intensity, 0.25, 1.);

    gl_FragColor = gl_Color * pixel;
}
