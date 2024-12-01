uniform float uTime;

float PI = acos(-1.);

#define rot(a) mat2(cos(a), -sin(a), sin(a), cos(a))
#define rotate(uv, a) (uv) * rot(a)
#define rotateAround(uv, pos, angle) (uv - pos) * rot(angle) + pos

float rectangle(in vec2 uv, vec2 pos, vec2 size, float angle) {
    uv = rotate(uv - pos, angle);
    pos = vec2(-size.x / 2.,  0.);
    vec2 res = smoothstep(-0.008, 0., uv - pos) - smoothstep(0., -0.008, -uv + (pos + size));
    return res.x * res.y;
}

float rect(in vec2 uv, vec2 pos, vec2 size, float angle) {
    uv = rotate(uv - pos, angle);
    return 1. - smoothstep(pow(length(max(2. * abs(uv - vec2(0., size.y / 2.)) - size, 0.)), 1.65), 0., 0.0011);
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
float f(float x) {
    return max(pow(2., -10. * abs(x)) + 0.7 * sin(1.3 * x), 0.);
}

void main() {
    vec2 uv = (gl_TexCoord[0].xy - 0.5) * 1.45;
    float d = length(uv);
    float time = 0.15 + pow(sin(0.45 * uTime), 2.) / 5.;
    float result = 0.9 * (1. - step(d, 0.36));
    vec2 Muv = rotateAround(abs(uv), vec2(0.25, 0.25), PI / 4.);
    result -= tree(Muv, vec2(0.25, 0.27), vec2(0.08, 0.14), PI * time, 4);
    // result -= rect(uv, vec2(-0.249), vec2(0.08, 0.7), PI / 4.);
    // result -= rect(uv, vec2(-0.249, 0.249), vec2(0.08, 0.7), PI * 3. / 4.);
    result = result * step(result, 0.75) + (1. - step(result, 0.75));
    result += (1. - smoothstep(d, 0.36, (0.9 + 0.15 * sin(4. * atan(uv.x, uv.y) - PI / 2.)) * (0.25 + 0.08 * f(3.2 * fract(uTime) - 0.29)))) * step(d, 0.34);
    result -= (1. - smoothstep(0.36, 0.37, d)) * (1. - step(d, 0.34));
    vec3 color = vec3(1. - result) - vec3(0., 1., 1.) * step(d, 0.36);
    gl_FragColor = vec4(color, max(1. - result, 1. - smoothstep(0.36, 0.37, d)));
}
