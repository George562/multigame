uniform sampler2D overlay;
uniform float u_time; // Time in seconds since load

void main() {
    gl_FragColor = gl_Color * texture2D(overlay, gl_TexCoord[0].xy);
}
