varying vec3 normal;
uniform float lightFactor;
uniform sampler2D texture;

void main()
{
    vec3 lightPosition = vec3(0.0, 2.0, 2.0);
    vec3 eyePosition = vec3(0.0, 0.0, 1.0);
    vec3 halfVector = normalize(lightPosition + eyePosition);
    float intensity = lightFactor + (1.0 - lightFactor) * dot(normalize(normal), normalize(halfVector));
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy) * intensity;
    gl_FragColor = gl_Color * pixel;
}
