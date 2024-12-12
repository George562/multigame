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

float Circle(in vec2 uv, in float r, in float R) {
    return smoothstep(R, 0., abs(length(uv) - r));
}

float rect(in vec2 uv, vec2 pos, vec2 size, float angle) {
    uv = rotate(uv - pos, angle);
    return 1. - smoothstep(pow(length(max(2. * abs(uv - vec2(0., size.y / 2.)) - size, 0.)), 1.5), 0., 0.0009);
}

void main() {
    vec2 uv = (gl_FragCoord.xy / iResolution.xy - 0.5) * (0.3 + 0.2 * step(angles, 3.));
    float phi = atan(iMouse.x - iResolution.x / 2., iMouse.y - iResolution.y / 2.);
    float d = length(uv);
    uv *= rot(phi);
    float result = rect(vec2(abs(uv.x), uv.y), vec2(0.), vec2(0.004, 0.09), PI / 4.)
                 + smoothstep(0.01, -0.1, d - 0.04)
                //  + rect(uv, vec2(0., -0.023), vec2(-0.009, 0.046), 0.)
    ;
    vec3 color = vec3(50., 2.5, 0.8) * step(d, 0.1);
    // float result = Figure(uv, 0.1, 0.)
    //              - Figure(uv, 0.09, 0.)
    //              + Figure(uv, 0.08, 0.)
    //              - Figure(uv, 0.07, 0.);
    float res = Circle(uv, 0.1, 0.01)
              + Circle(uv, 0.12, 0.02)
    ;
    gl_FragColor = vec4(result * color + res, step(d, 0.1) + step(0.1, d) * (result + res));
}
