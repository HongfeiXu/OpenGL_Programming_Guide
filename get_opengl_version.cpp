#include <stdio.h>
#include <GL/glut.h>

int main (int argc, char** argv)
{

	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow ("OpenGL Version");
	const GLubyte * name = glGetString (GL_VENDOR);
	const GLubyte * biaoshifu = glGetString (GL_RENDERER);
	const GLubyte * OpenGLVersion = glGetString (GL_VERSION);
	const GLubyte * gluVersion = gluGetString (GLU_VERSION);
	printf ("OpenGL实现厂商的名字：%s\n", name);
	printf ("渲染器标识符：%s\n", biaoshifu);
	printf ("OpenGL实现的版本号：%s\n", OpenGLVersion);
	printf ("GLU工具库版本：%s\n", gluVersion);
	return 0;
}
/*
OpenGL实现厂商的名字：NVIDIA Corporation
渲染器标识符：GeForce 410M/PCIe/SSE2
OpenGL实现的版本号：4.5.0 NVIDIA 353.62
GLU工具库版本：1.2.2.0 Microsoft Corporation
请按任意键继续. . .

*/