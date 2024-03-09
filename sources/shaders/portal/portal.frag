uniform sampler2D overlay;

uniform vec2 uResolution;  // Canvas size (width, height)
uniform vec2 uMouse;       // mouse position in screen pixels
uniform float uTime;       // Time in seconds since load

uniform vec2 uPlayerPosition;
uniform float u_playerRadius;

vec3 pallete(float t) {
    vec3 a = vec3(0.938, 0.328, 0.718);
    vec3 b = vec3(0.659, -3.142, 0.328);
    vec3 c = vec3(-3.142, -3.142, -3.142);
    vec3 d = vec3(-0.692, -2.032, -0.802);

    return a + b * cos(6.28318 * (c * t + d));
}

void main() {
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);

    vec2 playerPosition = vec2(uPlayerPosition.x, uResolution.y - uPlayerPosition.y);

    vec2 normal = gl_FragCoord.xy - playerPosition;
    vec2 lightPosition = vec2(uMouse.x, uResolution.y - uMouse.y) - playerPosition;
    float intensity = smoothstep(0.65, 0.85, dot(normalize(normal), normalize(lightPosition)));
    intensity += smoothstep(1.6, -0.2, length(normal) / u_playerRadius - 1.0);
    intensity = clamp(intensity, 0.25, 1.);
    // gl_FragColor = gl_Color * vec4(mix(pixel.rgb, pallete(uTime * 0.1), clamp(abs(sin(uTime * 2.)), 0.0, 0.3)) * intensity, pixel.a);
    gl_FragColor = gl_Color * vec4(pixel.rgb * intensity, pixel.a);
}
