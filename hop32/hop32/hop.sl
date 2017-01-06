#define GLSL(str) #str

const char *VertexShader = GLSL(
	attribute vec2 a_position;\n
	attribute vec2 a_texCoord;\n

	varying vec2 v_texCoord;\n

	void main()\n
	{\n
		gl_Position = vec4(a_position, 0.0, 1.0);\n
		v_texCoord = a_texCoord;\n
	}\n
);

const char *FragmentShader = GLSL(
	uniform sampler2D u_texture;\n
	varying vec2 v_texCoord;\n

	void main()\n
	{\n
		gl_FragColor = texture2D(u_texture, v_texCoord);\n
	}\n
);