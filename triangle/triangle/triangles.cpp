/////////////////////////////////
//
// triangles.cpp
//
////////////////////////////////
#include <iostream>
using namespace std;

#include "vgl.h"
#include "LoadShaders.h"

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDS { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];		// ���洢�������������
GLuint Buffers[NumBuffers];		// ���洢���㻺�������

const GLuint NumVertices = 6;

//-------------------------------------------------------------------------------
//
// init
//

void init (void)
{
	glGenVertexArrays (NumVAOs, VAOs);
	/*
	void glGenVertexArrays (GLsizei n, GLuint *arrays);
		���䶥��������������
	����n��δʹ�õĶ�����������arrays�У����������������VAO = Vertex Array Object�������ص����ֿ��������������Ļ������
	���������Ѿ�ʹ��δ��ʼ���Ķ������鼯�ϵ�Ĭ��״̬��������ֵ�ĳ�ʼ����
	��OpenGL�У�VAO�������Ͷ��㣨Vertices������������ĸ������ݡ�
	������Щ���������Ǳ��浽���㻺������У�Vertex Buffer Object��,���VBO��
	*/

	glBindVertexArray (VAOs[Triangles]);
	/*
	void glBindVertexArray (GLuint array);
		��һ������������󣬼�����һ���µĶ�������������������ƹ�������
	�������������VAOs[Triangles]�б�����glGenVertexArraysִ����ɺ����Ķ�������������Ϊ��������ú�����
	��һ�ΰ󶨶���ʱ��OpenGL�ڲ�������������������ڴ沢�ҽ�����Ϊ��ǰ����
	*/
	
	GLfloat vertices[NumVertices][2] = {
		{ -0.90, -0.90 },		// Triangle 1
		{ 0.85, -0.90 },
		{ -0.90, 0.85 },
		{ 0.90, -0.85 },		// Triangle 2,
		{ 0.90,  0.90 },
		{ -0.85, 0.90 }
	};

	glGenBuffers (NumBuffers, Buffers);
	/*
	void glGenBuffers (GLsizei n, GLuint *buffers);
	����n����ǰδʹ�õĻ���������ƣ������浽buffers�����С�
	*/

	glBindBuffer (GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	/*
	void glBindBuffer (GLenum target, GLuint buffer);
		��ʼ�����㻺�����
	����������
		1. ����ǵ�һ�ΰ�buffer��������һ��������޷������ͣ���ô������һ������������Ӧ���»������
		2. ����󶨵�һ���Ѿ������Ļ�������У���ô������Ϊ��ǰ������Ļ������
		3. ����󶨵�bufferֵΪ0����ôOpenGL�����ٶԵ�ǰtargetӦ���κλ������
	*/

	glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
	/*
	void glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
		���������뻺�����
	��Ҫ����������
		1. ���䶥����������Ĵ洢�ռ�
		2. �����ݴ�Ӧ�ó���������п�����OpenGL����˵��ڴ��С�
	*/

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders (shaders);
	glUseProgram (program);

	glVertexAttribPointer (vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET (0));
	/**
	����ԭ�ͣ�void glVertexAttribPointer(GLuint index, GLint size, GLenum type,
	GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	֮ǰ���ǵ�����glBindData�����ݸ������ֻ�����ݣ�֮������Ҫʹ������������ָ���������͡�
	���Ըú�����ɵ���Ҫ�����ǣ�
	1������OpenGL���ô洢���ݵĸ�ʽ
	2����Ϊ����ʹ����ɫ������̣�����ڶ�����ɫ���׶Σ����ǻ�ʹ�øú���������ɫ���е�in���͵����Ա����������ݡ�
	��ô��������ô��ϵ��������?
	����ͨ����һ������index��ָ������ɫ�������б������±�����á�
	���磺layout( location=index ) in vec4 position;
	������index��glVertexAttribPointer�ĵ�һ������һ������ô��ػ����������ݾͻᴫ�ݵ����position������ȥ��
	*/

	glEnableVertexAttribArray (vPosition);
	/**
	��ΪĬ������£��������������ǲ��ɷ��ʵģ�����������Ҫ�������º�����������
	��ΧΪ0��GL_MAX_VERTEX_ATTRIBS-1
	*/
}

//-------------------------------------------------------------------------------
//
// display
//

void display (void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	/**
	���ָ���Ļ������ݲ�����������Ϊ��ǰ�����ֵ
	��������ͨ���߼��������ָ�������ֵ������
	GL_COLOR_BUFFER_BIT ��ɫ����
	GL_DEPTH_BUFFER_BIT ��Ȼ���
	GL_STENCIL_BUFFER_BIT ģ�建��

	��OpenGLĬ�ϵ������ɫ�Ǻ�ɫ
	���Ҫָ��������ɫ�����Ե���glClearColor

	�������ǻ���Ҫ֪������ΪOpenGL��һ��״̬������˶����������趨OpenGL���ᱣ����
	���Զ���glClearColor����õĵ��÷����Ƿ��ڳ�ʼ�������У���Ϊ������ֻ�ᱻ����һ��
	�������display�У�OpenGL����ÿһ֡����������ظ����������ɫֵ���ή������Ч�ʡ�
	*/


	glBindVertexArray (VAOs[Triangles]);
	/**
	�ú��������ڳ�ʼ������������������εĹ��ܲ�̫һ��
	֮ǰ�ǳ�ʼ���������Ǽ���ö����������
	����ʹ������Ϊ����������ʹ�õĶ������顣
	*/

	glDrawArrays (GL_TRIANGLES, 0, NumVertices);
	/**
	����ԭ��void glDrawArrays(GLenum mode, GLint first, GLsizei count);
	��������ȾģʽΪGL_TRIANGLES����ʼλ��λ�ڻ����0ƫ��λ�ã�һ����ȾNumVertices��Ԫ�ء�
	�Ժ����ǻ����ѧϰ����ͼԪ
	*/

	glFlush ();
	/**
	�ú���ǿ�ư����н����е�OpenGL����������ɲ����䵽OpenGL����˴���
	*/
}

//-------------------------------------------------------------------------------
//
// main
//

int main (int argc, char ** argv)
{
	glutInit (&argc, argv);
	/*
	�ú���������ʼ��GLUT��,֮����뵱ǰ����ϵͳ��������
    �����������в���������ָ�����ڴ�С��λ�û���ɫ���͵�
    �����glutInitWindowSize��glutInitDisplayMode��glutInitWindowPosition�ȿ�����glutInit֮ǰ����
    �����ǵ��������в���ʱ��glutInit���Ƴ�֮ǰ�Ĳ������������ô��ڴ�С�ȵȡ�
	*/

	glutInitDisplayMode (GLUT_RGBA);
	/*
	�ú���ָ���˴�����ɫ����
	*/

	glutInitWindowSize (512, 512);
	/*
	���ô��ڴ�С
	*/

	glutInitContextVersion (4, 3);					// ����OpenGL����
	glutInitContextProfile (GLUT_CORE_PROFILE);	// ͬ��
	
	glutCreateWindow (argv[0]);
	/**
	����һ�����ڣ�����Ϊ���ڱ���
	������Ĵ��ڻ����OpenGL�������Ļ���
	�ú���API�ٷ�˵����
	The display state of a window is initially for the window to be shown.
	But the window's display state is not actually acted upon until glutMainLoop is entered.
	This means until glutMainLoop is called, rendering to a created window is ineffective
	because the window can not yet be displayed.
	�򵥵�˵��������ִ����glutMainLoop����֮�󣬶Դ��ڵ���Ⱦ�������á�
	���������glutMainLoop�����ڲ�����ʾ��
	*/

	glewExperimental = GL_TRUE;		
	// glewExperimental �൱һ���ܿ��أ�����������ó� GL_TRUE �Ϳ�����glew֧�����е���չ��glewInit()Ҳ����˳��������еĳ�ʼ����
	
	if (glewInit ()) {
		cerr << "Unable to initialize GLEW ... exiting..." << endl;
		exit (EXIT_FAILURE);
	}
	/**
	�ú�����������һ��������GLEW��OpenGL Extension Wrangler�����ÿ���C/C++����չ�⣬�������ǻ�ȡOpenGL��չ�ĸ��ֺ�����
	������˵����openGL��Windows�µ������
	����΢��Ϊ�����Լ���D3D������Ĭ�϶�openGL��֧���Ǻ����޵ġ�
	��openGL1.1�汾��ʼ����Ҳû�������ˣ���඼ʮ�����ˡ�
	��������Windows�¶���openGL��֧�֣�ȫ���Կ����̡�
	����Ϊ�ˣ����µ����µ��Կ�����Ҳ�Ƿǳ�����ġ�
	���ڲ�һ�����Կ���֧��openGL1�İ汾Ҳ�ǲ�һ���ģ�������Ҫ�ϸ�����վ�鿴��
	Ʃ���ҵ�GT750����֧��openGL4.3
	��Ȼ��װ���������֧�����µ�openGL�ˣ�����΢��û���ṩֱ�ӵ�openGL API������ʹ�������ȽϷ�����
	���ǣ�GLEW���ô������ˣ�����ʵ���Ƕ���Щ������������еķ�װ��ʹ�ó���Ա���Ժܷ���ĵ���glxxx��openGL������
	���ԣ�GLEW�򻯻�ȡ������ַ�Ĺ��̣����������ǵĹ�������
	��˴˺�����ʼ�������ǾͿ�����֮��Ĵ������淽���ʹ����ص�gl������
	*/

	init ();
	/**
	�ú�����ʼ��OpenGL��������ݣ�Ϊ֮�����Ⱦ��׼����
	*/

	glutDisplayFunc (display);
	/**
	�ú���Ϊ���������˻ص����������ڴ����и���ʱ���ûص������ͻ�ִ�С�
	������ָ�����ĵ�ַ��һ������ָ�롣
	*/

	glutMainLoop ();
	/**
	�ú����Ǹ�����ѭ������������ѭ����
	����һֱ�����Ѿ���ע��Ļص��������Լ��û�����Ȳ�����
	*/
}