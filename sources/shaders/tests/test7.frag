#define PI 3.14159265359
#define TWO_PI 6.28318530718

#define rot(a) mat2(cos(a), -sin(a), sin(a), cos(a))
#define rotate(uv, a) (uv) * rot(a)
#define rotateAround(uv, pos, angle) (uv - pos) * rot(angle) + pos

float metric1(in vec2 uv) {
    return abs(uv.x + sin(uv.x)) + abs(uv.y + sin(uv.y));
}
float metric2(in vec2 uv) {
    return abs(uv.x + cos(uv.x)) + abs(uv.y + cos(uv.y));
}

void main() {
    vec2 uv = (gl_FragCoord.xy / iResolution.xy - 0.5) * 40.;
    float m = clamp(iMouse.y / iResolution.y, 0.5, 0.9);
    float col = 0.;
    float time1 = 4.5 + 0.4 * sin(iTime * 1.5);

    col += smoothstep(1.2 + time1, 0.7 + time1, metric1(uv)) * 1.25;
    col -= smoothstep(0.7 + time1, 0.8, metric1(uv)) * 2.;

    // col += smoothstep(-0.3 + time, -0.9 + time, metric2(uv)) * 1.25;
    // col -= smoothstep(-0.9 + time, -0.3, metric2(uv)) * 2.;

    gl_FragColor = vec4(vec3(col), 1.0);
}
