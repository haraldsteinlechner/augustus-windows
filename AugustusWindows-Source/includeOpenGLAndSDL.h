#ifndef includedOpenGLAndSDL
#define includedOpenGLAndSDL

#define compileMac 0
#define compileLinux 1
#define compileWindows 2

#define compilePlatform compileWindows

#if compilePlatform==compileMac

	// Grafik-Dateien
	#include <glut/glut.h>
	#include <opengl/gl.h>
	#include <SDL/SDL.h>
	#include <SDL/SDL_Thread.h>
	#include "SDL_ttf.h"

	
#endif


#if compilePlatform==compileWindows

	#include <windows.h>

	#include <gl/glee.h>

	#include <gl/gl.h>
	#include <gl/glut.h>
	#include <SDL/SDL.h>
	#include "SDL_ttf.h"
	#include <SDL/SDL_thread.h>
	
	
#endif

#if compilePlatform==compileLinux

	#include <SDL/SDL.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
	#include <ft2build.h>
	#include FT_FREETYPE_H

	#include <SDL/SDL_ttf.h>

#endif

#endif