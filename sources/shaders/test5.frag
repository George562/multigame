#define PI 3.14159265359
#define TWO_PI 6.28318530718

#define rot(a) mat2(cos(a), -sin(a), sin(a), cos(a))
#define rotate(uv, a) (uv) * rot(a)
#define rotateAround(uv, pos, angle) (uv - pos) * rot(angle) + pos

float angles = 4.;

float Figure(in vec2 uv, in float r, float phi ) {
    uv *= rot(phi);
    float a = atan(uv.x, uv.y) + PI;
    float k = TWO_PI / angles;
    float d = cos(floor(0.5 + a / k) * k - a) * length(uv);
    return smoothstep(0.01, 0., d - r);
}

void main() {
    vec2 uv = (gl_FragCoord.xy / iResolution.xy - 0.5) * (0.3 + 0.2 * step(angles, 3.));
    float result = Figure(uv, 0.1, 0.)
                 - Figure(uv, 0.09, 0.)
                 + Figure(uv, 0.08, 0.)
                 - Figure(uv, 0.07, 0.);
    gl_FragColor = vec4(result);
}
