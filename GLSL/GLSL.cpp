// GLSL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
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


int main( int argc, char** argv )
{

	 const GLubyte* OpenGLVersion =glGetString(GL_VERSION); 
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
//2、读取、创建shader程序，编译连接等
	auto program_id = ShaderProgram("gl_texture.vert", "gl_texture.frag");
	glUseProgram(program_id);

//3、设置纹理相关参数、或者输入shader计算所需要的数据
	auto texture_id = LoadImage("1.jpg", height_width, height_width);//读入一张图片，转成纹理格式，把并把图片数据拷贝到opengl纹理单元。
	auto texture_attrib = glGetUniformLocation(program_id, "Texture");//找到shader程序中，变量名为Texture，类型为uniform的变量索引
	glUniform1i(texture_attrib,0);
	glActiveTexture(GL_TEXTURE0 + 0);//启用第一个纹理，并绑定纹理数据
	glBindTexture(GL_TEXTURE_2D, texture_id);


//4、设置渲染相关参数（矩形4个顶点、及其对应的纹理坐标）
	InitializeGeometry(program_id);
//5、绘制、渲染
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//6、取回数据到opencv，并显示结果
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	show();


	// edge detection
	//int filter_size;
//double ** filter = load_kernel(KERNEL_FILE, &filter_size);






	return 0;
}


/*

#include<Windows.h>
#include <stdio.h>
#include "include/glut.h"

int main(int argc, char** argv)
{
	glutInit(&argc,argv);
	//显示模式初始化
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	//定义窗口大小
	glutInitWindowSize(300,300);
	//定义窗口位置
	glutInitWindowPosition(100,100);
	//创建窗口
	glutCreateWindow("OpenGL Version");
	const GLubyte* name = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字
	const GLubyte* biaoshifu = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台
	const GLubyte* OpenGLVersion =glGetString(GL_VERSION); //返回当前OpenGL实现的版本号
	const GLubyte* gluVersion= gluGetString(GLU_VERSION); //返回当前GLU工具库版本
	printf("OpenGL实现厂商的名字：%s\n", name);
	printf("渲染器标识符：%s\n", biaoshifu);
	printf("OOpenGL实现的版本号：%s\n",OpenGLVersion );
	printf("OGLU工具库版本：%s\n", gluVersion);
	return 0;
}*/