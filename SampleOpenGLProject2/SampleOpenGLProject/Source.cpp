#include "Angel.h"
#define  dotSize   1818

// Designate the four corners of the square

int circleCount = 5;

/* All triangle and circle coordinates are storing in this vec4 vertices  */
vec4 vertices[dotSize];
vec4 colors[dotSize];

float value=0.5;	/* minimize value */

float constant = 0.05;   /* amount of angle increase */
GLfloat rotation = 0;    /* this value for  reshape angle */
GLfloat rotate = 0;      /* it will rotate or not rotate? */
GLfloat changeColor = 0;  /* this color change or not change */
GLuint shaderRotation;	/* location of rotation value for shader*/
GLuint shaderRotate;	/* location of rotate value for shader*/
GLuint endColor;		/* location of color value for shader*/
GLuint program;			/* shader */

/* calculate circle points(x,y,z) and store this coordinates in vertices array with query */
void drawCircle(int start,GLfloat x, GLfloat y, GLfloat radius)
{
	GLfloat twicePi = 2.0f * M_PI;
	GLfloat z = -0.1;
	vertices[start] = { x,y,z,1.0 };
	colors[start] = { 1.0, 1.0, 0.0, 1.0 };

	for (int i = start+1; i < start+360 ; i++)
	{ 
		/* starting value and radius calculate new adress x,y coordinate with cos and sin */
		vertices[i] = { x + (radius * cos(i *  twicePi / 360)) , y + (radius * sin(i * twicePi / 360)), z,1.0 };
		colors[i] = { 1.0, 1.0, 0.0, 1.0 };
	}

}

/* triangle coordinates calculating and then storing in the main array vertices */
void drawTriangle(){
float x = 0;
float y = 0;
	vec4 triangles[18]{
		vec4{  0.5,0.0,0,1.0},vec4{ 1.0,1.0,0,1.0 },vec4{ 0.0,0.5,0,1.0 },
		vec4{  0.0,0.5,0,1.0 },vec4{ -1.0,1.0,0,1.0 },vec4{ -0.5,0.0,0,1.0 },
		vec4{ -0.5,0.0,0,1.0 },vec4{ -1.0,-1.0,0,1.0 },vec4{ 0.0,-0.5,0,1.0 },
		vec4{ 0.0,-0.5,0,1.0 },vec4{ 1.0,-1.0,0,1.0 },vec4{ 0.5,0,0,1.0 },
		vec4{ -0.5,0.0,0,1.0 },vec4{ 0.0,-0.5,0,1.0 },vec4{ 0.5,0,0,1.0 },
		vec4{ -0.5,0.0,0,1.0 },vec4{ 0.0, 0.5,0,1.0 },vec4{ 0.5,0,0,1.0 }
	};
	for (int i = 0; i < 18; i++) {
		float z = 0.1;
		vertices[1800 + i] = { triangles[i].x * value,triangles[i].y * value,z,1.0 };
		colors[1800 + i] = { 0.5, 0.5, 0.5, 1.0 };
	}
}

void drawObject() {
	
	/* Triangle coordinates calculating   */
	drawTriangle();

	/* circles coordinates calculating with start x,start y and radius value  */
	drawCircle(0, 0 , 0 , 0.1*value);
	drawCircle(360, 0.5*value, 0 , 0.1*value);
	drawCircle(720, -0.5*value, 0 , 0.1*value);
	drawCircle(1080, 0, 0.5*value, 0.1*value);
	drawCircle(1440, 0, -0.5*value, 0.1*value);

}

void init()
{ 

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// Load shaders and use the resulting shader program
	program = InitShader("vertex.glsl", "fragment.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,BUFFER_OFFSET(0));

	// set up color arrays
	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	/* get location adress of variables */
	shaderRotation = glGetUniformLocation(program, "rotation");
	shaderRotate = glGetUniformLocation(program, "rotate");
	endColor = glGetUniformLocation(program, "changeColor");

}
void display(void) {
	// Paint the background gray
	glClearColor(1.0, 1.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawObject();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)+sizeof(colors) ,NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0, sizeof(vertices),vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
	/* send the values to vertex shader for changing color and rotate */
	glUniform1f(shaderRotation, rotation);    
	glUniform1f(shaderRotate, rotate);
	glUniform1f(endColor, changeColor);

	/* circles are drawing */
	for (int i = 0; i < circleCount; i++) {
		glDrawArrays(GL_TRIANGLE_FAN, i*360, 360);
	}

	/* Triangles are drawing */
	for (int j = 0; j < 6; j++) {
		glDrawArrays(GL_TRIANGLES, 1800+(j * 3), 3);
	}
	glFlush();
}

/* if key==1 object = 0.5 size
*     key==2 obcject = rotate
*	  key==3 object  = rotate and draw the dark when it was middle
*/
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(EXIT_SUCCESS);
		break;
	case 49:
		changeColor = 0;	/* not rotate,not draw darker, just minimize */
		rotate = 0;
		value = 0.5;
		break;
	case 50:
		changeColor = 0;   /* stop change color just rotate */
		rotate = 1;
		break;
	case 51:
		changeColor = 1;	/* change color and rotate */
		rotate = 1;
		break;
	}
}

/* This function change the angle and call the redisplay function */
void small(int) {

	if (rotate==1) {
		if (rotation >= 0.9 || rotation <= -0.9) {
			constant = -constant;    /* change the direction of primitives */
		}
		rotation += constant;
	}
	glutPostRedisplay();
	glutTimerFunc(50, small, 0);   /* for see the rotation sleep a little */
}



/*main fonksiyonu*/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(512,512);
	glutInitWindowPosition(400, 200);

	glutCreateWindow("My window");
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glewExperimental = GL_TRUE;

	glewInit();

	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glutTimerFunc(0, small, 0);

	glutMainLoop();
	return 0;
}

