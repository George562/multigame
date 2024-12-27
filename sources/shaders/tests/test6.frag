#define PI 3.14159265359
#define TWO_PI 6.28318530718

#define rot(a) mat2(cos(a), -sin(a), sin(a), cos(a))
#define rotate(uv, a) (uv) * rot(a)
#define rotateAround(uv, pos, angle) (uv - pos) * rot(angle) + pos

float rect(in vec2 uv, vec2 pos, vec2 size, float angle) {
    uv = rotate(uv - pos, angle);
    return 1. - smoothstep(length(max(2. * abs(uv - vec2(0., size.y / 2.)) - size, 0.)), 0., 0.002);
}

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    float m = clamp(iMouse.y / iResolution.y, 0.5, 0.9);
    uv.x = abs(uv.x - 0.501);

    float col = 0.;
    col += rect(uv, vec2(0.492, 0.115), vec2(0.005, m - 0.2), 0.);
    col += rect(uv, vec2(-0.01, 0.16), vec2(0.005, 0.51), atan(0.5, -0.05));
    col += rect(uv, vec2(-0.01, m + 0.025), vec2(0.005, 0.52), atan(0.5, -0.1));

    col += smoothstep(m - 0.2, m, uv.y) * step(0.1 / 0.5 * uv.x, m + 0.02 - uv.y);

    col += rect(uv, vec2(0., m + 0.045), vec2(0.01, 0.55), atan(0.5, -0.1));

    col += rect(uv, vec2(0., 0.14), vec2(0.005, 0.51), atan(0.5, -0.05));
    col += rect(uv, vec2(0.492, 0.06), vec2(0.005, 0.03), 0.);
    col += rect(uv, vec2(0., 0.11), vec2(0.005, 0.51), atan(0.5, -0.05));

    col += rect(uv, vec2(0., 0.09), vec2(0.005, 0.51), atan(0.5, -0.05));
    col += rect(uv, vec2(0.492, 0.01), vec2(0.005, 0.03), 0.);
    col += rect(uv, vec2(0., 0.06), vec2(0.005, 0.51), atan(0.5, -0.05));

    gl_FragColor = vec4(vec3(col),1.0);
}
