vec2 hash(vec2 p) {
    return fract(sin(vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)))) * 43758.5453);
}

float voronoi(vec2 x) {
    vec2 n = floor(x), f = fract(x);
    float distance_type = 1., F1 = 8.0;

    for (int j = -1; j <= 1; j++) {
        for(int i = -1; i <= 1; i++) {
            vec2 g = vec2(i, j);
            vec2 o = hash(n + g);
            // o = 0.5 + 0.41 * sin(iTime + 6.2831 * o); // animate
            vec2 r = g - f + o;
            float d = distance_type < 1.0 ? dot(r, r)  :          // euclidean^2
                      distance_type < 2.0 ? sqrt(dot(r, r)) :     // euclidean
                      distance_type < 3.0 ? abs(r.x) + abs(r.y) : // manhattan
                      max(abs(r.x), abs(r.y));                    // chebyshev

            if (d < F1) F1 = d;
        }
    }
    return F1;
}

vec2 twirl(vec2 UV, vec2 Center, float Strength, vec2 Offset) {
    vec2 delta = UV - Center;
    float angle = Strength * length(delta);
    vec2 uv = delta * mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
    return uv + Center + Offset;
}

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution.xy - vec2(0.5);
    float time = iTime * 0.5;
    vec2 Muv = uv * vec2(6., 6.);
    Muv = twirl(Muv, vec2(0., 0.), 1.95, vec2(-time));

    vec4 color = vec4(1., 0., 1., 1.);
    float param = voronoi(Muv);
    param *= smoothstep(0.1, 0.3, length(uv));
    param *= smoothstep(0.95, 0.75, 2. * length(uv));
    gl_FragColor = color * param;
}
