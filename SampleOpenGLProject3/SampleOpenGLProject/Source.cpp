#include "Angel.h"
#include "glui.h"

#define  dotSize   1818

// Designate the four corners of the square

int circleCount = 5;

/* All triangle and circle coordinates are storing in this vec4 vertices  */
vec4 vertices[dotSize];
vec4 colors[dotSize];


/* button's return values for checking the which operation will be run*/
#define START 0  
#define STOP 1
#define START1 2 
#define STOP1 3
#define START2 4  
#define STOP2 5
#define increase 6
#define dicrease 7
#define increase1 8
#define dicrease1 9
#define EDITTEXT 10
#define EDITTEXT1 11

GLUI *glui;

int main_window;			// main window

float speed = 0.5;    // for the control speed

GLUI_EditText *text;
GLUI_EditText *text2;


/* This values for the rotation animation */
GLfloat rotation = 0;    /* this value for  reshape angle */
GLfloat rotate = 1;      /* it will rotate or not rotate? */
GLuint shaderRotation;	/* location of rotation value for shader*/
GLuint shaderRotate;	/* location of rotate value for shader*/
float constant = -1.0;		 /* amount of angle increase */

/*  This values for the scale animation */
GLuint Scaling=1;			/* it is scaling? */
GLuint Scale;				/*  for shader */
GLuint ScaleNumber;			/* for shader constant */
float scaleConstant = -0.01;	/* speed value */

/* This values for the spiral animation */
GLfloat isSpiral = 1;   /* if it is 1,start the spiral animation.if it is 0,stop the animation */
GLuint AngleSpiral;
GLuint Spiral;				/* sending value shader side */
GLuint spiralDirection;		/* shader value's adress */
GLfloat angle = 0.0;	/**/
float direction = 1;     /* -1 for the negative direction */
float angleConstant = 0.3;  /* speed calculating with this argument */


GLfloat value = 0.5;		/* minimize value */
GLuint program;			/* shader */

						/* calculate circle points(x,y,z) and store this coordinates in vertices array with query */
void drawCircle(int start, GLfloat x, GLfloat y, GLfloat radius)
{
	GLfloat twicePi = 2.0f * M_PI;
	GLfloat z = -0.1;
	vertices[start] = { x,y,z,1.0 };
	colors[start] = { 1.0, 1.0, 0.0, 1.0 };

	for (int i = start + 1; i < start + 360; i++)
	{
		/* starting value and radius calculate new adress x,y coordinate with cos and sin */
		vertices[i] = { x + (radius * cos(i *  twicePi / 360)) , y + (radius * sin(i * twicePi / 360)), z,1.0 };
		colors[i] = { 1.0, 1.0, 0.0, 1.0 };
	}

}

/* triangle coordinates calculating and then storing in the main array vertices */
void drawTriangle() {
	float x = 0;
	float y = 0;
	vec4 triangles[18]{
		vec4{ 0.5,0.0,0,1.0 },vec4{ 1.0,1.0,0,1.0 },vec4{ 0.0,0.5,0,1.0 },
		vec4{ 0.0,0.5,0,1.0 },vec4{ -1.0,1.0,0,1.0 },vec4{ -0.5,0.0,0,1.0 },
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
	drawCircle(0, 0, 0, 0.1*value);
	drawCircle(360, 0.5*value, 0, 0.1*value);
	drawCircle(720, -0.5*value, 0, 0.1*value);
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
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// set up color arrays
	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

	/* get location adress of variables */
	shaderRotation = glGetUniformLocation(program, "rotation");
	shaderRotate = glGetUniformLocation(program, "rotate");
	Scale = glGetUniformLocation(program, "Scale"); 
	ScaleNumber = glGetUniformLocation(program, "ScaleNumber");
	AngleSpiral = glGetUniformLocation(program, "AngleSpiral");
	Spiral = glGetUniformLocation(program, "Spiral"); 
	spiralDirection = glGetUniformLocation(program, "spiralDirection");
}
void display(void) {
	// Paint the background gray
	glClearColor(1.0, 1.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawObject();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
	/* send the values to vertex shader for changing color and rotate */
	glUniform1f(shaderRotation, rotation);
	glUniform1f(shaderRotate, rotate);
	glUniform1f(Scale, Scaling);
	glUniform1f(ScaleNumber, value);
	glUniform1f(AngleSpiral, angle);
	glUniform1f(Spiral, isSpiral);
	glUniform1f(spiralDirection, direction);

	/* circles are drawing */
	for (int i = 0; i < circleCount; i++) {
		glDrawArrays(GL_TRIANGLE_FAN, i * 360, 360);
	}

	/* Triangles are drawing */
	for (int j = 0; j < 6; j++) {
		glDrawArrays(GL_TRIANGLES, 1800 + (j * 3), 3);
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

/* This function change the angle and call the redisplay function */
void idle() {
	glutSetWindow(main_window);
	if (rotate == 1) {		 /* if rotate is 1,start button is pressed and animation start */
		if (rotation <= -360) {					/* return the beginning */
			rotation +=360 ;    
		}
		rotation += constant;					/* increase the rotation angle */
	}
	if (Scaling == 1) {					/* if scaling is 1,start button is pressed and animation start */
		if (value <= 0.5 || value >= 1.0) {    /* just between the 0.5 and 1.5  */
			scaleConstant = -scaleConstant;     /* first increase and then decrease */
		}
		value += scaleConstant;	
	}
	if (isSpiral == 1) {				/* if isSpiral is 1,start button is pressed and animation start */
		if (angle >= 450) {
			angle = -angle;			/* change the direction of primitives */
		}
		if (angleConstant < 0) {		/* if dicrease button take the number negative,change the direction value with -1*/
			angle -= angleConstant;
			direction = -1;
		}
		else if(angleConstant > 0) {			/* if it become again positive,change the direction value with 1 */
			angle += angleConstant;
			direction = +1;
		}
		else {
			direction = 1;
		}
	}

	glutPostRedisplay();
	Sleep(1);
}


void glui_callback(int control) {
	switch (control) {
	case START:			/* first part of assignment: Rotate the start counter clockwise direction's start button */
		rotate = 1;
		break;
	case STOP:			/* stop button of rotate */
		rotate = 0;
		break;
	case increase:      /* increase speed of first part*/
		if (constant > -10.0) {
			constant -= 1.0;
			text->set_float_val(constant);  /* write the speed value on the text */
		}
		break;
	case dicrease:		 /* dicrease speed of first part  */
		if (constant < 10.0) {
			constant += 1.0;
			text->set_float_val(constant);  /* write the speed value on the text */
		}
		break;

	case START1:		/* second part of assignment: Make the star grow and shrink in size like a beating heart  */
		Scaling = 1;
		break;
	case STOP1:		/* stop the beating heart animation */
		Scaling = 0;
		break;

	case START2:			/* third part of assignment: Make the star continuously rotate in clockwise direction with spiral path */
		isSpiral = 1;
		break;
	case STOP2:				/* stop the spiral animation */
		isSpiral = 0;
		break;
	case increase1:      /* increase speed of spiral animation */
			if (angleConstant < 1.0) {
				angleConstant += 0.1;
				text2->set_float_val(angleConstant);  /* write the speed value on the text */
				}
		break;
	case dicrease1:		 /* dicrease speed of spiral animation */
			if (angleConstant > -1.0) {
				angleConstant -= 0.1;
				text2->set_float_val(angleConstant);  /* write the speed value on the text */
			   }
		break;
	}
}


/*main fonksiyonu*/
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(400, 200);
	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glewExperimental = GL_TRUE;
	/* decide the main window */
	main_window = glutCreateWindow("My window");

	/* create glui screen */
	glui = GLUI_Master.create_glui("First GLUI");

	/* create selection panel for buttons and column */
	GLUI_Panel *selection = glui->add_panel("First-Part");

	/* rotate buttons on the rotate panel */
	GLUI_Panel *panel = glui->add_panel_to_panel(selection, "Rotate counter clockwise");
	glui->add_button_to_panel(panel, "Start", START, glui_callback);
	glui->add_button_to_panel(panel, "Stop", STOP, glui_callback);

	/* column on the selection panel */
	glui->add_column_to_panel(selection, false);

	/* change speed buttons on the panel2 */
	GLUI_Panel *panel2 = glui->add_panel_to_panel(selection, "Speed");
	glui->add_button_to_panel(panel2, "Increase", increase, glui_callback);
	glui->add_button_to_panel(panel2, "Dicrease", dicrease, glui_callback);

	/* column on the selection panel */
	glui->add_column_to_panel(selection, false);

	/* for write speed value on the screen */
	text = glui->add_edittext_to_panel(selection, "Speed:", GLUI_EDITTEXT_FLOAT, &constant, EDITTEXT, glui_callback);

	/* for seperator  */
	glui->add_separator();

	/* create selection panel for buttons and column */
	GLUI_Panel *selection2 = glui->add_panel("Second-Part");

	/* rotate buttons on the rotate panel3 */
	GLUI_Panel *panel3 = glui->add_panel_to_panel(selection2, "Scaling");
	glui->add_button_to_panel(panel3, "Start", START1, glui_callback);
	glui->add_button_to_panel(panel3, "Stop", STOP1, glui_callback);
	
	glui->add_separator();

	/* create selection panel for buttons and column */
	GLUI_Panel *selection3 = glui->add_panel("Third-Part");

	/* rotate buttons on the rotate panel */
	GLUI_Panel *panel4 = glui->add_panel_to_panel(selection3, "Rotate clockwise");
	glui->add_button_to_panel(panel4, "Start", START2, glui_callback);
	glui->add_button_to_panel(panel4, "Stop", STOP2, glui_callback);

	/* column on the selection panel */
	glui->add_column_to_panel(selection3, false);

	/* change speed buttons on the panel2 */
	GLUI_Panel *panel5 = glui->add_panel_to_panel(selection3, "Speed");
	glui->add_button_to_panel(panel5, "Increase", increase1, glui_callback);
	glui->add_button_to_panel(panel5, "Dicrease", dicrease1, glui_callback);

	/* column on the selection panel */
	glui->add_column_to_panel(selection3, false);

	/*for write speed of spiral animation on the screen */
	text2 = glui->add_edittext_to_panel(selection3, "Speed:", GLUI_EDITTEXT_FLOAT, &angleConstant, EDITTEXT1, glui_callback);

	/* main window */
	glui->set_main_gfx_window(main_window);

	GLUI_Master.auto_set_viewport();

	/* idle function is calling from the glui */
	GLUI_Master.set_glutIdleFunc(idle);

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glutMainLoop();
	return 0;


}







