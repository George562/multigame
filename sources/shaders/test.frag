#iChannel0 "file://../textures/Player.png"

vec3 pallete(float t) {
    vec3 a = vec3(0.938, 0.328, 0.718);
    vec3 b = vec3(0.659, -3.142, 0.328);
    vec3 c = vec3(-3.142, -3.142, -3.142);
    vec3 d = vec3(-0.692, -2.032, -0.802);
    
    return a + b * cos(6.28318 * (c * t + d));
}

void main() {
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    vec4 pixel = texture2D(iChannel0, uv);
    gl_FragColor = vec4(mix(pixel.xyz, pallete(iTime * 0.1), clamp(abs(sin(iTime * 3.)), 0.0, 0.3)), pixel.w);
}