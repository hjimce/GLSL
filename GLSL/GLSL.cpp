// GLSL.cpp : �������̨Ӧ�ó������ڵ㡣
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
	glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);//opencv�洢ΪBGR˳��
//cv::flip(img, img, 0);//��Ҫ��ת
	cv::imshow("result",img);
	cv::waitKey(0);

}


int main( int argc, char** argv )
{

	 const GLubyte* OpenGLVersion =glGetString(GL_VERSION); 
//1����ʼ������
	glutInit(&argc, argv);
	glutCreateWindow("GLEW Test"); 
	glewExperimental = GL_TRUE;
	glewInit();
	if (!glewIsSupported("GL_VERSION_4_0")) 
	{
		std::cerr << "Failed to initialize GLEW with OpenGL 4.0!" << std::endl;
		return EXIT_FAILURE;
	}
//2����ȡ������shader���򣬱������ӵ�
	auto program_id = ShaderProgram("gl_texture.vert", "gl_texture.frag");
	glUseProgram(program_id);

//3������������ز�������������shader��������Ҫ������
	auto texture_id = LoadImage("1.jpg", height_width, height_width);//����һ��ͼƬ��ת�������ʽ���Ѳ���ͼƬ���ݿ�����opengl����Ԫ��
	auto texture_attrib = glGetUniformLocation(program_id, "Texture");//�ҵ�shader�����У�������ΪTexture������Ϊuniform�ı�������
	glUniform1i(texture_attrib,0);
	glActiveTexture(GL_TEXTURE0 + 0);//���õ�һ������������������
	glBindTexture(GL_TEXTURE_2D, texture_id);


//4��������Ⱦ��ز���������4�����㡢�����Ӧ���������꣩
	InitializeGeometry(program_id);
//5�����ơ���Ⱦ
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//6��ȡ�����ݵ�opencv������ʾ���
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
	//��ʾģʽ��ʼ��
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	//���崰�ڴ�С
	glutInitWindowSize(300,300);
	//���崰��λ��
	glutInitWindowPosition(100,100);
	//��������
	glutCreateWindow("OpenGL Version");
	const GLubyte* name = glGetString(GL_VENDOR); //���ظ���ǰOpenGLʵ�ֳ��̵�����
	const GLubyte* biaoshifu = glGetString(GL_RENDERER); //����һ����Ⱦ����ʶ����ͨ���Ǹ�Ӳ��ƽ̨
	const GLubyte* OpenGLVersion =glGetString(GL_VERSION); //���ص�ǰOpenGLʵ�ֵİ汾��
	const GLubyte* gluVersion= gluGetString(GLU_VERSION); //���ص�ǰGLU���߿�汾
	printf("OpenGLʵ�ֳ��̵����֣�%s\n", name);
	printf("��Ⱦ����ʶ����%s\n", biaoshifu);
	printf("OOpenGLʵ�ֵİ汾�ţ�%s\n",OpenGLVersion );
	printf("OGLU���߿�汾��%s\n", gluVersion);
	return 0;
}*/