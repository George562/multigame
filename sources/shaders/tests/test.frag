#iChannel0 "file://../../textures/Player.png"

// vec3 pallete(float t) {
//     vec3 a = vec3(0.938, 0.328, 0.718);
//     vec3 b = vec3(0.659, -3.142, 0.328);
//     vec3 c = vec3(-3.142, -3.142, -3.142);
//     vec3 d = vec3(-0.692, -2.032, -0.802);

//     return a + b * cos(6.28318 * (c * t + d));
// }

// void main() {
//     vec2 uv = gl_FragCoord.xy / iResolution.xy;
//     vec4 pixel = texture2D(iChannel0, uv);
//     gl_FragColor = vec4(mix(pixel.xyz, pallete(iTime * 0.1), clamp(abs(sin(iTime * 3.)), 0.0, 0.3)), pixel.w);
// }

vec3 pallete(float t) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);

    return a + b * cos(6.28318 * (c * t + d));
}

vec3 redColor = vec3(1.0, 0.0, 0.0);
vec3 blueColor = vec3(0.0, 0.0, 1.0);
float PI = 3.14159265359;
float PI2 = 3.14159265359 * 2.0;

void main() {
    vec2 uv = (gl_FragCoord.xy - iResolution.xy / 2.) / iResolution.xy;

    float d = length(uv);
    float phi = atan(uv.y, uv.x);
    // phi += exp(fract(iTime / 20.0) * 3.7);
    phi += iTime;

    vec3 col = pallete(sin(length(uv)) + iTime * 0.3);
    // vec3 col = vec3(smoothstep(0.55, 0.85, cos(length(uv)*50.0)));
    // vec3 col = vec3(smoothstep(0.55, 0.85, cos(length(uv)*20. + 3. * sin(iTime * 2.))));
    float r = 0.45;
    // vec3 col = mix(blueColor, redColor, smoothstep(PI + 0.02, PI - 0.02, PI2 * fract(9.0 * phi / PI2)));
    col *= smoothstep(r + 0.001, r - 0.001, length(uv));
    gl_FragColor = vec4(col, 1.0);
    // gl_FragColor = col;
}