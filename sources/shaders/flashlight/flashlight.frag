uniform sampler2D overlay;

uniform vec2 uResolution;  // Canvas size (width, height)
uniform vec2 uMouse;       // mouse position in screen pixels

uniform vec2 uPlayerPosition;
uniform float u_playerRadius;

void main() {
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);

    vec2 playerPosition = vec2(uPlayerPosition.x, uResolution.y - uPlayerPosition.y);

    vec2 normal = gl_FragCoord.xy - playerPosition;
    vec2 lightPosition = vec2(uMouse.x, uResolution.y - uMouse.y) - playerPosition;
    float intensity = smoothstep(0.65, 0.85, dot(normalize(normal), normalize(lightPosition)));
    intensity += smoothstep(1.6, -0.2, length(normal) / u_playerRadius - 1.0);
    intensity = clamp(intensity, 0.25, 1.);
    gl_FragColor = gl_Color * vec4(pixel.xyz * intensity, pixel.w);
}
