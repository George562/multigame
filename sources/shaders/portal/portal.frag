uniform sampler2D overlay;

void main() {
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);

    gl_FragColor = gl_Color * pixel;
}
