#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef _WIN32
#include <unistd.h>
#else
void usleep(unsigned int nanosec)
{
	Sleep(nanosec / 1000);
}
#endif

//data structures 
typedef struct _Vector {
	double x, y, z;
} Vector;

/******* Global variables *******/
char title[] = "Robot";
double rotAngle = 10;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float PI = 3.14159;
double radian;
double speed = 10;
double rot; //rotated angle 

			//vectors 
Vector pos;
Vector vel;
Vector dir;
Vector dis;
Vector mouseCoords;

// --------------- USER INTERFACE VARIABLES -----------------

// Window settings
int windowID;               // Glut window ID (for display)
GLUI *glui;                 // Glui window (for controls)
int Win[2];                 // window (x,y) size


// ---------------- ANIMATION VARIABLES ---------------------

// Animation settings
int animate_mode = 0;       // 0 = no anim, 1 = animate
int animation_frame = 0;      // Specify current frame of animation

 // ------ Joint Parameters----------//
// Left Side
 // Joint for Upper Arm Left
const float JOINT_MIN_UAL = -45.0f;
const float JOINT_MAX_UAL = 45.0f;
float joint_rot_ual = 0.0f;
// Joint for Lower Arm Left
const float JOINT_MIN_LAL = -10.0f;
const float JOINT_MAX_LAL = 60.0f;
float joint_rot_lal = 0.0f;
// Joint for Hand Left
const float JOINT_MIN_HL = -30.0f;
const float JOINT_MAX_HL = 90.0f;
float joint_rot_hl = 0.0f;
// Joint for Upper Leg Left
const float JOINT_MIN_ULL = -60.0f;
const float JOINT_MAX_ULL = 60.0f;
float joint_rot_ull = 0.0f;
// Joint for Lower Leg Left
const float JOINT_MIN_LLL = -45.0f;
const float JOINT_MAX_LLL = 10.0f;
float joint_rot_lll = 0.0f;
// Joint for Foot Left
const float JOINT_MIN_FL = -15.0f;
const float JOINT_MAX_FL = 40.0f;
float joint_rot_fl = 0.0f;
//
// Right Side
// Joint for Upper Arm Right
const float JOINT_MIN_UAR = -45.0f;
const float JOINT_MAX_UAR = 45.0f;
float joint_rot_uar = 0.0f;
// Joint for Lower Arm Right
const float JOINT_MIN_LAR = -60.0f;
const float JOINT_MAX_LAR = 10.0f;
float joint_rot_lar = 0.0f;
// Joint for Hand Right
const float JOINT_MIN_HR = -90.0f;
const float JOINT_MAX_HR = 30.0f;
float joint_rot_hr = 0.0f;
// Joint for Upper Leg Right
const float JOINT_MIN_ULR = -60.0f;
const float JOINT_MAX_ULR = 60.0f;
float joint_rot_ulr = 0.0f;
// Joint for Lower Leg Right
const float JOINT_MIN_LLR = -45.0f;
const float JOINT_MAX_LLR = 10.0f;
float joint_rot_llr = 0.0f;
// Joint for Foot Right
const float JOINT_MIN_FR = -15.0f;
const float JOINT_MAX_FR = 40.0f;
float joint_rot_fr = 0.0f;


// ***********  FUNCTION HEADER DECLARATIONS ****************
// Initialization functions
void initGlut(char* winName);
void initGlui();
void initGl();

// Callbacks for handling events in glut
void myReshape(int w, int h);
void animate();
void display(void);

// Callback for handling events in glui
void GLUI_Control(int id);

// Functions to help draw the object
void drawHead(float size);
void drawArmUpper(float size);
void drawArmLower(float size);
void drawHand(float size);
void drawTorso(float width);
void drawLegUpper(float width);
void drawLegLower(float width);
void drawFeet(float width);

// Return the current system clock (in seconds)
double getTime();

void keys(unsigned char key, int x, int y) {
	// Exit
	if (key == 27)
		exit(0);
	// Go through frames
	else if (key == 'p') {
		int mod = glutGetModifiers();
		if (mod == GLUT_ACTIVE_ALT)
			animate();
	}
}

void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // save current camera tranform
	glRotated(rotAngle, 0, 1, 0); // rotate by rotAngle
	glEnable(GL_COLOR_MATERIAL); // specify object color
	glOrtho(-SCREEN_WIDTH / 2, SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2, -100, 100);
	glColor3f(0.0, 0.0, 0.1); //red-ish

	glTranslated(pos.x, pos.y, 0);
	glRotatef(rot, 0, 0, 1);

	glPushMatrix();
	glPushMatrix();
	// Robot body
	// side view
	glLoadIdentity();
	glTranslatef(0.0f, 3.0f, -10.0f);
	glRotatef(90, 0, 1, 0);
	glRotatef(10, 0, 0, 1);
	drawHead(1.0);
	glPopMatrix();

	// Left Arm
	// Upper Arm Left Movements
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(joint_rot_ual, 0.0, 0.0, 1.0);
	glScalef(1.0, -1.0, 1.0);
	glTranslatef(0.0f, 0.5f, -9.0f);
	drawArmUpper(1.0);
	//Lower Arm Left
	glScalef(1.0, -1.0, 1.0);
	glTranslatef(0.0f, -1.3f, 0.0f);
	glRotatef(joint_rot_lal, 0.0, 0.0, 1.0);
	drawArmLower(1.0);
	//hand left
	glTranslatef(0.0f, -1.0f, 0.0f);
	glScalef(1.0, 1.0, 1.0);
	glRotatef(joint_rot_hl, 0.0, 0.0, 1.0);
	drawHand(1.0);
	glPopMatrix();
	glPopMatrix();

	// Right Arm
	// Upper Arm Right Movements
	glPushMatrix();
	glLoadIdentity();
	glScalef(1.0, -1.0, 1.0);
	glTranslatef(0.0f, 0.5f, -11.0f);
	glRotatef(joint_rot_uar, 0.0, 0.0, 1.0);
	drawArmUpper(1.0);
	//lower arm right 
	glTranslatef(0.0f, 1.2f, 0.0f);
	glScalef(1.0, 1.0, 1.0);
	glRotatef(joint_rot_lar, 0.0, 0.0, 1.0);
	drawArmLower(1.0);
	//hand right
	glTranslatef(0.0f, 1.0f, 0.0f);
	glScalef(1.0, 1.0, 1.0);
	glRotatef(joint_rot_hr, 0.0, 0.0, 1.0);
	drawHand(1.0);
	glPopMatrix();
	glPopMatrix();


	// Draw Torso
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.5f);
	glRotatef(90, 0, 1, 0);
	drawTorso(1.0);
	glPopMatrix();
	glPopMatrix();


	// Left Leg
	// Draw Upper Leg Left
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, -2.0f, 0.0f);
	glScalef(-1.0, 1.0, 1.0);
	glTranslatef(0.0f, 0.0f, -9.5f);
	glRotatef(joint_rot_ull, 0.0, 0.0, 1.0);
	drawLegUpper(1.0);
	//lower leg left 
	glScalef(-1.0, 1.0, 1.0);
	glTranslatef(0.0, -1.4f, 0.0);
	glRotatef(joint_rot_lll, 0.0, 0.0, 1.0);
	drawLegLower(1.0);
	// foot left
	glTranslatef(0.0, -0.8f, 0.0);
	glRotatef(joint_rot_fl, 0.0, 0.0, 1.0);
	glScalef(-1.0, 1.0, 1.0);
	drawFeet(1.0);
	glPopMatrix();
	glPopMatrix();

	// Right Leg
	// Draw Upper Leg Right
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, -2.0f, 0.0f);
	glScalef(1.0, 1.0, 1.0);
	glTranslatef(0.0f, 0.0f, -10.5f);
	glRotatef(joint_rot_ulr, 0.0, 0.0, 1.0);
	drawLegUpper(1.0);
	//lower leg right 
	glScalef(1.0, 1.0, 1.0);
	glTranslatef(0.0, -1.4f, 0.0);
	glRotatef(joint_rot_llr, 0.0, 0.0, 1.0);
	drawLegLower(1.0);
	// foot right
	glTranslatef(0.0, -0.8f, 0.0);
	glRotatef(joint_rot_fr, 0.0, 0.0, 1.0);
	glScalef(1.0, 1.0, 1.0);
	drawFeet(1.0);
	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}

void mouse(int btn, int state, int x, int y)
{
	mouseCoords.x = x - SCREEN_WIDTH / 2;
	mouseCoords.y = -y + SCREEN_HEIGHT / 2;
}

void move()
{
	dis.x - mouseCoords.x - pos.x;
	dis.y = mouseCoords.y - pos.y;
	//if magnitude is < speed, set to mouseCoords
	if (sqrt(dis.y*dis.y + dis.x*dis.x) < speed)
	{
		pos.x = mouseCoords.x;
		pos.y = mouseCoords.y;
	}
	else
	{
		radian = atan2(dis.y, dis.x);
		pos.x += cos(radian)*speed;
		pos.y += sin(radian)*speed;
		rot = radian * 180 / PI;
	}
}

void update(int value)
{
	move();
	glutPostRedisplay();
	glutTimerFunc(10, update, 0);
}

void myReshape(GLsizei width, GLsizei height) {
	if (height == 0) height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, aspect, 0.01f, 100.0f);
}

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Usage: demo [width] [height]\n");
		printf("Using 300x200 window by default...\n");
		Win[0] = 600;
		Win[1] = 400;
	}
	else {
		Win[0] = atoi(argv[1]);
		Win[1] = atoi(argv[2]);
	}

	glutInit(&argc, argv);
	initGlut(argv[0]);
	initGlui();
	initGl();
	initGL();
	glutMainLoop();
	return 0;
}

void initGlut(char* winName)
{
	// Set video mode: double-buffered, color, depth-buffered
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create window
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(Win[0], Win[1]);
	windowID = glutCreateWindow("3D Robot Walk Cycle");

	// Setup callback functions to handle events
	glutReshapeFunc(myReshape); // Call myReshape whenever window resized
	glutDisplayFunc(display);   // Call display whenever new frame needed 
}

// Animate button handler.  Called when the "animate" checkbox is pressed.
void animateButton(int)
{
	// synchronize variables that GLUT uses
	glui->sync_live();

	animation_frame = 0;
	if (animate_mode == 1) {
		// start animation
		GLUI_Master.set_glutIdleFunc(animate);
	}
	else {
		// stop animation
		GLUI_Master.set_glutIdleFunc(NULL);
	}
}

// Initialize GLUI and the user interface
void initGlui()
{
	GLUI_Master.set_glutIdleFunc(NULL);

	// Create GLUI window
	glui = GLUI_Master.create_glui("Walk Cycle Controls", 0, Win[0] + 30, 0);

	// Description
	glui->add_statictext("3D Robot Walk Cycle:");
	glui->add_separator();
	glui->add_statictext("P + ALT to go through keyframe");
	glui->add_statictext("Escape to quit program");
	glui->add_statictext("Animate checkbox to start walk cycle");

	// Left Side
	glui->add_separator();
	glui->add_statictext("Left Side");
	glui->add_separator();
	// Upper Arm
	GLUI_Spinner *joint_spinner
		= glui->add_spinner("Upper Arm", GLUI_SPINNER_FLOAT, &joint_rot_ual);
	joint_spinner->set_speed(0.1);
	joint_spinner->set_float_limits(JOINT_MIN_UAL, JOINT_MAX_UAL, GLUI_LIMIT_CLAMP);
	// Lower Arm
	GLUI_Spinner *joint_spinner_lal
		= glui->add_spinner("Lower Arm", GLUI_SPINNER_FLOAT, &joint_rot_lal);
	joint_spinner_lal->set_speed(0.1);
	joint_spinner_lal->set_float_limits(JOINT_MIN_LAL, JOINT_MAX_LAL, GLUI_LIMIT_CLAMP);
	// Hand
	GLUI_Spinner *joint_spinner_hl
		= glui->add_spinner("Hand", GLUI_SPINNER_FLOAT, &joint_rot_hl);
	joint_spinner_hl->set_speed(0.1);
	joint_spinner_hl->set_float_limits(JOINT_MIN_HL, JOINT_MAX_HL, GLUI_LIMIT_CLAMP);
	// Upper Leg
	GLUI_Spinner *joint_spinner_ull
		= glui->add_spinner("Upper Leg", GLUI_SPINNER_FLOAT, &joint_rot_ull);
	joint_spinner_ull->set_speed(0.1);
	joint_spinner_ull->set_float_limits(JOINT_MIN_ULL, JOINT_MAX_ULL, GLUI_LIMIT_CLAMP);
	// Lower Leg
	GLUI_Spinner *joint_spinner_lll
		= glui->add_spinner("Lower Leg", GLUI_SPINNER_FLOAT, &joint_rot_lll);
	joint_spinner_lll->set_speed(0.1);
	joint_spinner_lll->set_float_limits(JOINT_MIN_LLL, JOINT_MAX_LLL, GLUI_LIMIT_CLAMP);
	// Foot
	GLUI_Spinner *joint_spinner_fl
		= glui->add_spinner("Foot", GLUI_SPINNER_FLOAT, &joint_rot_fl);
	joint_spinner_fl->set_speed(0.1);
	joint_spinner_fl->set_float_limits(JOINT_MIN_FL, JOINT_MAX_FL, GLUI_LIMIT_CLAMP);


	// Right Side
	glui->add_separator();
	glui->add_statictext("Right Side");
	glui->add_separator();
	// Upper Arm
	GLUI_Spinner *joint_spinner_uar
		= glui->add_spinner("Upper Arm", GLUI_SPINNER_FLOAT, &joint_rot_uar);
	joint_spinner_uar->set_speed(0.1);
	joint_spinner_uar->set_float_limits(JOINT_MIN_UAR, JOINT_MAX_UAR, GLUI_LIMIT_CLAMP);
	// Lower Arm
	GLUI_Spinner *joint_spinner_lar
		= glui->add_spinner("Lower Arm", GLUI_SPINNER_FLOAT, &joint_rot_lar);
	joint_spinner_lar->set_speed(0.1);
	joint_spinner_lar->set_float_limits(JOINT_MIN_LAR, JOINT_MAX_LAR, GLUI_LIMIT_CLAMP);
	// Hand
	GLUI_Spinner *joint_spinner_hr
		= glui->add_spinner("Hand", GLUI_SPINNER_FLOAT, &joint_rot_hr);
	joint_spinner_hr->set_speed(0.1);
	joint_spinner_hr->set_float_limits(JOINT_MIN_HR, JOINT_MAX_HR, GLUI_LIMIT_CLAMP);
	// Upper Leg
	GLUI_Spinner *joint_spinner_ulr
		= glui->add_spinner("Upper Leg", GLUI_SPINNER_FLOAT, &joint_rot_ulr);
	joint_spinner_ulr->set_speed(0.1);
	joint_spinner_ulr->set_float_limits(JOINT_MIN_ULR, JOINT_MAX_ULR, GLUI_LIMIT_CLAMP);
	// Lower Leg
	GLUI_Spinner *joint_spinner_llr
		= glui->add_spinner("Lower Leg", GLUI_SPINNER_FLOAT, &joint_rot_llr);
	joint_spinner_llr->set_speed(0.1);
	joint_spinner_llr->set_float_limits(JOINT_MIN_LLR, JOINT_MAX_LLR, GLUI_LIMIT_CLAMP);
	// Foot
	GLUI_Spinner *joint_spinner_fr
		= glui->add_spinner("Foot", GLUI_SPINNER_FLOAT, &joint_rot_fr);
	joint_spinner_fr->set_speed(0.1);
	joint_spinner_fr->set_float_limits(JOINT_MIN_FR, JOINT_MAX_FR, GLUI_LIMIT_CLAMP);

	// Add button to specify animation mode 
	glui->add_separator();
	glui->add_checkbox("Animate Walk", &animate_mode, 0, animateButton);

	// Set the main window to be the "active" window
	glui->set_main_gfx_window(windowID);
}

// Performs most of the OpenGL intialization
void initGl(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutMouseFunc(mouse);
	glutTimerFunc(25, update, 0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	// glClearColor (red, green, blue, alpha)
	// Ignore the meaning of the 'alpha' value for now
	glClearColor(0.7f, 0.7f, 0.9f, 1.0f);
	// Keyboard functions to quit and go through frames
	glutKeyboardFunc(keys);

}

// Callback idle function for animating the scene
void animate()
{
	// Update geometry
	const double joint_rot_speed = 0.1;
	double joint_rot_t = (sin(animation_frame*joint_rot_speed) + 1.0) / 2.0;
	// Upper Arm Left
	joint_rot_ual = joint_rot_t * JOINT_MIN_UAL + (1 - joint_rot_t) * JOINT_MAX_UAL;
	// Upper Arm Right
	joint_rot_uar = joint_rot_t * JOINT_MIN_UAR + (1 - joint_rot_t) * JOINT_MAX_UAR;
	// Lower Arm Left
	joint_rot_lal = joint_rot_t * JOINT_MIN_LAL + (1 - joint_rot_t) * JOINT_MAX_LAL;
	// Lower Arm Right
	joint_rot_lar = joint_rot_t * JOINT_MIN_LAR + (1 - joint_rot_t) * JOINT_MAX_LAR;
	// Hand Left
	joint_rot_hl = joint_rot_t * JOINT_MIN_HL + (1 - joint_rot_t) * JOINT_MAX_HL;
	// Hand Right
	joint_rot_hr = joint_rot_t * JOINT_MIN_HR + (1 - joint_rot_t) * JOINT_MAX_HR;
	// Upper Leg Left
	joint_rot_ull = joint_rot_t * JOINT_MIN_ULL + (1 - joint_rot_t) * JOINT_MAX_ULL;
	// Upper Leg Right
	joint_rot_ulr = joint_rot_t * JOINT_MIN_ULR + (1 - joint_rot_t) * JOINT_MAX_ULR;
	// Lower Leg Left
	joint_rot_lll = joint_rot_t * JOINT_MIN_LLL + (1 - joint_rot_t) * JOINT_MAX_LLL;
	// Lower Leg Right
	joint_rot_llr = joint_rot_t * JOINT_MIN_LLR + (1 - joint_rot_t) * JOINT_MAX_LLR;
	// Foot Left
	joint_rot_fl = joint_rot_t * JOINT_MIN_FL + (1 - joint_rot_t) * JOINT_MAX_FL;
	// Foot Right
	joint_rot_fr - joint_rot_t * JOINT_MIN_FR + (1 - joint_rot_t) * JOINT_MAX_FR;

	// Update user interface
	glui->sync_live();

	// Tell glut window to update itself.  This will cause the display()
	// callback to be called, which renders the object (once you've written
	// the callback).
	glutSetWindow(windowID);
	glutPostRedisplay();

	// increment the frame number.
	animation_frame++;

	// Wait 50 ms between frames (20 frames per second)
	usleep(50000);
}


void drawHead(float width) {
	/****** Basic Shape***************/
	glBegin(GL_QUADS);
	// Top Orange
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(1.5f, 1.0f, -1.0f);
	glVertex3f(-1.5f, 1.0f, -1.0f);
	glVertex3f(-1.5f, 1.0f, 1.0f);
	glVertex3f(1.5f, 1.0f, 1.0f);
	// Bottom Orange
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(1.5f, -1.0f, 1.0f);
	glVertex3f(-1.5f, -1.0f, 1.0f);
	glVertex3f(-1.5f, -1.0f, -1.0f);
	glVertex3f(1.5f, -1.0f, -1.0f);
	// Front Red
	glColor3f(1.5f, 0.0f, 0.0f);
	glVertex3f(1.5f, 1.0f, 1.0f);
	glVertex3f(-1.5f, 1.0f, 1.0f);
	glVertex3f(-1.5f, -1.0f, 1.0f);
	glVertex3f(1.5f, -1.0f, 1.0f);
	// Back Red
	glColor3f(1.5f, 0.0f, 0.0f);
	glVertex3f(1.5f, -1.0f, -1.0f);
	glVertex3f(-1.5f, -1.0f, -1.0f);
	glVertex3f(-1.5f, 1.0f, -1.0f);
	glVertex3f(1.5f, 1.0f, -1.0f);
	// Left Red
	glColor3f(1.5f, 0.0f, 0.0f);
	glVertex3f(-1.5f, 1.0f, 1.0f);
	glVertex3f(-1.5f, 1.0f, -1.0f);
	glVertex3f(-1.5f, -1.0f, -1.0f);
	glVertex3f(-1.5f, -1.0f, 1.0f);
	// Right Red
	glColor3f(1.5f, 0.0f, 0.0f);
	glVertex3f(1.5f, 1.0f, -1.0f);
	glVertex3f(1.5f, 1.0f, 1.0f);
	glVertex3f(1.5f, -1.0f, 1.0f);
	glVertex3f(1.5f, -1.0f, -1.0f);
	glEnd();

	/******* EYe Right*****************/
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);
	// Eye Front
	glVertex3f(1.0f, 0.25f, 1.3f);
	glVertex3f(0.5f, 0.25f, 1.3f);
	glVertex3f(0.5f, -0.25f, 1.3f);
	glVertex3f(1.0f, -0.25f, 1.3f);
	// Eye Back
	glVertex3f(1.0f, -0.25f, 1.0f);
	glVertex3f(0.5f, -0.25f, 1.0f);
	glVertex3f(0.5f, 0.25f, 1.0f);
	glVertex3f(1.0f, 0.25f, 1.0f);
	// EYe Left
	glVertex3f(0.5f, 0.25f, 1.3f);
	glVertex3f(0.5f, 0.25f, 1.0f);
	glVertex3f(0.5f, -0.25f, 1.0f);
	glVertex3f(0.5f, -0.25f, 1.3f);
	// Eye Right 
	glVertex3f(1.0f, 0.25f, 1.0f);
	glVertex3f(1.0f, 0.25f, 1.3f);
	glVertex3f(1.0f, -0.25f, 1.3f);
	glVertex3f(1.0f, -0.25f, 1.0f);
	// Eye Top
	glVertex3f(1.0f, 0.25f, 1.0f);
	glVertex3f(0.5f, 0.25f, 1.0f);
	glVertex3f(0.5f, 0.25f, 1.3f);
	glVertex3f(1.0f, 0.25f, 1.3f);
	// Eye Bottom
	glVertex3f(1.0f, -0.25f, 1.3f);
	glVertex3f(0.5f, -0.25f, 1.3f);
	glVertex3f(0.5f, -0.25f, 1.0f);
	glVertex3f(1.0f, -0.25f, 1.0f);
	// Eye Detail 
	//front
	glColor3f(1.5f, 1.0f, 0.0f);
	glVertex3f(0.55f, 0.2f, 1.4f);
	glVertex3f(0.95f, 0.2f, 1.4f);
	glVertex3f(0.95f, -0.2f, 1.4f);
	glVertex3f(0.55f, -0.2f, 1.4f);
	//back
	glVertex3f(0.55f, 0.2f, 1.2f);
	glVertex3f(0.95f, 0.2f, 1.2f);
	glVertex3f(0.95f, -0.2f, 1.2f);
	glVertex3f(0.55f, -0.2f, 1.2f);
	//right
	glVertex3f(0.95f, 0.2f, 1.4f);
	glVertex3f(0.95f, 0.2f, 1.2f);
	glVertex3f(0.95f, -0.2f, 1.4f);
	glVertex3f(0.95f, -0.2f, 1.2f);
	//left
	glVertex3f(0.55f, 0.2f, 1.4f);
	glVertex3f(0.55f, 0.2f, 1.2f);
	glVertex3f(0.55f, -0.2f, 1.4f);
	glVertex3f(0.55f, -0.2f, 1.2f);
	//top
	glVertex3f(0.55f, 0.2f, 1.4f);
	glVertex3f(0.55f, 0.2f, 1.2f);
	glVertex3f(0.95f, 0.2f, 1.4f);
	glVertex3f(0.95f, 0.2f, 1.2f);
	//bottom
	glVertex3f(0.55f, -0.2f, 1.4f);
	glVertex3f(0.55f, -0.2f, 1.2f);
	glVertex3f(0.95f, -0.2f, 1.4f);
	glVertex3f(0.95f, -0.2f, 1.2f);
	glEnd();

	/******** Eye Left ******************/
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);
	// Eye Front
	glVertex3f(-1.0f, 0.25f, 1.3f);
	glVertex3f(-0.5f, 0.25f, 1.3f);
	glVertex3f(-0.5f, -0.25f, 1.3f);
	glVertex3f(-1.0f, -0.25f, 1.3f);
	// Eye Back
	glVertex3f(-1.0f, -0.25f, 1.0f);
	glVertex3f(-0.5f, -0.25f, 1.0f);
	glVertex3f(-0.5f, 0.25f, 1.0f);
	glVertex3f(-1.0f, 0.25f, 1.0f);
	// EYe Left
	glVertex3f(-0.5f, 0.25f, 1.3f);
	glVertex3f(-0.5f, 0.25f, 1.0f);
	glVertex3f(-0.5f, -0.25f, 1.0f);
	glVertex3f(-0.5f, -0.25f, 1.3f);
	// Eye Right 
	glVertex3f(-1.0f, 0.25f, 1.0f);
	glVertex3f(-1.0f, 0.25f, 1.3f);
	glVertex3f(-1.0f, -0.25f, 1.3f);
	glVertex3f(-1.0f, -0.25f, 1.0f);
	// Eye Top
	glVertex3f(-1.0f, 0.25f, 1.0f);
	glVertex3f(-0.5f, 0.25f, 1.0f);
	glVertex3f(-0.5f, 0.25f, 1.3f);
	glVertex3f(-1.0f, 0.25f, 1.3f);
	// Eye Bottom
	glVertex3f(-1.0f, -0.25f, 1.3f);
	glVertex3f(-0.5f, -0.25f, 1.3f);
	glVertex3f(-0.5f, -0.25f, 1.0f);
	glVertex3f(-1.0f, -0.25f, 1.0f);
	// Eye Detail
	//front
	glColor3f(1.5f, 1.0f, 0.0f);
	glVertex3f(-0.55f, 0.2f, 1.4f);
	glVertex3f(-0.95f, 0.2f, 1.4f);
	glVertex3f(-0.95f, -0.2f, 1.4f);
	glVertex3f(-0.55f, -0.2f, 1.4f);
	//back
	glVertex3f(-0.55f, 0.2f, 1.2f);
	glVertex3f(-0.95f, 0.2f, 1.2f);
	glVertex3f(-0.95f, -0.2f, 1.2f);
	glVertex3f(-0.55f, -0.2f, 1.2f);
	//right
	glVertex3f(-0.95f, 0.2f, 1.4f);
	glVertex3f(-0.95f, 0.2f, 1.2f);
	glVertex3f(-0.95f, -0.2f, 1.4f);
	glVertex3f(-0.95f, -0.2f, 1.2f);
	//left
	glVertex3f(-0.55f, 0.2f, 1.4f);
	glVertex3f(-0.55f, 0.2f, 1.2f);
	glVertex3f(-0.55f, -0.2f, 1.4f);
	glVertex3f(-0.55f, -0.2f, 1.2f);
	//top
	glVertex3f(-0.55f, 0.2f, 1.4f);
	glVertex3f(-0.55f, 0.2f, 1.2f);
	glVertex3f(-0.95f, 0.2f, 1.4f);
	glVertex3f(-0.95f, 0.2f, 1.2f);
	//bottom
	glVertex3f(-0.55f, -0.2f, 1.4f);
	glVertex3f(-0.55f, -0.2f, 1.2f);
	glVertex3f(-0.95f, -0.2f, 1.4f);
	glVertex3f(-0.95f, -0.2f, 1.2f);
	glEnd();

	/******** Ear Right ****************/
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);
	// Ear Front
	glVertex3f(2.0f, 0.3f, 0.2f);
	glVertex3f(1.45f, 0.3f, 0.2f);
	glVertex3f(1.45f, -0.3f, 0.2f);
	glVertex3f(2.0f, -0.3f, 0.2f);
	// Ear Back
	glVertex3f(2.0f, -0.3f, -0.2f);
	glVertex3f(1.45f, -0.3f, -0.2f);
	glVertex3f(1.45f, 0.3f, -0.2f);
	glVertex3f(2.0f, 0.3f, -0.2f);
	// Ear Left
	glVertex3f(1.45f, 0.3f, 0.2f);
	glVertex3f(1.45f, 0.3f, -0.2f);
	glVertex3f(1.45f, -0.3f, -0.2f);
	glVertex3f(1.45f, -0.3f, 0.2f);
	// Ear Right 
	glVertex3f(2.0f, 0.3f, -0.2f);
	glVertex3f(2.0f, 0.3f, 0.2f);
	glVertex3f(2.0f, -0.3f, 0.2f);
	glVertex3f(2.0f, -0.3f, -0.2f);
	// Ear Top
	glVertex3f(2.0f, 0.3f, -0.2f);
	glVertex3f(1.45f, 0.3f, -0.2f);
	glVertex3f(1.45f, 0.3f, 0.2f);
	glVertex3f(2.0f, 0.3f, 0.2f);
	// Ear Bottom
	glVertex3f(2.0f, -0.3f, 0.2f);
	glVertex3f(1.45f, -0.3f, 0.2f);
	glVertex3f(1.45f, -0.3f, -0.2f);
	glVertex3f(2.0f, -0.3f, -0.2f);
	glEnd();

	/*********** Ear Left ******************/
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);
	// Ear Front
	glVertex3f(-2.0f, 0.3f, 0.2f);
	glVertex3f(-1.45f, 0.3f, 0.2f);
	glVertex3f(-1.45f, -0.3f, 0.2f);
	glVertex3f(-2.0f, -0.3f, 0.2f);
	// Ear Back
	glVertex3f(-2.0f, -0.3f, -0.2f);
	glVertex3f(-1.45f, -0.3f, -0.2f);
	glVertex3f(-1.45f, 0.3f, -0.2f);
	glVertex3f(-2.0f, 0.3f, -0.2f);
	// Ear Left
	glVertex3f(-1.45f, 0.3f, 0.2f);
	glVertex3f(-1.45f, 0.3f, -0.2f);
	glVertex3f(-1.45f, -0.3f, -0.2f);
	glVertex3f(-1.45f, -0.3f, 0.2f);
	// Ear Right 
	glVertex3f(-2.0f, 0.3f, -0.2f);
	glVertex3f(-2.0f, 0.3f, 0.2);
	glVertex3f(-2.0f, -0.3f, 0.2f);
	glVertex3f(-2.0f, -0.3f, -0.2f);
	// Ear Top
	glVertex3f(-2.0f, 0.3f, -0.2f);
	glVertex3f(-1.45f, 0.3f, -0.2f);
	glVertex3f(-1.45f, 0.3f, 0.2f);
	glVertex3f(-2.0f, 0.3f, 0.2f);
	// Ear Bottom
	glVertex3f(-2.0f, -0.3f, 0.2f);
	glVertex3f(-1.45f, -0.3f, 0.2f);
	glVertex3f(-1.45f, -0.3f, -0.2f);
	glVertex3f(-2.0f, -0.3f, -0.2f);
	glEnd();

	/*****AntBottom*************************/
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);
	// Box Front
	glVertex3f(0.15f, 1.35f, 0.2f);
	glVertex3f(-0.15f, 1.35f, 0.2f);
	glVertex3f(-0.15f, 1.0f, 0.2f);
	glVertex3f(0.15f, 1.0f, 0.2f);
	// Box Back
	glVertex3f(0.15f, 1.0f, -0.2f);
	glVertex3f(-0.15f, 1.0f, -0.2f);
	glVertex3f(-0.15f, 1.35f, -0.2f);
	glVertex3f(0.15f, 1.35f, -0.2f);
	// Box Left
	glVertex3f(-0.15f, 1.35f, 0.2f);
	glVertex3f(-0.15f, 1.35f, -0.2f);
	glVertex3f(-0.15f, 1.0f, -0.2f);
	glVertex3f(-0.15f, 1.0f, 0.2f);
	// Box Right 
	glVertex3f(0.15f, 1.35f, -0.2f);
	glVertex3f(0.15f, 1.35f, 0.2f);
	glVertex3f(0.15f, 1.0f, 0.2f);
	glVertex3f(0.15f, 1.0f, -0.2f);
	// Box Top
	glVertex3f(0.15f, 1.35f, -0.2f);
	glVertex3f(-0.15f, 1.35f, -0.2f);
	glVertex3f(-0.15f, 1.35f, 0.2f);
	glVertex3f(0.15f, 1.35f, 0.2f);
	// Box Bottom
	glVertex3f(0.15f, 1.0f, 0.2f);
	glVertex3f(-0.15f, 1.0f, 0.2f);
	glVertex3f(-0.15f, 1.0f, -0.2f);
	glVertex3f(0.15f, 1.0f, -0.2f);
	glEnd();

	/***** AntTop *********************/
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);
	// Box Front
	glVertex3f(0.2f, 1.6f, 0.25f);
	glVertex3f(-0.2f, 1.6f, 0.25f);
	glVertex3f(-0.2f, 1.3f, 0.25f);
	glVertex3f(0.2f, 1.3f, 0.25f);
	// Box Back
	glVertex3f(0.2f, 1.3f, -0.25f);
	glVertex3f(-0.2f, 1.3f, -0.25f);
	glVertex3f(-0.2f, 1.6f, -0.25f);
	glVertex3f(0.2f, 1.6f, -0.25f);
	// Box Left
	glVertex3f(-0.2f, 1.6f, 0.25f);
	glVertex3f(-0.2f, 1.6f, -0.25f);
	glVertex3f(-0.2f, 1.3f, -0.25f);
	glVertex3f(-0.2f, 1.3f, 0.25f);
	// Box Right 
	glVertex3f(0.2f, 1.6f, -0.25f);
	glVertex3f(0.2f, 1.6f, 0.25f);
	glVertex3f(0.2f, 1.3f, 0.25f);
	glVertex3f(0.2f, 1.3f, -0.25f);
	// Box Top
	glVertex3f(0.2f, 1.6f, -0.25f);
	glVertex3f(-0.2f, 1.6f, -0.25f);
	glVertex3f(-0.2f, 1.6f, 0.25f);
	glVertex3f(0.2f, 1.6f, 0.25f);
	// Box Bottom
	glVertex3f(0.2f, 1.3f, 0.25f);
	glVertex3f(-0.2f, 1.3f, 0.25f);
	glVertex3f(-0.2f, 1.3f, -0.25f);
	glVertex3f(0.2f, 1.3f, -0.25f);
	glEnd();

	/****** Mouth ********************/
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);
	// Box Front
	glVertex3f(0.5f, -0.5f, 1.3f);
	glVertex3f(-0.5f, -0.5f, 1.3f);
	glVertex3f(-0.5f, -0.75f, 1.3f);
	glVertex3f(0.5f, -0.75f, 1.3f);
	// Box Back
	glVertex3f(0.5f, -0.5f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 1.0f);
	glVertex3f(-0.5f, -0.75f, 1.0f);
	glVertex3f(0.5f, -0.75f, 1.0f);
	// Box Left
	glVertex3f(-0.5f, -0.5f, 1.3f);
	glVertex3f(-0.5f, -0.5f, 1.0f);
	glVertex3f(-0.5f, -0.75f, 1.0f);
	glVertex3f(-0.5f, -0.75f, 1.3f);
	// Box Right 
	glVertex3f(0.5f, -0.5f, 1.0f);
	glVertex3f(0.5f, -0.5f, 1.3f);
	glVertex3f(0.5f, -0.75f, 1.3f);
	glVertex3f(0.5f, -0.75f, 1.0f);
	// Box Top
	glVertex3f(0.5f, -0.5f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 1.3f);
	glVertex3f(0.5f, -0.5f, 1.3f);
	// Box Bottom
	glVertex3f(0.5f, -0.75f, 1.3f);
	glVertex3f(-0.5f, -0.75f, 1.3f);
	glVertex3f(-0.5f, -0.75f, 1.0f);
	glVertex3f(0.5f, -0.75f, 1.0f);
	glEnd();

	/********** Neck *********************/
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);
	// Box Front
	glVertex3f(0.15f, -1.0f, 0.2f);
	glVertex3f(-0.15f, -1.0f, 0.2f);
	glVertex3f(-0.15f, -1.75f, 0.2f);
	glVertex3f(0.15f, -1.75f, 0.2f);
	// Box Back
	glVertex3f(0.15f, -1.75f, -0.2f);
	glVertex3f(-0.15f, -1.75f, -0.2f);
	glVertex3f(-0.15f, -1.0f, -0.2f);
	glVertex3f(0.15f, -1.0f, -0.2f);
	// Box Left
	glVertex3f(-0.15f, -1.0f, 0.2f);
	glVertex3f(-0.15f, -1.0f, -0.2f);
	glVertex3f(-0.15f, -1.75f, -0.2f);
	glVertex3f(-0.15f, -1.75f, 0.2f);
	// Box Right 
	glVertex3f(0.15f, -1.0f, -0.2f);
	glVertex3f(0.15f, -1.0f, 0.2f);
	glVertex3f(0.15f, -1.75f, 0.2f);
	glVertex3f(0.15f, -1.75f, -0.2f);
	// Box Top
	glVertex3f(0.15f, -1.0f, -0.2f);
	glVertex3f(-0.15f, -1.0f, -0.2f);
	glVertex3f(-0.15f, -1.0f, 0.2f);
	glVertex3f(0.15f, -1.0f, 0.2f);
	// Box Bottom
	glVertex3f(0.15f, -1.75f, 0.2f);
	glVertex3f(-0.15f, -1.75f, 0.2f);
	glVertex3f(-0.15f, -1.75f, -0.2f);
	glVertex3f(0.15f, -1.75f, -0.2f);
	glEnd();
}

void drawArmUpper(float width) {
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.5f);

	// Box Front
	glVertex3f(0.3f, 0.9f, 0.25f);
	glVertex3f(-0.3f, 0.9f, 0.25f);
	glVertex3f(-0.3f, -0.9f, 0.25f);
	glVertex3f(0.3f, -0.9f, 0.25f);

	// Box Back
	glVertex3f(0.3f, -0.9f, -0.25f);
	glVertex3f(-0.3f, -0.9f, -0.25f);
	glVertex3f(-0.3f, 0.9f, -0.25f);
	glVertex3f(0.3f, 0.9f, -0.25f);

	// Box Left
	glVertex3f(-0.3f, 0.9f, 0.25f);
	glVertex3f(-0.3f, 0.9f, -0.25f);
	glVertex3f(-0.3f, -0.9f, -0.25f);
	glVertex3f(-0.3f, -0.9f, 0.25f);

	// Box Right 
	glVertex3f(0.3f, 0.9f, -0.25f);
	glVertex3f(0.3f, 0.9f, 0.25f);
	glVertex3f(0.3f, -0.9f, 0.25f);
	glVertex3f(0.3f, -0.9f, -0.25f);

	// Box Top
	glVertex3f(0.3f, 0.9f, -0.25f);
	glVertex3f(-0.3f, 0.9f, -0.25f);
	glVertex3f(-0.3f, 0.9f, 0.25f);
	glVertex3f(0.3f, 0.9f, 0.25f);

	// Box Bottom
	glVertex3f(0.3f, -0.9f, 0.25f);
	glVertex3f(-0.3f, -0.9f, 0.25f);
	glVertex3f(-0.3f, -0.9f, -0.25f);
	glVertex3f(0.3f, -0.9f, -0.25f);

	glEnd();
}

void drawArmLower(float width) {
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);

	// Box Front
	glVertex3f(0.25f, 0.8f, 0.25f);
	glVertex3f(-0.25f, 0.8f, 0.25f);
	glVertex3f(-0.25f, -0.8f, 0.25f);
	glVertex3f(0.25f, -0.8f, 0.25f);

	// Box Back
	glVertex3f(0.25f, -0.8f, -0.25f);
	glVertex3f(-0.25f, -0.8f, -0.25f);
	glVertex3f(-0.25f, 0.8f, -0.25f);
	glVertex3f(0.25f, 0.8f, -0.25f);

	// Box Left
	glVertex3f(-0.25f, 0.8f, 0.25f);
	glVertex3f(-0.25f, 0.8f, -0.25f);
	glVertex3f(-0.25f, -0.8f, -0.25f);
	glVertex3f(-0.25f, -0.8f, 0.25f);

	// Box Right 
	glVertex3f(0.25f, 0.8f, -0.25f);
	glVertex3f(0.25f, 0.8f, 0.25f);
	glVertex3f(0.25f, -0.8f, 0.25f);
	glVertex3f(0.25f, -0.8f, -0.25f);

	// Box Top
	glVertex3f(0.25f, 0.8f, -0.25f);
	glVertex3f(-0.25f, 0.8f, -0.25f);
	glVertex3f(-0.25f, 0.8f, 0.25f);
	glVertex3f(0.25f, 0.8f, 0.25f);

	// Box Bottom
	glVertex3f(0.25f, -0.8f, 0.25f);
	glVertex3f(-0.25f, -0.8f, 0.25f);
	glVertex3f(-0.25f, -0.8f, -0.25f);
	glVertex3f(0.25f, -0.8f, -0.25f);

	glEnd();
}

void drawHand(float width) {
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.5f);

	//  Front
	glVertex3f(0.3f, 0.5f, 0.5f);
	glVertex3f(-0.3f, 0.5f, 0.5f);
	glVertex3f(-0.3f, -0.5f, 0.5f);
	glVertex3f(0.3f, -0.5f, 0.5f);

	//  Back
	glVertex3f(0.3f, -0.5f, -0.5f);
	glVertex3f(-0.3f, -0.5f, -0.5f);
	glVertex3f(-0.3f, 0.5f, -0.5f);
	glVertex3f(0.3f, 0.5f, -0.5f);

	//  Left
	glVertex3f(-0.3f, 0.5f, 0.5f);
	glVertex3f(-0.3f, 0.5f, -0.5f);
	glVertex3f(-0.3f, -0.5f, -0.5f);
	glVertex3f(-0.3f, -0.5f, 0.5f);

	//  Right 
	glVertex3f(0.3f, 0.5f, -0.5f);
	glVertex3f(0.3f, 0.5f, 0.5f);
	glVertex3f(0.3f, -0.5f, 0.5f);
	glVertex3f(0.3f, -0.5f, -0.5f);

	//  Top
	glVertex3f(0.3f, 0.5f, -0.5f);
	glVertex3f(-0.3f, 0.5f, -0.5f);
	glVertex3f(-0.3f, 0.5f, 0.5f);
	glVertex3f(0.3f, 0.5f, 0.5f);

	//  Bottom
	glVertex3f(0.3f, -0.5f, 0.5f);
	glVertex3f(-0.3f, -0.5f, 0.5f);
	glVertex3f(-0.3f, -0.5f, -0.5f);
	glVertex3f(0.3f, -0.5f, -0.5f);

	glEnd();
}

void drawTorso(float width)
{
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glColor3f(1.0, 0.0, 0.0);
	glScalef(3.0, 3.0, 2.0);
	glutSolidCube(1);
	glPopMatrix();

	glEnd();
}

void drawLegUpper(float width)
{
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glColor3f(1.0, 0.0, 0.0);
	glScalef(0.65, 2.0, 0.70);
	glutSolidCube(1);
	glPopMatrix();
}

void drawLegLower(float width)
{
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glColor3f(1.0, 0.0, 1.0);
	glScalef(0.50, 1.0, 0.20);
	glutSolidCube(1);
	glPopMatrix();
}

void drawFeet(float width)
{
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glColor3f(1.0, 0.0, 0.0);
	glScalef(0.70, 0.70, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}


