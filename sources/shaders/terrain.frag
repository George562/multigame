uniform float offset;
uniform vec3 point;
uniform vec4 color;
uniform mat4 matrix;
uniform sampler2D overlay;
uniform sampler2D current;

uniform vec2 u_resolution;  // Canvas size (width, height)
uniform vec2 u_mouse;       // mouse position in screen pixels
uniform float u_time;       // Time in seconds since load

uniform vec2 u_playerPosition;
uniform float u_playerRadius;

void main()
{
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);

    vec2 normal = gl_FragCoord.xy - u_playerPosition.xy;
    vec2 lightPosition = vec2((u_mouse.x - u_playerPosition.x), -(u_mouse.y - u_playerPosition.y));
    float intensity = smoothstep(0.0, 1.0, dot(normalize(normal), normalize(lightPosition)));
    intensity += smoothstep(0.5, 0.0, length(gl_FragCoord.xy - u_playerPosition) / u_playerRadius - 1.0);
    intensity = max(0.0, min(1.0, intensity));
    gl_FragColor = gl_Color * vec4(pixel.xyz * intensity, pixel.w);
}
