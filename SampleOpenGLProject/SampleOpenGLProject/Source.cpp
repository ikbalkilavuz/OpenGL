#include "Angel.h"
#define  dotSize   1812

// Designate the four corners of the square

int circleCount = 5;

/* All triangle and circle coordinates are storing in this vec4 vertices  */
vec4 vertices[dotSize];


/* calculate circle points(x,y,z) and store this coordinates in vertices array with query */
void drawCircle(int start,GLfloat x, GLfloat y, GLfloat radius)
{
	GLfloat twicePi = 2.0f * M_PI;
	GLfloat z = 0.0;

	vertices[start] = { x,y,z,1.0 };

	for (int i = start+1; i < start+360 ; i++)
	{ 
		/* starting value and radius calculate new adress x,y coordinate with cos and sin */
		vertices[i] = { x + (radius * cos(i *  twicePi / 360)) , y + (radius * sin(i * twicePi / 360)), z,1.0 };
	}

}

/* triangle coordinates calculating and then storing in the main array vertices */
void drawTriangle() {
	vec4 triangles[12]{
		vec4{1.0,1.0,0,1.0},vec4{ 0,0.5,0,1.0 },vec4{ -1.0,1.0,0,1.0 },
		vec4{ -1.0,1.0,0,1.0 },vec4{ -0.5,0,0,1.0 },vec4{ -1.0,-1.0,0,1.0 },
		vec4{ -1.0,-1.0,0,1.0 },vec4{ 0,-0.5,0,1.0 },vec4{ +1.0,-1.0,0,1.0 },
		vec4{ +1.0,-1.0,0,1.0 },vec4{ 0.5,0,0,1.0 },vec4{ 1.0,1.0,0,1.0 }
	};
	for (int i = 0; i < 12; i++) {
		vertices[1800 + i] = triangles[i];
	}
}

void init()
{ 
	/* circles coordinates calculating with start x,start y and radius value  */
	drawCircle(0,0, 0, 0.1);    
	drawCircle(360, 0.5, 0, 0.1);	
	drawCircle(720, -0.5, 0, 0.1);	
	drawCircle(1080, 0, 0.5, 0.1);	
	drawCircle(1440, 0, -0.5, 0.1);	

	/* Triangle oordinates calculating   */
	drawTriangle();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices , GL_STATIC_DRAW);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vertex.glsl", "fragment.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	
}
void display(void) {
	// Paint the background gray
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	/* circles are drawing */
	for (int i = 0; i < circleCount; i++) {
		glDrawArrays(GL_TRIANGLE_FAN, i*360, 360);
	}

	/* Triangles are drawing */
	for (int j = 0; j < 4; j++) {
		glDrawArrays(GL_TRIANGLES, 1800+(j * 3), 3);
	}
	glFlush();
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(EXIT_SUCCESS);
		break;
	}
}

/*main fonksiyonu*/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(512,512);
	glutInitWindowPosition(500, 200);

	glutCreateWindow("My window");
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glewExperimental = GL_TRUE;

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

