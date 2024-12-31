#define PI 3.14159265359
#define TWO_PI 6.28318530718

#define rot(a) mat2(cos(a), -sin(a), sin(a), cos(a))
#define rotate(uv, a) (uv) * rot(a)
#define rotateAround(uv, pos, angle) (uv - pos) * rot(angle) + pos

float random(float x) { return fract(sin(x * 1273.1637) * 43758.5453); }
float random(vec2 p) { return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453); }

float noise (in float x, float m) {
    float i = floor(x);
    float u = smoothstep(0.2, 0.8, fract(x));
    return mix(random(mod(i, m)), random(mod(i + 1.0, m)), u);
}
float noise (in vec2 uv) {
    vec2 i = floor(uv);
    vec2 u = smoothstep(0.2, 0.8, fract(uv));
    return mix(
        mix(random(i), random(i + vec2(1.0, 0.0)), u.x),
        mix(random(i + vec2(0.0, 1.0)), random(i + vec2(1.0, 1.0)), u.x), u.y);
}

float rectangle(in vec2 uv, vec2 pos, vec2 size, float angle) {
    uv = rotate(uv - pos, angle);
    vec2 res = smoothstep(0.008, 0., abs(uv - vec2(0.,  size.y / 2.)) - size / 2.);
    return res.x * res.y;
}

float Figure(in vec2 uv, in float r, float phi, float angles) {
    uv *= rot(phi);
    float a = atan(uv.x, uv.y) + PI;
    float k = TWO_PI / angles;
    float d = cos(floor(0.5 + a / k) * k - a) * length(uv);
    return smoothstep(0.01, 0., d - r);
}

float ChristmasTree(in vec2 uv, vec2 pos, float size) {
    uv -= pos;
    float res = 0.;
    for (float i = 0.; i < 3.; i++) {
        res += Figure(uv, size, 0., 3.);
        uv.y += 0.1;
    }
    return min(res, 1.);
}

float littleFlame(in vec2 uv, vec2 pos, float size) { 
    float res = 0.;
    uv -= pos;
    uv *= 20.;
    res += length(vec2(uv.x, abs(uv.y) + size));
    return smoothstep(1.1, 1., res);
}

vec4 garland(in vec2 uv, vec2 pos, float amount) {
    uv -= pos;
    float time = mod(floor(-iTime), 3.);
    vec4 color = vec4(0.);
    for (float i = 0.; i < amount; i ++) {
        color.rgb += littleFlame(uv, vec2(0.1 * i, 0.), 0.37);
        if (mod(time + i, 3.) == 0.) {
            color.gb -= littleFlame(uv, vec2(0.1 * i, 0.), 0.5);
        } else if (mod(time + i, 3.) == 1.) {
            color.rb -= littleFlame(uv, vec2(0.1 * i, 0.), 0.5);
        } else {
            color.rg -= littleFlame(uv, vec2(0.1 * i, 0.), 0.5);
        }
    }
    color.a = max(max(color.r, color.g), color.b);
    return color;
}

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution.yy;
    vec4 color = vec4(0.);
    color.g += ChristmasTree(uv, vec2(0.5, 0.53), 0.15) * 0.5;
    color += ChristmasTree(uv, vec2(0.5, 0.53), 0.16) * smoothstep(0.4, 0., color.g);
    color.xyz += vec3(0.4, 0.2, 0.1) * rectangle(uv, vec2(0.5, 0.03), vec2(0.2, 0.13), 0.);
    float zvezda = min(Figure(uv - vec2(0.5, 0.86), 0.055, 0., 3.) + Figure(uv - vec2(0.5, 0.86), 0.055, PI, 3.), 1.);
    color.rgb = mix(color.rgb, vec3(1.), zvezda);
    zvezda = Figure(uv - vec2(0.5, 0.86), 0.05, 0., 3.) + Figure(uv - vec2(0.5, 0.86), 0.05, PI, 3.);
    color.rgb = vec3(mix(color.rg, vec2(1.), zvezda), mix(color.b, 0., zvezda));
    vec4 g = vec4(0.);
    g += garland(rotateAround(uv, vec2(0.5), PI / 9.), vec2(0.38, 0.64), 3.);
    g += garland(rotateAround(uv, vec2(0.5), PI / 9.), vec2(0.35, 0.44), 5.);
    g += garland(rotateAround(uv, vec2(0.5), PI / 9.), vec2(0.37, 0.24), 4.);
    color = mix(color, g, g.a);
    gl_FragColor = color;
}
