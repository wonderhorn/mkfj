#pragma once
#include<iostream>
#include<vector>
#include<GLES2/gl2.h>
#include<GLES2/gl2ext.h>
#include<android/log.h>

#ifndef GL_TEXTURE_RECTANGLE_EXT
#define GL_TEXTURE_RECTANGLE_EXT 0x84f5
#endif

class GLGraphic
{
public:
	GLGraphic();
	bool init(struct android_app* app, GLuint rmat, GLuint tmat, GLuint mmat, GLuint utexture);
	GLuint loadImage(const std::string& file_name, int colorkey = 0x00000000);
	void draw2DRectangle(GLuint tex, float x, float y, float z,
		float w, float h, float texx = 0.0f, float texy = 0.0f,
		float texw = 1.0f, float texh = 1.0f, float magx = 1.0, float magy = 1.0);
	void draw2DRectangleRotate(GLuint tex, float angle, float x, float y, float z,
		float w, float h, float texx = 0.0f, float texy = 0.0f,
		float texw = 1.0f, float texh = 1.0f, float magx = 1.0, float magy = 1.0);
	void drawString(const std::string& str, float x, float y);
	GLuint test_texture;

private:
	struct android_app* app;
	GLuint rmat, tmat, mmat, utexture; //location of uniforms in the GLSL program
	//struct engine* engine;
	//std::vector<GLuint> handles;
};