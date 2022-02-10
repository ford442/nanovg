//
// Copyright (c) 2013 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#include <stdio.h>

#define GLFW_INCLUDE_ES3
#define GLFW_INCLUDE_GLEXT

#include <emscripten.h>
#include <emscripten/html5.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "nanovg.h"
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"
#include "demo.h"
#include "perf.h"


void errorcb(int error, const char* desc)
{
//	printf("GLFW error %d: %s\n", error, desc);
}

int blowup = 0;
int screenshot = 0;
int premult = 0;

/*
static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	NVG_NOTUSED(scancode);
	NVG_NOTUSED(mods);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		blowup = !blowup;
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		screenshot = 1;
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		premult = !premult;
}
*/

EGLDisplay display;
EGLSurface surface;
EGLContext contextegl;
EGLint config_size,major,minor;
EGLConfig eglconfig=NULL;
EmscriptenWebGLContextAttributes attr;
EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx;
EGLint v0=0,v1=1,v2=2,v3=3,v4=4,v6=6,v8=8,v32=32,a,b;

void renderFrame(){
eglSwapBuffers(display,surface);
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
renderDemo(vg, mx,my, winWidth,winHeight, t, blowup, &data);
renderGraph(vg, 5,5, &fps);
nvgEndFrame(vg);
}



int main()
{
//	GLFWwindow* window;
	DemoData data;
	NVGcontext* vg = NULL;
	PerfGraph fps;
	double prevt = 0;

//	if (!glfwInit()) {
//		printf("Failed to init GLFW.");
//		return -1;
//	}

	initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");

//	glfwSetErrorCallback(errorcb);
//	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//	window = glfwCreateWindow(1000, 600, "NanoVG", NULL, NULL);
//	window = glfwCreateWindow(1000, 600, "NanoVG", glfwGetPrimaryMonitor(), NULL);
//	if (!window) {
//		glfwTerminate();
//		return -1;
//	}
//	glfwSetKeyCallback(window, key);
//	glfwMakeContextCurrent(window);
	
eglBindAPI(EGL_OPENGL_ES_API);
S=EM_ASM_INT({return parseInt(document.getElementById('pmhig').innerHTML,10);});
emscripten_webgl_init_context_attributes(&attr);
attr.alpha=EM_TRUE;
attr.stencil=EM_TRUE;
attr.depth=EM_TRUE;
attr.antialias=EM_FALSE;
attr.premultipliedAlpha=EM_FALSE;
attr.preserveDrawingBuffer=EM_FALSE;
attr.enableExtensionsByDefault=EM_FALSE;
attr.renderViaOffscreenBackBuffer=EM_FALSE;
attr.powerPreference=EM_WEBGL_POWER_PREFERENCE_DEFAULT;
attr.failIfMajorPerformanceCaveat=EM_FALSE;
attr.majorVersion=2;
attr.minorVersion=0;
ctx=emscripten_webgl_create_context("#canvas",&attr);
display=eglGetDisplay(EGL_DEFAULT_DISPLAY);
eglInitialize(display,&v3,&v0);
eglChooseConfig(display,attribute_list,&eglconfig,1,&config_size);
contextegl=eglCreateContext(display,eglconfig,EGL_NO_CONTEXT,anEglCtxAttribs2);
surface=eglCreateWindowSurface(display,eglconfig,NULL,attribut_list);
eglMakeCurrent(display,surface,surface,contextegl);
emscripten_webgl_make_context_current(ctx);
	
	
	vg = nvgCreateGLES3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	if (vg == NULL) {
		printf("Could not init nanovg.\n");
		return -1;
	}

	if (loadDemoData(vg, &data) == -1)
		return -1;

//	glfwSwapInterval(0);

//	glfwSetTime(0);
//	prevt = glfwGetTime();

//	while (!glfwWindowShouldClose(window))
//	{
		double mx, my, t, dt;
		int winWidth, winHeight;
		int fbWidth, fbHeight;
		float pxRatio;

	//	t = glfwGetTime();
		dt = t - prevt;
		prevt = t;
		updateGraph(&fps, dt);
	//	glfwGetCursorPos(window, &mx, &my);
	//	glfwGetWindowSize(window, &winWidth, &winHeight);
	//	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		// Calculate pixel ration for hi-dpi devices.
		pxRatio = (float)fbWidth / (float)winWidth;
		glViewport(0, 0, fbWidth, fbHeight);
		glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		
		emscripten_set_main_loop((void(*)())renderFrame,0,0);
		glEnable(GL_DEPTH_TEST);

	//	if (screenshot) {
	//		screenshot = 0;
	//		saveScreenShot(fbWidth, fbHeight, premult, "dump.png");
	//	}
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}

	freeDemoData(vg, &data);
	nvgDeleteGLES3(vg);
//	glfwTerminate();
	return 0;
}
