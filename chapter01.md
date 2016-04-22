# 第一章 OpenGL概述

[TOC]

***
##1.1 什么是OpenGL

[OpenGL<sup>wikipedia</sup>](https://en.wikipedia.org/wiki/OpenGL)是一种应用程序编程接口（API），它是一种可以对图形硬件设备特性进行访问的软件库。

一个用来渲染图像的OpenGL程序需要执行的主要操作如下：
1. 从OpenGL的几何图元中设置数据，用于构建形状。 
2. 使用不同的着色器（shader）对输入的图元数据执行计算操作，判断它们的位置、颜色，以及其他渲染属性。 
3. 将输入图元的数学描述转化为与屏幕位置对应的像素片元（fragment）。这一步也称为光栅化（rasterization）。 
4. 最后，针对光栅化过程产生的每个片元，执行片元着色器（fragment shader），从而决定这个片元的最终颜色和位置。 
5. 如果有必要，还需要对每个片元执行一些额外的操作，例如判断片元对应的对象是否可见，或者将片元的颜色与当前屏幕位置的颜色进行融合。


>何为光栅化（rasterization）？
将输入图元的数学描述转换为与屏幕位置对应的像素片元，称为光栅化。

##1.2 初识OpenGL程序

一些图形学名词：
1. 几何图元，包括点、线、三角形以及Patch。
2. 渲染（render）,表示计算机从模型创建最终图像的过程。
3. 着色器（shader），专为图形处理单元（GPU）编译的一种小型程序。
4. 四种不同的着色阶段（shander stage），其中最常用的包括**顶点着色器**（vertex shader）以及**片元着色器**，前者用于处理顶点数据，后者用于处理光栅化后的片元数据。所有OpenGL程序都需要用到这两类着色器。
5. 帧缓存（framebuffer），像素（pixel），是显示器上最小的可见单元。计算机系统将所有的像素保存到帧缓存当中，后者是有图形硬件设备管理的一块独立内存区域，可以直接映射到最终的显示设备上。

例1.1 第一个OpenGL程序triangles.cpp
[代码（坚果云）](https://www.jianguoyun.com/p/DTz18G4Qg6D2BRiEpRM)
在VS 2013上实现，其中包含了[红书官网](http://www.opengl-redbook.com/)所提供的源文件中的头文件，以及其中的库文件。运行结果如下：
![运行结果](http://images2015.cnblogs.com/blog/731538/201604/731538-20160407231336031-1802356172.png)

##1.3 OpenGL语法

本书所使用的GLUT（OpenGL Utility Toolkit）版本为Freeglut，是原始GLUT库的一个新变种。

##1.4 OpenGL渲染管线

rendering pipeline，它是一系列数据处理过程，并且将应用程序的数据转换到最终渲染的图像。下图为OpenGL4.3版本的管线。包括：
顶点数据，顶点着色器，细分着色器（细分控制着色器，细分计算着色器），几何着色器，图元设置，剪切，光栅化，片元着色器
![RenderingPipeline](https://www.opengl.org/wiki_132/images/thumb/RenderingPipeline.png/270px-RenderingPipeline.png)

###1.4.1 准备向OpenGL传输数据

OpenGL需要将所有的数据都保存到缓存对象（buffer object）中。
我们可以使用多种方式创建这样的数据缓存，最常用的是[glBufferData()<sup>wiki</sup>](https://www.opengl.org/wiki/GLAPI/glBufferData)。

>Buffer Objects are OpenGL Objects that store an array of unformatted memory allocated by the OpenGL context (aka: the GPU). These can be used to store vertex data, pixel data retrieved from images or the framebuffer, and a variety of other things.    
--from wiki

###1.4.2 将数据传输到OpenGL

当将缓存初始化完毕后，通过调用OpenGL的一个绘制命令来请求渲染几何图元。[glDrawArrays()<sup>wiki</sup>](https://www.opengl.org/wiki/GLAPI/glDrawArrays)就是一个常用的绘制命令。OpenGL的绘制通常就是将顶点数据传输到OpenGL服务端。

###1.4.3 顶点着色

对于绘制命令传输的每个顶点，OpenGL都会调用一个顶点着色器来处理顶点相关的数据。
只是将数据复制并传递到下一个着色阶段，叫做传递着色器（pass-through shader）。
通常来说，一个复杂的应用程序可能包含许多顶点着色器，但在同一时刻只能有一个顶点着色器起作用。

###1.4.4 细分着色

顶点着色器处理每个顶点的关联数据之后，如果同时激活了细分着色器，那么它将进一步处理这些数据。（第9章介绍）
细分着色器阶段会用到两个着色器来分别管理Patch数据并产生最终的形状。

###1.4.5 几何着色

第10章介绍。

###1.4.6 图元装配

图元装配将顶点及相关的集合图元之间组织起来，准备下一步剪切和光栅化操作。

###1.4.7 剪切

顶点可能落在视口（viewport）之外，此时与顶点相关的图元会做出改动，以保证相关的像素不会在视口外绘制。剪切（clipping）由OpenGL自动完成。

###1.4.8 光栅化

将更新后的**图元**（primitive）传递到光栅化单元，生成对应的**片元**（fragment）。我们将一个片元是为一个“候选的像素”。也就是可以放置在帧缓存（framebuffer）中的像素，但是它也可能被最终剔除，不再更新对应的像素位置。之后两个阶段将会执行片元的处理。

###1.4.9 片元着色

最后一个可以通过编程控制屏幕上显示颜色的阶段。在Fragment Shader阶段中，我们使用着色器计算片元的最终颜色和它的深度值。
>顶点着色器与片元着色器之间的区别：
顶点着色（包括细分和几何着色）决定了一个图元应该位于屏幕的什么位置，而片元着色使用这些信息来决定某个片元的颜色应该是什么。

###1.4.10 逐片元的操作

在这个阶段会使用深度测试（depth test，或者通常也称为z-bufffering）和模板测试（stencil test）的方式来决定一个片元是否是可见的。

##1.5 第一个程序：深入分析

###1.5.1 进入main()函数

```c++
int main (int argc, char ** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGBA);
	glutInitWindowSize (512, 512);
	glutInitContextVersion (4, 3);					
	glutInitContextProfile (GLUT_CORE_PROFILE);	
	glutCreateWindow (argv[0]);
	glewExperimental = GL_TRUE;		
	
	if (glewInit ()) {
		cerr << "Unable to initialize GLEW ... exiting..." << endl;
		exit (EXIT_FAILURE);
	}
	
	init ();
	
	glutDisplayFunc (display);
	
	glutMainLoop ();
	
```

前面的6行使用GLUT（OpenGL Utility Toolkit）初始化和打开了一个渲染用的窗口：
1. glutInit()负责初始化GLUT库，负责设置其他GLUT例程所必须的数据结构。
2. glutInitDisplayMode()设置了程序所使用的窗口的类型。在这个例子中只设置了窗口使用的RGBA颜色空间。
3. glutInitWindowSize()设置所需的窗口大小。
4. glutInitContextVersion()、glutInitContextProfile设置所需的OpenGL环境（context）的类型。这个例子中使用OpenGL 4.3版本的核心模式（core profile）来创建环境。这个模式确保使用的只是OpenGL的最新特性，否则也可以使用兼容模式，这样自OpenGL 1.0以来的所有特性都可以在程序中使用。
5. glutCreateWindow()，如果当前的系统环境可以满足glutInitDisplayMode()的显示模式要求，这里就会创建一个窗口（此时会调用计算机窗口系统的接口）。只有GLUT创建了一个窗口之后（其中包含创建创建OpenGL环境的过程），我们才可以使用OpenGL相关的函数。
接下来会调用glewInit()函数，属于另一个辅助库GLEW（OpenGL Extention Wrangler）。GLEW可以简化获取函数地址的过程，并且包含了可以跨平台使用的其他一些OpenGL编程方法。

到这，完成了使用OpenGL之前的全部设置工作。之后init()函数初始化OpenGL相关的所有数据。在之后完成渲染工作。

1. glutDisplayFunc()，它设置了一个显示回调（diplay callback），即GLUT在每次更新窗口内容的时候回自动调用该例程。
2. glutMainLoop()，这是一个无限执行的循环，它会负责一直处理窗口和操作系统的用户输入等操作。（注意：不会执行在glutMainLoop()之后的所有命令。）

###1.5.2 OpenGL的初始化过程

```c++
void init (void)
{
	glGenVertexArrays (NumVAOs, VAOs);
	glBindVertexArray (VAOs[Triangles]);
	GLfloat vertices[NumVertices][2] = {
		{ -0.90, -0.90 },		// Triangle 1
		{ 0.85, -0.90 },
		{ -0.90, 0.85 },
		{ 0.90, -0.85 },		// Triangle 2,
		{ 0.90,  0.90 },
		{ -0.85, 0.90 }
	};

	glGenBuffers (NumBuffers, Buffers);
	glBindBuffer (GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);
	ShaderInfo shaders[] = {							
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders (shaders);	
	glUseProgram (program);
	glVertexAttribPointer (vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET (0));
	glEnableVertexAttribArray (vPosition);
}
```

####*初始化顶点数组对象*

- glGenVertexArrays(NumVAOs, VAOs)分配顶点数组对象（vertext array object）。OpenGL会因此分配一部分（NumVAOs个）顶点数组对象的**名称**供我们使用，保存到数组VAOs中。

> void glGenVertexArrays(GLsizei n​, GLuint *arrays​);
n
Specifies the number of vertex array object names to generate.
arrays
Specifies an array in which the generated vertex array object names are stored.
The names returned in arrays​ are marked as used, for the purposes of glGenVertexArrays only, but they acquire state and type only when they are first bound.

很多OpenGL命令都是glGen*的形式，它们负责分配不同类型的OpenGL对象的名称。这里的名称类似C语言中的指针变量，必须分配内存并且用名称引用它之后，名称才有意义。在OpenGL中，这个分配的机制叫做绑定对象（bind an object）。这通过一系列glBind*形式的OpenGL函数集合去实现。

- glBindVertexArray (VAOs[Triangles])创建一个顶点数组对象，并与其**名称**（VAOs[Triangles]）关联起来。
> void glBindVertexArray(GLuint array​);
array
Specifies the name of the vertex array to bind.


- 当我们第一次绑定对象时（例如，第一次用指定的对象名作为参数调用glBind*()），OpenGL内部会分配这个对象所需的内存并且将它作为当前对象，即后继的操作都会作用于这个被绑定的对象。例如，这里的顶点数组对象就会被后面执行的代码所改变。
有两种情况我们需要绑定一个对象：
1. 创建对象并初始化它所对应的数据时。
2. 每次我们准备使用这个对象，而它并不是当前绑定的对象时。

- （未使用）glDeleteVertexArrays()，当我们完成对顶点数组对象的操作之后，可以调用此函数将它（们）释放。
> void glDeleteVertexArrays(GLsizei n​, const GLuint *arrays​);
n
Specifies the number of vertex array objects to be deleted.
arrays
Specifies the address of an array containing the n​ names of the objects to be deleted.

- （未使用）glIsVertexArray()，检查某个名称是否已经关联到一个顶点数组对象了。
> GLboolean glIsVertexArray(GLuint array​);
array
Specifies a value that may be the name of a vertex array object.

####*分配顶点缓存对象*

顶点数组对象（VAO）负责保存一系列顶点的数据。这些数据保存到缓存对象（Buffer Object）中，并且由当前绑定的顶点数组对象管理。
>官方解释：**A Vertex Array Object (VAO) is an OpenGL Object that stores all of the state needed to supply vertex data (with one minor exception noted below). It stores the format of the vertex data as well as the Buffer Objects providing the vertex data arrays.** 

- glGenBuffers (NumBuffers, Buffers) 返回NumBuffers个当前未使用的缓存对象名称到数组Buffers中。（名称不一定是连续的整型数据）
> void glGenBuffers(GLsizei n​, GLuint * buffers​);
n
Specifies the number of buffer object names to be generated.
buffers
Specifies an array in which the generated buffer object names are stored.
No buffer objects are associated with the returned buffer object names until they are first bound by calling glBindBuffer​.

- glBindBuffer (GL_ARRAY_BUFFER, Buffers[ArrayBuffer])在分配缓存的名称之后，就可以调用glBindBuffer()来绑定它们了。由于OpenGL中有很多种不同类型的缓存对象，因此绑定一个缓存时，需要指定它所对应的类型。此例中由于是将顶点数据保存到缓存中（故为顶点缓存对象VBO），因此使用GL_ARRAY_BUFFER类型。
> void glBindBuffer(GLenum target​, GLuint buffer​);
target
Specifies the target buffer object. The symbolic constant must be GL_ARRAY_BUFFER, GL_ATOMIC_COUNTER_BUFFER, GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, GL_DRAW_INDIRECT_BUFFER, GL_DISPATCH_INDIRECT_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER, GL_QUERY_BUFFER, GL_SHADER_STORAGE_BUFFER, GL_TEXTURE_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, or GL_UNIFORM_BUFFER.
buffer
Specifies the name of a buffer object.
When a buffer object is bound to a target, the previous binding for that target is automatically broken.

- （未使用）glDeleteBuffers()，所有的缓存对象都可以使用glDeleteBuffers()直接释放。
> void glDeleteBuffers(GLsizei n​, const GLuint * buffers​);
n
Specifies the number of buffer objects to be deleted.
buffers
Specifies an array of buffer objects to be deleted.

- glIsBuffer()，使用此函数来判断一个整数值是否为缓存对象的名称。
> GLboolean glIsBuffer(GLuint buffer);
buffer
Specifies a value that may be the name of a buffer object.
A name returned by glGenBuffers​, but not yet associated with a buffer object by calling glBindBuffer​, is not the name of a buffer object.

####*将数据载入缓存对象*

- glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW)，初始化顶点缓存对象之后（VBO），我们需要把顶点数据传输到缓存对象中。它主要有两个任务：分配顶点数据所需的存储空间（内存中），然后将数据从应用程序的数组中拷贝到OpenGL服务端的内存中。
>void glBufferData(GLenum target​, GLsizeiptr size​, const GLvoid * data​, GLenum usage​);
target
Specifies the target buffer object. The symbolic constant must be GL_ARRAY_BUFFER, GL_ATOMIC_COUNTER_BUFFER, GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, GL_DRAW_INDIRECT_BUFFER, GL_DISPATCH_INDIRECT_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER, GL_QUERY_BUFFER, GL_SHADER_STORAGE_BUFFER, GL_TEXTURE_BUFFER, GL_TRANSFORM_FEEDBACK_BUFFER, or GL_UNIFORM_BUFFER.
size
Specifies the size in bytes of the buffer object's new data store.
data
Specifies a pointer to data that will be copied into the data store for initialization, or NULL if no data is to be copied.
usage
Specifies the expected usage pattern of the data store. The symbolic constant must be GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY.


####*初始化顶点与片元着色器*

- 对于每一个OpenGL程序，当它所使用的OpenGL版本高于或等于3.1时，就需要指定至少两个着色器：顶点着色器和片元着色器。这个例子中，我们使用[LoadShaders()](https://www.jianguoyun.com/p/DeN644EQg6D2BRjXqxM)来实现这个要求。
对于OpenGL程序员而言，着色器就是使用OpenGL着色语言（GLSL，OpenGL Shading Language）编写的一个小型函数。

例1.2 triangles.cpp对应的顶点着色器：triangles.vert

    #version 430 core
    layout (location = 0) in vec4 vPosition;
    void
    main ()
    {
    		gl_Position = vPosition;
    }

事实上这就是我们所说的传递着色器（pass-through shader）。它只负责将输入数据拷贝到输出数据中。
第一行

     #version 430 core

指定了OpenGL着色语言的版本。每个着色器的第一行都应该设置#version，否则就会假设使用“110”版本。
下一步，分配了一个着色器变量，着色器变量是着色器与外部世界的联系所在。着色器并不知道自己的数据从哪里来，它只是在每次运行时直接获取数据对应的输入变量。而我们必须自己完成**着色管线的装配**，然后才可以将应用程序中的数据与不同的OpenGL着色阶段相互关联。

    layout (location = 0) in vec4 vPosition;

- vPosition 是变量的名称。这个变量保存的是顶点的位置信息。（字符“v”作为这个顶点属性名称的前缀）
- vec4，是vPosition的类型，在这里它是一个GLSL的四维浮点数向量。、
- in，它指定了数据进入着色器的流向。
- layout(location = 0)，叫做**布局限定符**（layout qualifier），目的是为变量提供元数据（meta data）。*这里设置vPosition的位置属性location为0。这个设置与init()函数的最后两行会共同起作用。*

最后，在着色器的main()函数中实现它的主体部分。对于这个着色器而言，它所实现的就是将输入的顶点位置（存在vPosition中）复制到**顶点着色器的指定输出位置gl_Position**。

例1.3 triangles.cpp对应的片元着色器：triangles.frag

    @version 430 core
    out vec4 fColor
    void
    main()
    {
        fColor = vec4(0.0, 0.0, 1.0, 1.0);
    }

- 变量名为fColor，使用了out限定符，在这里着色器将会把fColor对应的数值输出，而这也就是片元所对应的颜色值。（因此这里前缀“f”）
- OpenGL使用了RGBA颜色空间，A即aplha值（透明度）。1.0为不透明。

init()中的最后两个函数指定了顶点着色器的变量与我们存储在缓存对象中的数据的关系。这也就是我们所说的**着色管线的装配过程**，即将程序与着色器之间，以及不同着色阶段之间的数据通道连接起来。

- glVertexAttribPointer (vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET (0));
为了输入顶点着色器所需的数据，也就是OpenGL将要处理的所有顶点数据，需要在着色器中声明一个in变量，然后使用glVertexAttribPointer()将它关联到一个顶点属性数组。glVertexAttribPointer()是一个非常灵活的命令，只要内存中的数据是规范组织的（保存在一个连续的数组中，不使用其他基于节点的容器，如链表），我们就可以使用glVertexArrtibPointer()告诉OpenGL直接从内存中获取数据。
> void glVertexAttribPointer(GLuint index​, GLint size​, GLenum type​, GLboolean normalized​, GLsizei stride​, const GLvoid * pointer​);
 void glVertexAttribIPointer(GLuint index​, GLint size​, GLenum type​, GLsizei stride​, const GLvoid * pointer​);
 void glVertexAttribLPointer(GLuint index​, GLint size​, GLenum type​, GLsizei stride​, const GLvoid * pointer​);
index
Specifies the index of the generic vertex attribute to be modified.
size
Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4. Additionally, the symbolic constant GL_BGRA is accepted by glVertexAttribPointer. The initial value is 4.
type
Specifies the data type of each component in the array. The different functions take different values.
glVertexAttribPointer and glVertexAttribIPointer both take: GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, and GL_UNSIGNED_INT
glVertexAttribPointer also can take: GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, and GL_UNSIGNED_INT_10F_11F_11F_REV.
glVertexAttribLPointer takes only GL_DOUBLE.
The initial value is GL_FLOAT.
normalized
For glVertexAttribPointer, specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
stride
Specifies the byte offset between consecutive generic vertex attributes. If stride​ is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
pointer
Specifies a offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.

- glEnableVertexAttribArray (vPosition);
我们通过glEnableVertexAttribArray()来启用顶点属性数组。glVertexAttribPointer()初始化的属性数组指针索引传入这个函数。
>void glEnableVertexAttribArray(GLuint index​);
 void glDisableVertexAttribArray(GLuint index​);
index
Specifies the index of the generic vertex attribute to be enabled or disabled.


###1.5.3 第一次使用OpenGL进行渲染
```c++
void display (void)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glBindVertexArray (VAOs[Triangles]);
	glDrawArrays (GL_TRIANGLES, 0, NumVertices);
	glFlush ();
}
```

- glClear(GL_COLOR_BUFFER_BIT)，清除帧缓存的数据。
> void glClear(GLbitfield mask​);
mask
Bitwise OR of masks that indicate the buffers to be cleared. The three masks are GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, and GL_STENCIL_BUFFER_BIT.

- （未使用）glClearColor(GLfloat red​, GLfloat green​, GLfloat blue​, GLfloat alpha​)，设置当前使用的清除颜色值，用于RGBA模式下对颜色缓存的清除工作。
>清除颜色本身也是OpenGL状态机制的一个例子，它的数值会一直保留在当前OpenGL环境当中。
**OpenGL有一个庞大的状态量列表。当创建一个新的OpenGL环境时，所有的状态量都会被初始化为默认值。因为OpenGL会保留所有更改的状态值，所以我们可以减少设置状态数值的次数。**
故而，在设置清除颜色为白色时，可以在display()函数中调用glClearColor(1, 1, 1, 1)，也可以在init()函数中调用glClearColor(1, 1, 1, 1)，后者效率更高，因为可以避免冗余的状态切换。

- glBindVertexArray (VAOs[Triangles])，选择作为顶点数据使用的顶点数组。我们可以使用这个函数来切换程序中保存的多个顶点数据对象集合。

- glDrawArrays (GL_TRIANGLES, 0, NumVertices)，使用**当前绑定的顶点数组元素**来建立一系列的几何图元。实现顶点数据向OpenGL管线的传输。
>glDrawArrays: render primitives from array data
void glDrawArrays(GLenum mode​, GLint first​, GLsizei count​);
mode
Specifies what kind of primitives to render. Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted.
first
Specifies the starting index in the enabled arrays.
count
Specifies the number of indices to be rendered.

- glFlush ()，强制所有进行中的OpenGL命令立即完成并传输到OpenGL服务端处理。
>glFlush: force execution of GL commands in finite time
 void glFlush(void​);

- （未使用）glFinish()，会等待所有当前的OpenGL操作完成。而glFlush()则只是强制所有运行中的命令送入OpenGL服务端而已，不会等待所有的命令完成。因此当我们需要了解OpenGL是在是么时候完成操作的时候使用glFinish()。
>glFinish: block until all GL execution is complete
void glFinish(void​);
glFinish does not return until the effects of all previously called GL commands are complete. Such effects include all changes to GL state, all changes to connection state, and all changes to the frame buffer contents.

- （未使用）glEnable()，glDisable()，启用或禁用OpenGL的操作模式。
>glEnable: enable or disable server-side GL capabilities
 void glEnable(GLenum cap​);
 void glDisable(GLenum cap​);
cap
Specifies a symbolic constant indicating a GL capability.
- （未使用）glIsEnabled()，返回是否启用指定模式。
>glIsEnabled, glIsEnabledi: test whether a capability is enabled
GLboolean glIsEnabled(GLenum cap​);
 GLboolean glIsEnabledi(GLenum cap​, GLuint index​);
cap
Specifies a symbolic constant indicating a GL capability.
index
Specifies the index of the capability.


***
*相关链接：*
- [OpenGL WIKI](https://www.opengl.org/wiki/Main_Page)
- [关于VAO && VBO官方](https://www.opengl.org/wiki/Vertex_Specification)
- [关于VAO && VBO博客](http://www.cnblogs.com/MyGameAndYOU/p/4678657.html)
- [GLUT API](https://www.opengl.org/resources/libraries/glut/spec3/spec3.html)
- [FreeGLUT API](http://freeglut.sourceforge.net/docs/api.php)
- [GLEW 官网](http://glew.sourceforge.net/)
