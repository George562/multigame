#ifdef GL_ES
precision mediump float;
#endif

uniform float offset;
uniform vec3 point;
uniform vec4 color;
uniform mat4 matrix;
uniform sampler2D overlay;
uniform sampler2D current;

uniform vec2 u_resolution;  // Canvas size (width, height)
uniform vec2 u_mouse;       // mouse position in screen pixels
uniform float u_time;       // Time in seconds since load

void main()
{
    vec3 normal = vec3(gl_FragCoord.xy - u_resolution.xy / 2.0, 1.0);
    vec3 lightPosition = vec3((u_mouse.x - u_resolution.x / 2.0) * 0.05, -(u_mouse.y - u_resolution.y / 2.0) * 0.05, 2.0);
    vec3 eyePosition = vec3(0.0, 0.0, 0.0);
    vec3 halfVector = normalize(lightPosition + eyePosition);
    float intensity = dot(normalize(normal), normalize(halfVector));
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);
    gl_FragColor = gl_Color * vec4(pixel.x * intensity, pixel.y * intensity, pixel.z * intensity, pixel.w);
}
