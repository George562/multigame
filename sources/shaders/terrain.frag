#ifdef GL_ES
precision mediump float;
#endif

varying vec3 normal;
uniform float lightFactor;

uniform float offset;
uniform vec3 point;
uniform vec4 color;
uniform mat4 matrix;
uniform sampler2D overlay;
uniform sampler2D current;

uniform vec2 u_resolution;  // Canvas size (width,height)
uniform vec2 u_mouse;       // mouse position in screen pixels
uniform float u_time;       // Time in seconds since load

void main()
{
    vec3 lightPosition = vec3(0.0, 1.0, 2.0);
    vec3 eyePosition = vec3(0.0, 0.0, 1.0);
    vec3 halfVector = normalize(lightPosition + eyePosition);
    float intensity = 1 + (1 - lightFactor) * dot(normalize(normal), normalize(halfVector));
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy) * intensity;
    gl_FragColor = gl_Color * pixel;
}
