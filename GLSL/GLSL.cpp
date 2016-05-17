// GLSL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <string>
#include "include/glew.h"
#include "include/GLUT.H"


























#include <opencv2/opencv.hpp>
#include "shader.h"
#define height_width 512

void show(int height=height_width,int width=height_width)
{
	cv::Mat img=cv::Mat::zeros(height, width, CV_8UC3);

	//use fast 4-byte alignment (default anyway) if possible
	//glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);

	//set length of one complete row in destination data (doesn't need to equal img.cols)
//glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());
	glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);//opencv存储为BGR顺序
//cv::flip(img, img, 0);//需要翻转
	cv::imshow("result",img);
	cv::waitKey(0);

}
#include "include/glew.h" // include GLEW and new version of GL on Windows
#include <stdio.h>
#include <assert.h>

/* window global variables */
int g_gl_width = 512;
int g_gl_height = 512;


/* global variables for the secondary framebuffer handle and attached texture */

GLuint g_fb_tex = 0;
/* global variable for the screen-space quad that we will draw */
GLuint g_ss_quad_vao = 0;

/* initialise secondary framebuffer. this will just allow us to render our main
scene to a texture instead of directly to the screen. returns false if something
went wrong in the framebuffer creation */
GLuint init_fb () 
{
	GLuint g_fb = 0;
	glGenFramebuffers (1, &g_fb);
	/* create the texture that will be attached to the fb. should be the same
	dimensions as the viewport */
	glGenTextures (1, &g_fb_tex);
	glBindTexture (GL_TEXTURE_2D, g_fb_tex);
	glTexImage2D (
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		g_gl_width,
		g_gl_height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/* attach the texture to the framebuffer */
	glBindFramebuffer (GL_FRAMEBUFFER, g_fb);
	glFramebufferTexture2D (
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_fb_tex, 0
	);
	/* create a renderbuffer which allows depth-testing in the framebuffer */
	GLuint rb = 0;
	glGenRenderbuffers (1, &rb);
	glBindRenderbuffer (GL_RENDERBUFFER, rb);
	glRenderbufferStorage (
		GL_RENDERBUFFER, GL_DEPTH_COMPONENT, g_gl_width, g_gl_height
	);
	/* attach renderbuffer to framebuffer */
	glFramebufferRenderbuffer (
		GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb
	);
	/* tell the framebuffer to expect a colour output attachment (our texture) */
	GLenum draw_bufs[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers (1, draw_bufs);
	
	/* validate the framebuffer - an 'incomplete' error tells us if an invalid
	image format is attached or if the glDrawBuffers information is invalid */
	GLenum status = glCheckFramebufferStatus (GL_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != status) {
		fprintf (stderr, "ERROR: incomplete framebuffer\n");
		if (GL_FRAMEBUFFER_UNDEFINED == status) {
			fprintf (stderr, "GL_FRAMEBUFFER_UNDEFINED\n");
		} else if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == status) {
			fprintf (stderr, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
		} else if (GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT == status) {
			fprintf (stderr, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
		} else if (GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER == status) {
			fprintf (stderr, "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n");
		} else if (GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER== status) {
			fprintf (stderr, "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");
		} else if (GL_FRAMEBUFFER_UNSUPPORTED == status) {
			fprintf (stderr, "GL_FRAMEBUFFER_UNSUPPORTED\n");
		} else if (GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE == status) {
			fprintf (stderr, "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n");
		} else if (GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS == status) {
			fprintf (stderr, "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n");
		} else {
			fprintf (stderr, "unspecified error\n");
		}
		return false;
	}
	
	/* re-bind the default framebuffer as a safe precaution */
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
	return g_fb;
}

void init_ss_quad () {
	// x,y vertex positions
	GLfloat ss_quad_pos[] = {
		-1.0, -1.0,
		1.0, -1.0,
		1.0,  1.0,
		1.0,  1.0,
		-1.0,  1.0,
		-1.0, -1.0
	};
	// create VBOs and VAO in the usual way
	glGenVertexArrays (1, &g_ss_quad_vao);
	glBindVertexArray (g_ss_quad_vao);
	GLuint vbo;
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (
		GL_ARRAY_BUFFER,
		sizeof (ss_quad_pos),
		ss_quad_pos,
		GL_STATIC_DRAW
		);
	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (0);
}
int main( int argc, char** argv )
{

//1、初始化环境
	glutInit(&argc, argv);
	glutCreateWindow("GLEW Test"); 
	
	glewExperimental = GL_TRUE;
	glewInit();
	if (!glewIsSupported("GL_VERSION_4_0")) 
	{
		std::cerr << "Failed to initialize GLEW with OpenGL 4.0!" << std::endl;
		return EXIT_FAILURE;
	}
	GLint g_fb=init_fb ();
	//init_ss_quad ();
//2、读取、创建shader程序，编译连接等
	auto program_id = ShaderProgram("shader/gl_texture.vert", "shader/gl_texture.frag");
	glUseProgram(program_id);

//3、设置纹理相关参数、
	auto texture_id = LoadImage("1.jpg", height_width, height_width);//读入一张图片，转成纹理格式，把并把图片数据拷贝到opengl纹理单元。
	glActiveTexture(GL_TEXTURE0 + 0);//启用第一个纹理，并绑定纹理数据
	glBindTexture(GL_TEXTURE_2D, texture_id);
//4.输入shader计算所需要的数据

	//auto texture_attrib = glGetUniformLocation(program_id,"Texture");//找到shader程序中，变量名为Texture，类型为uniform的变量索引
	//glUniform1i(texture_attrib,0);

	//auto time=glGetUniformLocation(program_id,"time");
	//glUniform1f(time,100);
	auto time=glGetUniformLocation(program_id,"co");
	glUniform1f(time,1.);
	//glBindFragDataLocation(program_id,0.5, "co");


//4、设置渲染相关参数（矩形4个顶点、及其对应的纹理坐标）
	InitializeGeometry(program_id);
//5、绘制、渲染


	
	
/*
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);*/





	glBindFramebuffer (GL_FRAMEBUFFER, g_fb);

		glClearColor (0, 0, 0., 1.0);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glDrawArrays (GL_TRIANGLES, 0, 8);
		


		glFlush ();
	glFinish ();


//6、取回数据到opencv，并显示结果
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	show();


	// edge detection
	//int filter_size;
//double ** filter = load_kernel(KERNEL_FILE, &filter_size);






	return 0;
}


