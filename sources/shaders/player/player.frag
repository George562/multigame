uniform vec2 direction;

#define PI 3.14159265359
#define TWO_PI 6.28318530718

#define rot(a) mat2(cos(a), -sin(a), sin(a), cos(a))
#define rotate(uv, a) (uv) * rot(a)
#define rotateAround(uv, pos, angle) (uv - pos) * rot(angle) + pos

float Circle(in vec2 uv, in float r, in float R) {
    return smoothstep(R, 0., abs(length(uv) - r));
}

float rect(in vec2 uv, vec2 pos, vec2 size, float angle) {
    uv = rotate(uv - pos, angle);
    return 1. - smoothstep(pow(length(max(2. * abs(uv - vec2(0., size.y / 2.)) - size, 0.)), 1.5), 0., 0.0011);
}

void main() {
    vec2 uv = (gl_TexCoord[0].xy - 0.5) * 0.3;
    uv *= rot(atan(direction.x, direction.y));
    float result = Circle(uv, 0.1, 0.01)
                 + Circle(uv, 0.12, 0.02)
                 + Circle(uv, 0.033, 0.01) * 2.
                 + rect(vec2(abs(uv.x), uv.y), vec2(0.027), vec2(0.01, 0.055), PI / 4.)
    ;
    gl_FragColor = vec4(vec3(result), step(length(uv), 0.1) + step(0.1, length(uv)) * result);
}
