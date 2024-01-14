uniform float u_time; // Time in seconds since load
#define M_PI       3.14159265358979323846   // pi

void main() {
    vec4 vertex = gl_Vertex;
    vertex.x += 7 *  cos(u_time * 3.);
    vertex.y += 10 * sin(u_time * 3. + M_PI / 8) * cos(2. * u_time * 3. - M_PI / 8);
    // Visualizing of move https://www.desmos.com/calculator/r5z5nlhbjg

    gl_Position = gl_ModelViewProjectionMatrix * vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}
