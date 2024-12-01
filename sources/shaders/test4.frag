float PI = 3.14159265359;

#define rot(a) mat2(cos(a), -sin(a), sin(a), cos(a))
#define rotate(uv, a) (uv) * rot(a)
#define rotateAround(uv, pos, angle) (uv - pos) * rot(angle) + pos

float rectangle(in vec2 uv, vec2 pos, vec2 size, float angle) {
    uv = rotate(uv - pos, angle);
    pos = vec2(-size.x / 2.,  0.);
    vec2 res = smoothstep(-0.004, 0., uv - pos) - smoothstep(0., -0.004, -uv + (pos + size));
    return res.x * res.y;
}

float rect(in vec2 uv, vec2 pos, vec2 size, float angle) {
    uv = rotate(uv - pos, angle);
    return 1. - smoothstep(length(max(2. * abs(uv - vec2(0., size.y / 2.)) - size, 0.)), 0., 0.006);
}

float branch(in vec2 uv, vec2 pos, vec2 size, float angle) {
    float result = 0.;
    result += rect(uv, pos, size, angle);
    result += rect(uv, pos, size, -angle);
    return result;
}

float d[10];
float tree(in vec2 uv, vec2 pos, vec2 size, float angle, int deep) {
    for (int i = 0; i < 10; i++) { d[i] = 0.; }
    float result = rect(uv, pos, size, 0.);
    pos += vec2(0., size.y);
    size *= 0.75;
    result += branch(uv, pos, size, angle);
    for (int i = 0; d[0] != 2.;) {
        if (i < deep && d[i] == 0.) {
            pos += rotate(vec2(0., size.y), angle);
            uv = rotateAround(uv, pos, -angle);

            size *= 0.75;
            result += branch(uv, pos, size, angle);
            i++;
        } else if (i < deep && d[i] == 1.) {
            pos += rotate(vec2(0., size.y), -angle);
            uv = rotateAround(uv, pos, angle);

            size *= 0.75;
            result += branch(uv, pos, size, angle);
            i++;
        } else {
            size /= 0.75;
            if (d[i] == 0.) {
                uv = rotateAround(uv, pos, angle);
                pos -= rotate(vec2(0., size.y), angle);
            } else if (d[i] == 2.) {
                uv = rotateAround(uv, pos, -angle);
                pos -= rotate(vec2(0., size.y), angle);
            }
            d[i - 1] += 1.;
            d[i] = 0.;
            i--;
        }
    }
    return result;
}

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution.y;
    float time = 0.15 + pow(sin(0.35 * iTime), 2.) / 5.;
    float result = 0.;
    // result += rectangle(uv, vec2(0.25, 0.25), vec2(0.1, 0.3), 0.);
    // result += rect     (uv, vec2(0.25, 0.25), vec2(0.1, 0.3), 0.);
    // result += rect     (uv, vec2(0.25, 0.25), vec2(0.1, 0.3), 0.);
    // result += rect     (uv, vec2(0.25, 0.25), vec2(0.1, 0.3), 0.);
    result += tree(uv, vec2(iResolution.x / iResolution.y / 2., 0.2), vec2(0.04, 0.18), PI * time, 4);
    gl_FragColor = vec4(result);
}
