uniform float uTime; // Time in seconds since load
#define M_PI       3.14159265358979323846   // pi

void main() {
    vec4 vertex = gl_Vertex;
    // vertex.x += 7 *  cos(uTime * 3.);
    // vertex.y += 10 * sin(uTime * 3. + M_PI / 8);

    gl_Position = gl_ModelViewProjectionMatrix * vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}
