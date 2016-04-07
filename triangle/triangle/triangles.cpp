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

GLuint VAOs[NumVAOs];		// 来存储顶点数组对象名
GLuint Buffers[NumBuffers];		// 来存储顶点缓存对象名

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
		分配顶点数组对象的名字
	返回n个未使用的对象名到数组arrays中，用作顶点数组对象（VAO = Vertex Array Object）。返回的名字可以用来分配更多的缓存对象，
	并且它们已经使用未初始化的顶点数组集合的默认状态进行了数值的初始化。
	在OpenGL中，VAO负责管理和顶点（Vertices）集合相关联的各种数据。
	但是这些数据我们是保存到顶点缓存对象中（Vertex Buffer Object）,简称VBO。
	*/

	glBindVertexArray (VAOs[Triangles]);
	/*
	void glBindVertexArray (GLuint array);
		绑定一个顶点数组对象，即创建一个新的顶点数组对象并且与其名称关联起来
	对于这个函数，VAOs[Triangles]中保存着glGenVertexArrays执行完成后分配的对象名，而它作为参数传入该函数。
	第一次绑定对象时，OpenGL内部会分配这个对象所需的内存并且将它作为当前对象。
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
	返回n个当前未使用的缓存对象名称，并保存到buffers数组中。
	*/

	glBindBuffer (GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	/*
	void glBindBuffer (GLenum target, GLuint buffer);
		初始化顶点缓存对象
	完成三项工作：
		1. 如果是第一次绑定buffer，且它是一个非零的无符号整型，那么将创建一个与该名称相对应的新缓存对象。
		2. 如果绑定到一个已经创建的缓存对象中，那么它将成为当前被激活的缓存对象。
		3. 如果绑定的buffer值为0，那么OpenGL将不再对当前target应用任何缓存对象。
	*/

	glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
	/*
	void glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
		将数据载入缓存对象
	主要有两个任务：
		1. 分配顶点数据所需的存储空间
		2. 将数据从应用程序的数组中拷贝到OpenGL服务端的内存中。
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
	函数原型：void glVertexAttribPointer(GLuint index, GLint size, GLenum type,
	GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	之前我们调用了glBindData所传递给缓存的只是数据，之后我们要使用它，还必须指定数据类型。
	所以该函数完成的主要任务是：
	1、告诉OpenGL，该存储数据的格式
	2、因为我们使用着色器来编程，因此在顶点着色器阶段，我们会使用该函数来给着色器中的in类型的属性变量传递数据。
	那么它们是怎么联系起来的呢?
	便是通过第一个参数index，指明了着色器程序中变量的下标的作用。
	例如：layout( location=index ) in vec4 position;
	如果这个index和glVertexAttribPointer的第一个参数一样，那么相关缓存区的数据就会传递到这个position变量中去。
	*/

	glEnableVertexAttribArray (vPosition);
	/**
	因为默认情况下，顶点属性数组是不可访问的，所以我们需要调用以下函数激活它。
	范围为0到GL_MAX_VERTEX_ATTRIBS-1
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
	清除指定的缓存数据并且重新设置为当前的清除值
	参数可以通过逻辑或操作来指定多个数值参数。
	GL_COLOR_BUFFER_BIT 颜色缓存
	GL_DEPTH_BUFFER_BIT 深度缓存
	GL_STENCIL_BUFFER_BIT 模板缓存

	而OpenGL默认的清除颜色是黑色
	如果要指定其他颜色，可以调用glClearColor

	另外我们还需要知道，因为OpenGL是一种状态机，因此对它的所有设定OpenGL都会保留。
	所以对于glClearColor，最好的调用方法是放在初始化方法中，因为这样它只会被调用一次
	如果放在display中，OpenGL这样每一帧都会调用它重复设置清除颜色值，会降低运行效率。
	*/


	glBindVertexArray (VAOs[Triangles]);
	/**
	该函数我们在初始化函数见过，不过这次的功能不太一样
	之前是初始化，现在是激活该顶点数组对象。
	现在使用它作为顶点数据所使用的顶点数组。
	*/

	glDrawArrays (GL_TRIANGLES, 0, NumVertices);
	/**
	函数原型void glDrawArrays(GLenum mode, GLint first, GLsizei count);
	设置了渲染模式为GL_TRIANGLES，起始位置位于缓存的0偏移位置，一共渲染NumVertices个元素。
	以后我们会继续学习各种图元
	*/

	glFlush ();
	/**
	该函数强制吧所有进行中的OpenGL命令立即完成并传输到OpenGL服务端处理。
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
	该函数用来初始化GLUT库,之后会与当前窗口系统产生交互
    　关于命令行参数，可以指定窗口大小、位置或颜色类型等
    　因此glutInitWindowSize、glutInitDisplayMode或glutInitWindowPosition等可以在glutInit之前运行
    　但是当命令行有参数时，glutInit会移除之前的操作，比如设置窗口大小等等。
	*/

	glutInitDisplayMode (GLUT_RGBA);
	/*
	该函数指定了窗口颜色类型
	*/

	glutInitWindowSize (512, 512);
	/*
	设置窗口大小
	*/

	glutInitContextVersion (4, 3);					// 设置OpenGL环境
	glutInitContextProfile (GLUT_CORE_PROFILE);	// 同上
	
	glutCreateWindow (argv[0]);
	/**
	创建一个窗口，参数为窗口标题
	创建完的窗口会关联OpenGL的上下文环境
	该函数API官方说明：
	The display state of a window is initially for the window to be shown.
	But the window's display state is not actually acted upon until glutMainLoop is entered.
	This means until glutMainLoop is called, rendering to a created window is ineffective
	because the window can not yet be displayed.
	简单的说，就是在执行了glutMainLoop函数之后，对窗口的渲染才起作用。
	如果不调用glutMainLoop，窗口不会显示。
	*/

	glewExperimental = GL_TRUE;		
	// glewExperimental 相当一个总开关，如果将它设置成 GL_TRUE 就可以让glew支持所有的拓展，glewInit()也可以顺利完成所有的初始化。
	
	if (glewInit ()) {
		cerr << "Unable to initialize GLEW ... exiting..." << endl;
		exit (EXIT_FAILURE);
	}
	/**
	该函数属于另外一个辅助库GLEW（OpenGL Extension Wrangler），该库是C/C++的扩展库，方便我们获取OpenGL扩展的各种函数。
	而这里说明下openGL在Windows下的情况。
	万恶的微软为了推自己的D3D，所以默认对openGL的支持是很有限的。
	从openGL1.1版本开始就再也没有升级了，差不多都十多年了。
	所以现在Windows下对于openGL的支持，全靠显卡厂商。
	正因为此，更新到最新的显卡驱动也是非常必须的。
	对于不一样的显卡，支持openGL1的版本也是不一样的，具体需要上各家网站查看。
	譬如我的GT750，就支持openGL4.3
	虽然安装完驱动后就支持最新的openGL了，但是微软并没有提供直接的openGL API，导致使用起来比较繁琐。
	于是，GLEW得用处就来了，他其实就是对这些繁琐的事情进行的封装，使得程序员可以很方便的调用glxxx的openGL函数。
	所以，GLEW简化获取函数地址的过程，减少了我们的工作量！
	因此此函数初始化后，我们就可以在之后的代码里面方便地使用相关的gl函数。
	*/

	init ();
	/**
	该函数初始化OpenGL的相关数据，为之后的渲染做准备。
	*/

	glutDisplayFunc (display);
	/**
	该函数为窗口设置了回调函数，即在窗口有更新时，该回调函数就会执行。
	参数是指函数的地址，一个函数指针。
	*/

	glutMainLoop ();
	/**
	该函数是个无限循环函数，即死循环。
	它会一直处理已经被注册的回调函数，以及用户输入等操作。
	*/
}