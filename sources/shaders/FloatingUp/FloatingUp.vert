uniform float uTime; // Time in seconds since load

void main() {
    vec4 vertex = gl_Vertex;
    vertex.y -= 75. * uTime;

    gl_Position = gl_ModelViewProjectionMatrix * vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}
