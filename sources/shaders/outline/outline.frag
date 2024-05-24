uniform sampler2D overlay;

uniform vec2 uResolution;  // Canvas size (width, height)

void main() {
    vec4 pixel = texture2D(overlay, gl_TexCoord[0].xy);
	
	float width = 3.;

	if (pixel.a == 0.) {
		for (float y = -width; y <= width; y++) {
			for (float x = -width; x <= width; x++) {
				if (texture(overlay, gl_TexCoord[0].xy + vec2(x, y) / uResolution).a != 0.0) {
					pixel = vec4(0., 255., 0., 255.);
				}
			}
		}
	}
    gl_FragColor = gl_Color * pixel;
}
