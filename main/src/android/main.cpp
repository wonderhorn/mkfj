/*
* Copyright (C) 2010 The Android Open Source Project
*
* Apache License Version 2.0 (�u�{���C�Z���X�v) �Ɋ�Â��ă��C�Z���X����܂��B;
* �{���C�Z���X�ɏ������Ȃ��ꍇ�͂��̃t�@�C�����g�p�ł��܂���B
* �{���C�Z���X�̃R�s�[�́A�ȉ��̏ꏊ�������ł��܂��B
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* �K�p�����@�߂܂��͏��ʂł̓��ӂɂ���Ė������Ȃ�����A�{���C�Z���X�Ɋ�Â��ĔЕz�����\�t�g�E�F�A�́A
* �����َ����킸�A�����Ȃ�ۏ؂��������Ȃ��Ɍ���̂܂�
* �Еz����܂��B
* �{���C�Z���X�ł̌����Ɛ������K�肵���������ẮA
* �{���C�Z���X���Q�Ƃ��Ă��������B
*
*/

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))

#include<assert.h>
#include<GLES2/gl2.h>
#include"android/GLGraphic.h"

#include"jumpnrun/system/Parmanent.h"
#include"framework/Game.h"

gfw::Game* game = nullptr;

/**
* �ۑ���Ԃ̃f�[�^�ł��B
*/
struct saved_state {
	float angle;
	int32_t x;
	int32_t y;
};

/**
* �A�v���̕ۑ���Ԃł��B
*/
struct engine {
	struct android_app* app;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	int animating;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
	struct saved_state state;

	GLuint pobj, attr_uv, unif_texture, u_rmat, u_tmat, u_mmat;
	GLGraphic* glgraphics;

	int z_x, z_y, x_x, x_y;
	int up_x, up_y, right_x, right_y, down_x, down_y, left_x, left_y;
};


static GLuint loadShader(GLenum type, const char *shaderSrc) {

	GLuint shader;

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSrc, NULL);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled != 1)
	{
		GLchar log[10240];
		GLint l;
		glGetShaderInfoLog(shader, 10240, &l, log);
		std::cout << log << std::endl;
	}
	assert(compiled == GL_TRUE);
	return shader;
}

void initParmanentsForPlay()
{
	$P.reset();
	$P.players.push_back(jnr::PlayerData(0, std::string("player1"), &$P.keycont, jnr::Deck(), true));
	$P.players.push_back(jnr::PlayerData(0, std::string("boss1"), &$P.ai.Controller(), jnr::Deck(), true));
	$P.save_file_name = "/sdcard/savedata.txt";
	$P.load($P.save_file_name);
	unsigned int t = time(nullptr);
	std::string str_t = "00000";
	$P.start_time = str_t;
	//CreateDirectory((std::string("replay\\") + str_t).c_str(), NULL);
	$P.save((std::string("replay\\") + str_t + "\\" + "savedata.txt").c_str());

	$P.mutex_rand.lock();
	//$P.rand.reset(myutil::str2int($P.start_time));
	$P.mutex_rand.unlock();
	$P.to_save = true;
}


/**
* ���݂̕\���� EGL �R���e�L�X�g�����������܂��B
*/
static int engine_init_display(struct engine* engine) {
	// OpenGL ES �� EGL �̏�����

	/*
	* �ړI�̍\���̑����������Ŏw�肵�܂��B
	* �ȉ��ŁA�I���X�N���[�� �E�B���h�E��
	* �݊����̂���A�e�F�Œ� 8 �r�b�g�̃R���|�[�l���g�� EGLConfig ��I�����܂�
	*/
	const EGLint attribs[] =
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};
	const EGLint context_attribs[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	EGLint w, h, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	EGLint majorv, minorv;
	eglInitialize(display, &majorv, &minorv);

	/* �����ŁA�A�v���P�[�V�����͖ړI�̍\����I�����܂��B���̃T���v���ł́A
	* ���o�����ƈ�v����ŏ��� EGLConfig ��
	* �I������P���ȑI���v���Z�X������܂� */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID �́AANativeWindow_setBuffersGeometry() ��
	* ����Ď󂯎���邱�Ƃ��ۏ؂���Ă��� EGLConfig �̑����ł��B
	* EGLConfig ��I�������炷���ɁAANativeWindow �o�b�t�@�[����v�����邽�߂�
	* EGL_NATIVE_VISUAL_ID ���g�p���Ĉ��S�ɍč\���ł��܂��B*/
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attribs);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}

	//eglBindAPI(EGL_OPENGL_ES_API);

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;
	engine->state.angle = 0;

	// GL �̏�Ԃ����������܂��B
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	//glEnable(GL_CULL_FACE);
	//glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);

	//shader
	GLbyte vsstr[] =
		"attribute mediump vec4 vp;\n"
		"attribute mediump vec2 auv;\n"
		"varying vec2 vuv;\n"
		"uniform mat4 mmat;\n"
		"uniform mat4 rmat;\n"
		"uniform mat4 tmat;\n"
		"void main(){ \n"
		//"  gl_Position = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0) * vp;\n"
		//"  gl_Position = mat4(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0) * vp;\n"
		//"  gl_Position = mmat * rmat * tmat * vp;\n"
		"  gl_Position = rmat * mmat * tmat * vp;\n"
		//"  gl_Position = vp;\n"
		"  vuv = auv;\n"
		"} \n";

	GLbyte fsstr[] =
		"precision mediump float;\n"
		//"varying vec2 texcoordVarying;\n"
		"varying mediump vec2 vuv;"
		"uniform sampler2D unif_texture;\n"
		"void main() {\n"
		"vec4 c = texture2D(unif_texture, vuv);\n"
		"if (c.a > 0.05)"
		"  gl_FragColor = c;"
		"else discard;"
		//"gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
		"}\n";
	GLuint vshader, fshader;
	vshader = loadShader(GL_VERTEX_SHADER, (const char*)vsstr);
	fshader = loadShader(GL_FRAGMENT_SHADER, (const char*)fsstr);

	GLuint pobj = glCreateProgram();
	glAttachShader(pobj, vshader);
	glAttachShader(pobj, fshader);
	glBindAttribLocation(pobj, 0, "vp");
	glBindAttribLocation(pobj, 1, "auv");
	//engine->attr_uv = glGetAttribLocation(pobj, "texcoordVarying");
	glLinkProgram(pobj);
	engine->u_rmat = glGetUniformLocation(pobj, "rmat");
	engine->u_tmat = glGetUniformLocation(pobj, "tmat");
	engine->u_mmat = glGetUniformLocation(pobj, "mmat");
	engine->unif_texture = glGetUniformLocation(pobj, "unif_texture");
	engine->pobj = pobj;

	engine->glgraphics = new GLGraphic();
	$P.app = engine->app;
	bool res = engine->glgraphics->init(engine->app,
		engine->u_rmat, engine->u_tmat, engine->u_mmat, engine->unif_texture);

	//engine->unif_texture = glGetUniformLocation(pobj, "texture");
	assert(engine->unif_texture >= 0);
	assert(engine->u_rmat >= 0);
	assert(engine->u_tmat >= 0);
	assert(engine->u_mmat >= 0);

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA);
	glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.01);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initParmanentsForPlay();
	gfw::Media& media = $P.media;
	media.initialize(engine->glgraphics);
	$P.loadAllImages();

	game = new gfw::Game();
	game->initialize();


	return 0;
}


static void screen2viewport(int x, int y, int vx, int vy, int vw, int vh,
	float& out_x, float& out_y)
{
	x = x - vx;
	out_x = ((float)x / vw) * 2.0 - 1.0;
	y = y - vy;
	out_y = (((float)y / vh) * 2.0 - 1.0);

}
/**
* �f�B�X�v���C���̌��݂̃t���[���̂݁B
*/
static void engine_draw_frame(struct engine* engine) {
	if (engine->display == NULL) {
		// �f�B�X�v���C������܂���B
		return;
	}

	int vpsize = std::min(engine->width, engine->height);
	//int vpsize = 500;
	int vx = 0;
	int vy = engine->height - vpsize;
	float touch_x = 0.0f, touch_y = 0.0f;
	screen2viewport(engine->state.x, engine->height - engine->state.y,
		vx, vy, vpsize, vpsize, touch_x, touch_y);
	glViewport(vx, vy, vpsize, vpsize);


	// �F�ŉ�ʂ�h��Ԃ��܂��B
	//glClearColor(((float)engine->state.x) / engine->width, engine->state.angle,
	//	((float)engine->state.y) / engine->height, 1);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(engine->pobj);

	if (0)
	{
		GLfloat vVertices[] = {
			-0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, 0.5f , 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f, 1.0f,
		};
		/*GLfloat vVertices[] = {
		x, y - h, 0.0f, 1.0f,
		x + w, y - h, 0.0f, 1.0f,
		x, y , 0.0f, 1.0f,
		x + w, y, 0.0f, 1.0f,
		};*/
		GLfloat vTexCoords[] = {
			0, 0,
			1, 0,
			1, 1,
			0, 1,
		};


		//glEnable(GL_TEXTURE_2D);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(TEXTURE_TYPE, tex);
		//(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		//glUniformMatrix4fv(rmat, 1, GL_FALSE, mat_r);
		//glUniformMatrix4fv(tmat, 1, GL_FALSE, mat_t);
		//glUniformMatrix4fv(mmat, 1, GL_FALSE, mat_m);
		//glUniform1i(utexture, 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, vVertices);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, vTexCoords);
		//glUniformMatrix4fv(engine->umat, 1, GL_FALSE, mat);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


		GLGraphic* graphics = engine->glgraphics;
		graphics->draw2DRectangleRotate(graphics->test_texture,
			//3.14 / 2,
			engine->state.angle,
			touch_x, touch_y,
			0,
			//1.0, 1.0);
			1.0, 1.0 * 480 / 320);
		/*graphics->draw2DRectangle(graphics->test_texture,
			//3.14 / 2,
			0,
			0,
			0,
			1.0, 1.0);*/
	}
	auto g = $P.media.getGraphics();
	if (1) {
	//if (g->startRendering()) {

		if(game)
			game->render($P.media);

		g->endRendering();
	}

	//g->drawSpinResize($P.img_stand2, 320, 240, 0,
	//		0, 0, 320, 480, 0, 0xffffffff,
	//		1.0, 0.5, engine->state.angle);

	/*g->drawSpinResize($P.img_obj, 60, 0, 0,
				0, 0, 32, 64, 0, 0xffffffff,
				1.0, 1.0, 0);
	g->drawSpinResize($P.img_obj, 92, 64, 0,
		0, 0, 32, 64, 0, 0xffffffff,
		1.0, 1.0, 0);
	g->drawSpinResize($P.img_obj, 124, 128, 0,
		0, 0, 32, 64, 0, 0xffffffff,
		1.0, 1.0, 0);

	g->draw($P.img_obj, 260, 0, 0,
		0, 0, 32, 64, 0, 0xffffffff);
	g->draw($P.img_obj, 292, 64, 0,
		0, 0, 32, 64, 0, 0xffffffff);
	g->draw($P.img_obj, 324, 128, 0,
		0, 0, 32, 64, 0, 0xffffffff);*/

	//view port2
	int vx2 = 0;
	int vy2 = 0;

	vy2 = vy - engine->width;
	/*
	if (engine->width < engine->height)
	{
		vy2 = engine->height - vpsize - 300;
	}
	else {
		vx2 = vpsize;
	}*/

	//glClearColor(0, 0, 1, 0);

	glViewport(vx2, vy2, engine->width, engine->width);
	float bx = 0, by = 0;

	bx = 420;
	by = 20 + 60;
	engine->z_x = vx2 + bx * engine->width / 640;
	engine->z_y = vy - by * engine->width / 480 - 48;
	g->draw($P.img_obj,
		bx, by, 0,
		640, 1248, 32, 32);

	bx = 500;
	by = 30 + 60;
	engine->x_x = vx2 + bx * engine->width / 640;
	engine->x_y = vy - by * engine->width / 480 - 48;
	g->draw($P.img_obj,
		bx, by, 0,
		640 + 32, 1248, 32, 32);

	bx = 80;
	by = 10 + 60;
	engine->up_x = vx2 + bx * engine->width / 640;
	engine->up_y = vy - by * engine->width / 480 - 48;
	g->draw($P.img_obj,
		bx, by, 0,
		640 + 32 * 2, 1248, 32, 32);

	bx = 120;
	by = 50 + 60;
	engine->right_x = vx2 + bx * engine->width / 640;
	engine->right_y = vy - by * engine->width / 480 - 48;
	g->draw($P.img_obj,
		bx, by, 0,
		640 + 32 * 3, 1248, 32, 32);

	bx = 80;
	by = 90 + 60;
	engine->down_x = vx2 + bx * engine->width / 640;
	engine->down_y = vy - by * engine->width / 480 - 48;
	g->draw($P.img_obj,
		bx, by, 0,
		640 + 32 * 4, 1248, 32, 32);

	bx = 40;
	by = 50 + 60;
	engine->left_x = vx2 + bx * engine->width / 640;
	engine->left_y = vy - by * engine->width / 480 - 48;
	g->draw($P.img_obj,
		bx, by, 0,
		640 + 32 * 5, 1248, 32, 32);


	eglSwapBuffers(engine->display, engine->surface);
}

/**
* ���݃f�B�X�v���C�Ɋ֘A�t�����Ă��� EGL �R���e�L�X�g���폜���܂��B
*/
static void engine_term_display(struct engine* engine) {
	if (engine->display != EGL_NO_DISPLAY) {
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT) {
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE) {
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->animating = 0;
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}

/**
* ���̓��̓C�x���g���������܂��B
*/
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
	struct engine* engine = (struct engine*)app->userData;
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		auto act = AMotionEvent_getAction(event);
		if (AKEY_EVENT_ACTION_DOWN == act)
		{
			engine->state.x = AMotionEvent_getX(event, 0);
			engine->state.y = AMotionEvent_getY(event, 0);
			$P.media.input.setZRect(engine->z_x, engine->z_y, 48, 48);
			$P.media.input.setXRect(engine->x_x, engine->x_y, 48, 48);
			$P.media.input.setUpRect(engine->up_x, engine->up_y, 48, 48);
			$P.media.input.setRightRect(engine->right_x, engine->right_y, 48, 48);
			$P.media.input.setDownRect(engine->down_x, engine->down_y, 48, 48);
			$P.media.input.setLeftRect(engine->left_x, engine->left_y, 48, 48);
			$P.media.input.touch(engine->state.x, engine->height - engine->state.y);
		}
		else if (AKEY_EVENT_ACTION_UP == act)
		{
			engine->state.x = AMotionEvent_getX(event, 0);
			engine->state.y = AMotionEvent_getY(event, 0);
			$P.media.input.touch(-99999, -99999);
		}
		return 1;
	}
	return 0;
}

/**
* ���̃��C�� �R�}���h���������܂��B
*/
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
	struct engine* engine = (struct engine*)app->userData;
	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		// ���݂̏�Ԃ�ۑ�����悤�V�X�e���ɂ���ėv������܂����B�ۑ����Ă��������B
		engine->app->savedState = malloc(sizeof(struct saved_state));
		*((struct saved_state*)engine->app->savedState) = engine->state;
		engine->app->savedStateSize = sizeof(struct saved_state);
		break;
	case APP_CMD_INIT_WINDOW:
		// �E�B���h�E���\������Ă��܂��B�������Ă��������B
		if (engine->app->window != NULL) {
			engine_init_display(engine);
			engine_draw_frame(engine);
		}
		break;
	case APP_CMD_WINDOW_RESIZED:
		if (engine->display != NULL && engine->surface != NULL) {
			EGLint w, h;
			eglQuerySurface(engine->display, engine->surface, EGL_WIDTH, &w);
			eglQuerySurface(engine->display, engine->surface, EGL_HEIGHT, &h);
			engine->width = w;
			engine->height = h;
			$P.media.input.setZRect(engine->z_x, engine->z_y, 300, 300);
		}
		break;
	case APP_CMD_TERM_WINDOW:
		// �E�B���h�E����\���܂��͕��Ă��܂��B�N���[�� �A�b�v���Ă��������B
		engine_term_display(engine);
		break;
	case APP_CMD_GAINED_FOCUS:
		// �A�v�����t�H�[�J�X���擾����ƁA�����x�v�̊Ď����J�n���܂��B
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_enableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
			// �ڕW�� 1 �b���Ƃ� 60 �̃C�x���g���擾���邱�Ƃł� (�č�)�B
			ASensorEventQueue_setEventRate(engine->sensorEventQueue,
				engine->accelerometerSensor, (1000L / 60) * 1000);
		}
		break;
	case APP_CMD_LOST_FOCUS:
		// �A�v�����t�H�[�J�X�������ƁA�����x�v�̊Ď����~���܂��B
		// ����ɂ��A�g�p���Ă��Ȃ��Ƃ��̃o�b�e���[��ߖ�ł��܂��B
		if (engine->accelerometerSensor != NULL) {
			ASensorEventQueue_disableSensor(engine->sensorEventQueue,
				engine->accelerometerSensor);
		}
		// �܂��A�A�j���[�V�������~���܂��B
		engine->animating = 0;
		engine_draw_frame(engine);
		break;
	}
}

/**
* ����́Aandroid_native_app_glue ���g�p���Ă���l�C�e�B�u �A�v���P�[�V����
* �̃��C�� �G���g�� �|�C���g�ł��B���ꎩ�̂̃X���b�h�ŃC�x���g ���[�v�ɂ���Ď��s����A
* ���̓C�x���g���󂯎�����葼�̑�������s�����肵�܂��B
*/
void android_main(struct android_app* state) {
	struct engine engine;

	memset(&engine, 0, sizeof(engine));
	state->userData = &engine;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	engine.app = state;

	// �����x�v�̊Ď��̏���
	engine.sensorManager = ASensorManager_getInstance();
	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
		ASENSOR_TYPE_ACCELEROMETER);
	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
		state->looper, LOOPER_ID_USER, NULL, NULL);

	if (state->savedState != NULL) {
		// �ȑO�̕ۑ���ԂŊJ�n���܂��B�������Ă��������B
		engine.state = *(struct saved_state*)state->savedState;
	}

	engine.animating = 1;

	// ���[�v�̓X�^�b�t�ɂ��J�n��҂��Ă��܂��B

	while (1) {
		// �ۗ����̂��ׂẴC�x���g��ǂݎ��܂��B
		int ident;
		int events;
		struct android_poll_source* source;

		if (game)
		{
			$P.media.input.poll();
			game->run($P.media);
		}

		// �A�j���[�V�������Ȃ��ꍇ�A�������Ƀu���b�N���ăC�x���g����������̂�҂��܂��B
		// �A�j���[�V��������ꍇ�A���ׂẴC�x���g���ǂݎ����܂Ń��[�v���Ă��瑱�s���܂�
		// �A�j���[�V�����̎��̃t���[����`�悵�܂��B
		while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
			(void**)&source)) >= 0) {

			// ���̃C�x���g���������܂��B
			if (source != NULL) {
				source->process(state, source);
			}

			// �Z���T�[�Ƀf�[�^������ꍇ�A�������������܂��B
			if (ident == LOOPER_ID_USER) {
				if (engine.accelerometerSensor != NULL) {
					ASensorEvent event;
					while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
						&event, 1) > 0) {
						LOGI("accelerometer: x=%f y=%f z=%f",
							event.acceleration.x, event.acceleration.y,
							event.acceleration.z);
					}
				}
			}

			// �I�����邩�ǂ����m�F���܂��B
			if (state->destroyRequested != 0) {
				engine_term_display(&engine);
				return;
			}
		}

		if (engine.animating) {
			// �C�x���g�����������玟�̃A�j���[�V���� �t���[����`�悵�܂��B
			engine.state.angle += .1f;
			if (engine.state.angle > 3.14 * 2) {
				engine.state.angle = 0;
			}

			// �`��͉�ʂ̍X�V���[�g�ɍ��킹�Ē�������Ă��邽�߁A
			// �����Ŏ��Ԓ���������K�v�͂���܂���B
			engine_draw_frame(&engine);
		}
	}
}