#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <gl/glut.h>
#include "drawutil.h"
#include "util.h"

void setColor(Score::UserType type)
{
	if (type == Score::USER) glColor3f(1.0f, 1.0f, 0.3f);
	else glColor3f(1, 0, 0);
}

void drawSphere(const Position &p, float radius)
{
	glPushMatrix();
	glTranslatef(p.x, p.y, p.z);
	glutSolidSphere(radius, NUM_SLICE, NUM_SLICE);
	glPopMatrix();
}

void drawCylinder(const Position &p1, const Position &p2, float r1, float r2)
{
	glm::vec3 v(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
	glm::vec3 d1, d2;

	if (v.y == 0 && v.z == 0) d1 = glm::normalize(glm::cross(v, glm::vec3(0, 1, 0)));
	else d1 = glm::normalize(glm::cross(v, glm::vec3(1, 0, 0)));
	d2 = glm::normalize(glm::cross(v, d1));

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= NUM_CIRCLE_POINT; ++i)
	{
		float theta = 2 * PI * i / NUM_CIRCLE_POINT;
		float dx = d1.x * cos(theta) + d2.x * sin(theta);
		float dy = d1.y * cos(theta) + d2.y * sin(theta);
		float dz = d1.z * cos(theta) + d2.z * sin(theta);
		glVertex3f(p1.x + r1 * dx, p1.y + r1 * dy, p1.z + r1 * dz);
		glVertex3f(p2.x + r2 * dx, p2.y + r2 * dy, p2.z + r2 * dz);
	}
	glEnd();
}

void drawBody(float top, float bottom, float x1, float z1, float x2, float z2)
{
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= NUM_CIRCLE_POINT; ++i)
	{
		float theta = 2 * PI * i / NUM_CIRCLE_POINT;
		glVertex3f(x1 * cos(theta), top, z1 * sin(theta));
		glVertex3f(x2 * cos(theta), bottom, z2 * sin(theta));
	}
	glEnd();
}
