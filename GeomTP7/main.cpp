
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include "Octree.h"

/* au cas ou M_PI ne soit defini */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

#define WIDTH  768
#define HEIGHT 768

#define KEY_ESC 27

GLfloat qaWhite[] = { 1.0, 1.0, 1.0, 1.0 }; // White Color

float angleX = 0.0f; //angle de rotation en Y de la scene
float angleY = 0.0f; //angle de rotation en X de la scene

float pasDeplacement = 0.5f;

//position lumiere
float xLitePos = 0;
float yLitePos = 10;
float zLitePos = -100;

float tx = 0.0;
float ty = 0.0;
float tz = 0.0;

GLfloat VOXEL_SIZE = 5.0f;

Voxel*		DATA;
uint32_t	DATA_SIZE;

struct
{
	Sphere sphere;
	float voxel_size;
} gs;

point3* BOX = new point3[2]
{ 
	point3(-50, -50, -50),
	point3(50, 50, 50)
};

GLfloat* getVoxelData(point3 origin) {
	GLfloat *data = new GLfloat[108];
	for (int i = 0; i < 107; i+=3) {
		data[i]		= (origin.x + g_vertex_buffer_data[i] * VOXEL_SIZE);
		data[i+1]	= (origin.y + g_vertex_buffer_data[i+1] * VOXEL_SIZE);
		data[i+2]	= (origin.z + g_vertex_buffer_data[i+2] * VOXEL_SIZE);
	}
	return data;
}

void drawVoxel(Voxel v) {
	for (int i = 0; i < 107; i += 9) {
		if (v.potentiel > 200) {
			glBegin(GL_TRIANGLES);
			glVertex3f(v.vertices[i + 0], v.vertices[i + 1], v.vertices[i + 2]);
			glVertex3f(v.vertices[i + 3], v.vertices[i + 4], v.vertices[i + 5]);
			glVertex3f(v.vertices[i + 6], v.vertices[i + 7], v.vertices[i + 8]);
			glEnd();
		}
	}
}

void drawDATA(uint32_t size) {
	for (int i = 0; i < size; ++i) {
		drawVoxel(DATA[i]);
	}
}

Voxel* getBoxData(point3 box[2], uint32_t* size) {
	point3 a = box[0];
	point3 b = box[1];

	*size = floor(abs(b.z - a.z) * abs(b.y - a.y) * abs(b.x - a.x) / VOXEL_SIZE);

	Voxel* data = new Voxel[*size];

	int idx = 0;
	for (int z = a.z; z < b.z; z += VOXEL_SIZE) {
		for (int y = a.y; y < b.y; y += VOXEL_SIZE) {
			for (int x = a.x; x < b.x; x += VOXEL_SIZE) {
				data[idx] = Voxel
				{ 
					point3(x, y, z), 
					VOXEL_SIZE,
					255
				};
				++idx;
			}
		}
	}

	return data;
}

Voxel* getSphereData(point3 box[2], Sphere s, uint32_t* size) {
	point3 a = box[0];
	point3 b = box[1];

	*size = floor(abs(b.z - a.z) * abs(b.y - a.y) * abs(b.x - a.x) / VOXEL_SIZE);

	Voxel* data = new Voxel[*size];

	int idx = 0;
	for (int z = a.z; z < b.z; z += VOXEL_SIZE) {
		for (int y = a.y; y < b.y; y += VOXEL_SIZE) {
			for (int x = a.x; x < b.x; x += VOXEL_SIZE) {
				float dist = sqrtf((s.origin.x - x)*(s.origin.x - x) + (s.origin.y - y)*(s.origin.y - y) + (s.origin.z - z)*(s.origin.z - z));
				
				Voxel v = Voxel
				{
					point3(x, y, z),
					VOXEL_SIZE,
					0
				};
				
				if (dist <= s.radius) {
					v.potentiel = 255;
				}
				else {
					v.potentiel = 0;
				}

				data[idx] = v;
				++idx;
			}
		}
	}

	return data;
}

Voxel* getIntersectionSphereData(point3 box[2], Sphere s1, Sphere s2, uint32_t* size) {
	point3 a = box[0];
	point3 b = box[1];

	*size = floor(abs(b.z - a.z) * abs(b.y - a.y) * abs(b.x - a.x) / VOXEL_SIZE);

	Voxel* data = new Voxel[*size];

	int idx = 0;
	for (int z = a.z; z < b.z; z += VOXEL_SIZE) {
		for (int y = a.y; y < b.y; y += VOXEL_SIZE) {
			for (int x = a.x; x < b.x; x += VOXEL_SIZE) {
				float dist1 = sqrtf((s1.origin.x - x)*(s1.origin.x - x) + (s1.origin.y - y)*(s1.origin.y - y) + (s1.origin.z - z)*(s1.origin.z - z));
				float dist2 = sqrtf((s2.origin.x - x)*(s2.origin.x - x) + (s2.origin.y - y)*(s2.origin.y - y) + (s2.origin.z - z)*(s2.origin.z - z));

				Voxel v = Voxel
				{
					point3(x, y, z),
					VOXEL_SIZE,
					0
				};

				if (dist2 <= s2.radius && dist1 <= s1.radius) {
					v.potentiel = 255;
				}
				else {
					v.potentiel = 0;
				}

				data[idx] = v;
				++idx;
			}
		}
	}

	return data;
}

Voxel* getUnionSphereData(point3 box[2], Sphere s1, Sphere s2, uint32_t* size) {
	point3 a = box[0];
	point3 b = box[1];

	*size = floor(abs(b.z - a.z) * abs(b.y - a.y) * abs(b.x - a.x) / VOXEL_SIZE);

	Voxel* data = new Voxel[*size];

	int idx = 0;
	for (int z = a.z; z < b.z; z += VOXEL_SIZE) {
		for (int y = a.y; y < b.y; y += VOXEL_SIZE) {
			for (int x = a.x; x < b.x; x += VOXEL_SIZE) {
				float dist1 = sqrtf((s1.origin.x - x)*(s1.origin.x - x) + (s1.origin.y - y)*(s1.origin.y - y) + (s1.origin.z - z)*(s1.origin.z - z));
				float dist2 = sqrtf((s2.origin.x - x)*(s2.origin.x - x) + (s2.origin.y - y)*(s2.origin.y - y) + (s2.origin.z - z)*(s2.origin.z - z));

				Voxel v = Voxel
				{
					point3(x, y, z),
					VOXEL_SIZE,
					0
				};

				if (dist2 <= s2.radius || dist1 <= s1.radius) {
					v.potentiel = 255;
				}
				else {
					v.potentiel = 0;
				}

				data[idx] = v;
				++idx;
			}
		}
	}

	return data;
}

Voxel* getDiffSphereData(point3 box[2], Sphere s1, Sphere s2, uint32_t* size) {
	point3 a = box[0];
	point3 b = box[1];

	*size = floor(abs(b.z - a.z) * abs(b.y - a.y) * abs(b.x - a.x) / VOXEL_SIZE);

	Voxel* data = new Voxel[*size];

	int idx = 0;
	for (int z = a.z; z < b.z; z += VOXEL_SIZE) {
		for (int y = a.y; y < b.y; y += VOXEL_SIZE) {
			for (int x = a.x; x < b.x; x += VOXEL_SIZE) {
				float dist1 = sqrtf((s1.origin.x - x)*(s1.origin.x - x) + (s1.origin.y - y)*(s1.origin.y - y) + (s1.origin.z - z)*(s1.origin.z - z));
				float dist2 = sqrtf((s2.origin.x - x)*(s2.origin.x - x) + (s2.origin.y - y)*(s2.origin.y - y) + (s2.origin.z - z)*(s2.origin.z - z));

				Voxel v = Voxel
				{
					point3(x, y, z),
					VOXEL_SIZE,
					0
				};

				if ((dist2 <= s2.radius && dist1 > s1.radius) || (dist2 > s2.radius && dist1 <= s1.radius)) {
					v.potentiel = 255;
				}
				else {
					v.potentiel = 0;
				}

				data[idx] = v;
				++idx;
			}
		}
	}

	return data;
}

/* initialisation d'OpenGL*/
static void init ( ) {
	//glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
	glShadeModel(GL_SMOOTH);   // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
	
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Z Buffer pour la suppression des parties cachées
	glEnable(GL_DEPTH_TEST);

	GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat position[] = { 100, 100, 0, 1.0 };
	GLfloat position2[] = { -100, 100, 0, 1.0 };

	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_POSITION, position2);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0);
}

void updateData() {
	point3 origin1 = point3(-5.f, 0.f, 0.f);
	point3 origin2 = point3(5.f, 0.f, 0.f);

	Sphere s1 = { origin1, 35.f };
	Sphere s2 = { origin2, 15.f };

	//DATA = getBoxData(BOX, &DATA_SIZE);
	//DATA = getSphereData(BOX, s1, &DATA_SIZE);
	//DATA = getDiffSphereData(BOX, s1, s2, &DATA_SIZE);
	//DATA = getIntersectionSphereData(BOX, s1, s2, &DATA_SIZE);
	//DATA = getUnionSphereData(BOX, s1, s2, &DATA_SIZE);

	Octree octree = Octree(point3(.0f, .0f, .0f), 50.0f);
	std::vector<Voxel> voxels = std::vector<Voxel>();
	octree.createNodes(gs.sphere, voxels);



	DATA_SIZE = voxels.size();
	DATA = new Voxel[DATA_SIZE];
	for (int i = 0; i < DATA_SIZE; ++i) {
		DATA[i] = voxels[i];
	}
}

/* Dessin */
void display ( void ) {
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); //effacement du buffer

	//Description de la scene
	glLoadIdentity ( );

	glEnable ( GL_DEPTH_TEST );

	float rotate = glutGet(GLUT_ELAPSED_TIME) / 50 % 360;

	glPushMatrix();
	
	//glRotatef(rotate, 0, rotate, 1.f);
	
	drawDATA(DATA_SIZE);

	glPopMatrix();

	//affiche les axes du repere
	glColor3f ( 0.0, 1.0, 0.0 ); //Y vert
	glBegin ( GL_LINES );
	glVertex3f ( 0, 0, 0 );
	glVertex3f ( 0, 20, 0 );
	glEnd ( );

	glColor3f ( 0.0, 0.0, 1.0 ); //Z bleu
	glBegin ( GL_LINES );
	glVertex3f ( 0, 0, 0 );
	glVertex3f ( 0, 0, 20 );
	glEnd ( );

	glColor3f ( 1.0, 0.0, 0.0 ); //X rouge
	glBegin ( GL_LINES );
	glVertex3f ( 0, 0, 0 );
	glVertex3f ( 20, 0, 0 );
	glEnd ( );

	glutSwapBuffers ( );// echange des buffers

	glutPostRedisplay();
}

/* Au cas ou la fenetre est modifiee ou deplacee */
void reshape ( int w, int h ) {
	glViewport ( 0, 0, ( GLsizei ) w, ( GLsizei ) h );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ( );
	glOrtho ( -100, 100, -100, 100, -300, 300 );
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ( );
}

GLvoid gestionSouris ( int x, int y ) {
	angleX = x * 720 / WIDTH; //gere l'axe Oy
	angleY = -( y * 180 / HEIGHT - 90 ) * 4; //gere l'axe Ox

	glutPostRedisplay ( );
}

GLvoid gestionFleche ( int key, int x, int y ) {
	switch ( key ) {
		case GLUT_KEY_UP:
			tz += int ( pasDeplacement ); glutPostRedisplay ( );
			break;

		case GLUT_KEY_DOWN:
			tz -= int ( pasDeplacement ); glutPostRedisplay ( );
			break;
			glutPostRedisplay ( );
	}

}

GLvoid window_key_down ( unsigned char key, int x, int y )  //appuie des touches
{
	switch ( key ) {

		//deplacement de la camera
		case 'z':
			gs.sphere.origin.y += pasDeplacement;
			updateData();
			break;

		case 's':
			gs.sphere.origin.y -= pasDeplacement;
			updateData();
			break;

		case 'q':
			gs.sphere.origin.x -= pasDeplacement;
			updateData();
			break;

		case 'd':
			gs.sphere.origin.x += pasDeplacement;
			updateData();
			break;

		case 'p':
			VOXEL_SIZE += 1;
			updateData();
			break;

		case 'm':
			VOXEL_SIZE -= 1;
			VOXEL_SIZE = VOXEL_SIZE < 1.0F ? 1.0F : VOXEL_SIZE;
			updateData();
			break;

		case 'o':
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;

		case 'l':
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;

		//sortie
		case KEY_ESC:
			exit ( 0 );
			break;

		default:
			printf ( "La touche %d non active.\n", key );
			break;
	}
	glutPostRedisplay ( );
}

int main ( int argc, char **argv ) {
	gs.sphere = { point3(-5.f, 0.f, 0.f), 35.f };
	gs.voxel_size = VOXEL_SIZE;
	updateData();

	glutInitWindowSize ( 400, 400 );
	glutInit ( &argc, argv );
	glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_RGB );
	glutCreateWindow ( "Voxel" );
	init ( );
	glutReshapeFunc ( reshape );
	glutKeyboardFunc ( &window_key_down );
	glutDisplayFunc ( display );
	//glutPassiveMotionFunc ( gestionSouris );
	glutSpecialFunc ( &gestionFleche );
	glutMainLoop ( );
	return 0;
}
