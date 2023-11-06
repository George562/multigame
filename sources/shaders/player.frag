uniform sampler2D overlay;

uniform vec2 u_resolution;  // Canvas size (width, height)
uniform vec2 u_mouse;       // mouse position in screen pixels
uniform float u_time;       // Time in seconds since load

uniform vec2 u_playerPosition;
uniform float u_playerRadius;

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))
                 * 43758.5453123);
}

vec3 pallete(float t) {
    vec3 a = vec3(0.938, 0.328, 0.718);
    vec3 b = vec3(0.659, -3.142, 0.328);
    vec3 c = vec3(-3.142, -3.142, -3.142);
    vec3 d = vec3(-0.692, -2.032, -0.802);
    
    return a + b * cos(6.28318 * (c * t + d));
}

void main() {
    vec2 uv = (gl_FragCoord.xy - u_resolution.xy / 2.) / u_resolution.xy;
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);

    float coef = u_time * 300000.;
    float d = length(coef * uv);
    float phi = abs(dot(vec2(cos(d), sin(d)), normalize(coef * uv)));

    vec2 playerPosition = vec2(u_playerPosition.x, u_resolution.y - u_playerPosition.y);

    vec2 lightPosition = gl_FragCoord.xy - playerPosition;
    vec2 normal = vec2(u_mouse.x, u_resolution.y - u_mouse.y) - playerPosition;
    float intensity = smoothstep(0.65, 0.85, dot(normalize(normal), normalize(lightPosition)));
    intensity = clamp(intensity, 0.4, 1.);

    // gl_FragColor = gl_Color * vec4(mix(pixel.xyz, pallete(u_time * 0.1), clamp(abs(sin(u_time * 2.)), 0.0, 0.3)) * intensity, pixel.w);
    // gl_FragColor = gl_Color * pixel * phi;
    gl_FragColor = gl_Color * vec4(pixel.xyz * random(uv * u_time), pixel.w);
}
