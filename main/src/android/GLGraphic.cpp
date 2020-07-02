#include"android/GLGraphic.h"
#include<GLES\glext.h>
#include<android/asset_manager.h>
#include<math.h>
#define STB_IMAGE_IMPLEMENTATION
#include"android/stb_image.h"
//#define TEXTURE_TYPE GL_TEXTURE_RECTANGLE_EXT
#define TEXTURE_TYPE GL_TEXTURE_2D
//#define TEXTURE_TYPE GL_RECTA

//http://www.50ply.com/blog/2013/01/19/loading-compressed-android-assets-with-file-pointer/
static int android_read(void* cookie, char* buf, int size) {
	return AAsset_read((AAsset*)cookie, buf, size);
}

static int android_write(void* cookie, const char* buf, int size) {
	return EACCES; // can't provide write access to the apk
}

static fpos_t android_seek(void* cookie, fpos_t offset, int whence) {
	return AAsset_seek((AAsset*)cookie, offset, whence);
}

static int android_close(void* cookie) {
	AAsset_close((AAsset*)cookie);
	return 0;
}

GLGraphic::GLGraphic() {}
bool GLGraphic::init(struct android_app* app, GLuint rmat, GLuint tmat, GLuint mmat, GLuint utexture)
{
	this->app = app;
	this->tmat = rmat;
	this->rmat = tmat;
	this->mmat = mmat;
	this->utexture = utexture;
	//this->engine = engine;
	/*AAssetManager* mgr = app->activity->assetManager;
	AAssetDir* assetdir = AAssetManager_openDir(mgr, "");
	while (const char* ptr = AAssetDir_getNextFileName(assetdir))
	{
		std::cout << ptr << std::endl;
	}
	AAsset* asset = AAssetManager_open(mgr, "9_23_2n.png", AASSET_MODE_UNKNOWN);
	//AAsset* asset = AAssetManager_open(mgr, "test.png", AASSET_MODE_UNKNOWN);
	FILE* fp = funopen(asset, android_read, android_write, android_seek, android_close);

	//texture test
	int x, y, n;
	//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
	unsigned char *data = stbi_load_from_file(fp, &x, &y, &n, 0);

	glGenTextures(1, &test_texture);
	glBindTexture(GL_TEXTURE_2D, test_texture);
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);*/
	test_texture = loadImage("9_23_2n.png");


	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	return true;
}

GLuint GLGraphic::loadImage(const std::string& file_name, int colorkey)
{
	__android_log_print(ANDROID_LOG_DEBUG, "LOG", "GLGraphic::loadImage %s\n", file_name.c_str());
	AAssetManager* mgr = this->app->activity->assetManager;
	AAssetDir* assetdir = AAssetManager_openDir(mgr, "");
	while (const char* ptr = AAssetDir_getNextFileName(assetdir))
	{
		std::cout << ptr << std::endl;
	}
	AAsset* asset = AAssetManager_open(mgr, file_name.c_str(), AASSET_MODE_UNKNOWN);
	//AAsset* asset = AAssetManager_open(mgr, "test.png", AASSET_MODE_UNKNOWN);
	FILE* fp = funopen(asset, android_read, android_write, android_seek, android_close);
	if (!fp)
		return -1;

	//texture test
	int x, y, n;
	//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
	unsigned char *data = stbi_load_from_file(fp, &x, &y, &n, 4);

	//set transparency
	if (colorkey != 0x00000000)
	{
		int r_ref = (colorkey & 0x00ff0000) >> 16;
		int g_ref = (colorkey & 0x0000ff00) >> 8;
		int b_ref = (colorkey & 0x000000ff) >> 0;
		for (int idx = 0; idx < x * y; idx++)
		{
			int b = data[idx * 4];
			int g = data[idx * 4 + 1];
			int r = data[idx * 4 + 2];
			unsigned char& a = data[idx * 4 + 3];
			if (r_ref == r && g_ref == g && b_ref == b)
			{
				a = 0;
			}
		}
	}

	GLuint handle = -1;
	glGenTextures(1, &handle);
	glBindTexture(TEXTURE_TYPE, handle);
	glActiveTexture(GL_TEXTURE0);
	glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(TEXTURE_TYPE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(TEXTURE_TYPE, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	return handle;
}

void GLGraphic::draw2DRectangle(GLuint tex, float x, float y, float z,
	float w, float h, float texx, float texy,
	float texw, float texh, float magx, float magy)
{
	glEnable(TEXTURE_TYPE);
	GLfloat mat_e[] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	GLfloat vVertices[] = {
		x, y - h, 0.0f, 1.0f,
		x + w, y - h, 0.0f, 1.0f,
		x, y , 0.0f, 1.0f,
		x + w, y, 0.0f, 1.0f,
	};
	GLfloat vTexCoords[] = {
		texx, texy + texh,
		texx + texw, texy + texh,
		texx, texy,
		texx + texw, texy,
	};

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	//(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glUniformMatrix4fv(rmat, 1, GL_FALSE, mat_e);
	glUniformMatrix4fv(tmat, 1, GL_FALSE, mat_e);
	glUniformMatrix4fv(mmat, 1, GL_FALSE, mat_e);
	glUniform1i(utexture, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vVertices);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, vTexCoords);
	//glUniformMatrix4fv(engine->umat, 1, GL_FALSE, mat);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glDrawArrays(GL_TRIANGLES, 0, 4);
	glDisable(TEXTURE_TYPE);

}

void GLGraphic::draw2DRectangleRotate(GLuint tex, float angle, float x, float y, float z,
	float w, float h, float texx, float texy,
	float texw, float texh, float magx, float magy)
{
	GLfloat mat_r[] = {
		cosf(angle), -sinf(angle), 0.0f, 0.0f,
		sinf(angle), cosf(angle), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	GLfloat mat_t[] = {
		1.0,    0.0f, 0.0f, 0.0f,
		0.0f, 1.0,    0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x + w / 2  , y - h / 2    , z   , 1.0f,
	};
	GLfloat mat_m[] = {
		w * magx,    0.0f, 0.0f, 0.0f,
		0.0f, h * magy,    0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	GLfloat vVertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, 0.5f , 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f,
	};
	GLfloat vTexCoords[] = {
		texx, texy + texh,
		texx + texw, texy + texh,
		texx, texy,
		texx + texw, texy,
	};

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(TEXTURE_TYPE, tex);
	//(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glUniformMatrix4fv(rmat, 1, GL_FALSE, mat_r);
	glUniformMatrix4fv(tmat, 1, GL_FALSE, mat_t);
	glUniformMatrix4fv(mmat, 1, GL_FALSE, mat_m);
	glUniform1i(utexture, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vVertices);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, vTexCoords);
	//glUniformMatrix4fv(engine->umat, 1, GL_FALSE, mat);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	//glDrawArrays(GL_TRIANGLES, 0, 4);

}

void GLGraphic::drawString(const std::string& str, float x, float y) {}