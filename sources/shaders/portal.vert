uniform float u_time; // Time in seconds since load

void main() {
    vec4 vertex = gl_Vertex;
    vertex.x += cos(gl_Vertex.y * 0.02 + u_time * 3.8) * 3
              + sin(gl_Vertex.y * 0.02 + u_time * 6.3) * 9;
    vertex.y += sin(gl_Vertex.x * 0.02 + u_time * 2.4) * 50
              + cos(gl_Vertex.x * 0.02 + u_time * 5.2) * 15;

    gl_Position = gl_ModelViewProjectionMatrix * vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}
