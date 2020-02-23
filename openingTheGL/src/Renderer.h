#pragma once

#include<GL/glew.h>

//A macro for assertion, to add a breakpoint when error is thrown
#define ASSERT(x) if (!(x))  __debugbreak();

//__INTELLISENSE__ forces intelliSense to not to consider the lines within,
//This is necessary because the intelliSense messes up when used with the glCall preprocessor macro
//
//More from Microsoft docs:
//the compiler macro __INTELLISENSE__, which is only defined when using the IntelliSense compiler. You can use this macro to guard code the IntelliSense compiler does not understand, or use it to toggle between the build and IntelliSense compiler.
//https://devblogs.microsoft.com/cppblog/troubleshooting-tips-for-intellisense-slowness/
#ifndef __INTELLISENSE__

#ifdef DEBUG

//A macro that adds a before and after functions for function x, I.e in our case, call Clear error before x and then calls GllogCall after x
//in GLLogCall(#x, __FILE__, __LINE__):
//"#x" is name/string of the function x, __File__ gives the file name current function is running and __line_ gives current line of execution
//"\" specifies the compiler to ignore the new line character
//
//This GLCall() macro will be added to every single opengl call we are doing in this function so that every time there is an error,in the call, we can get a stack trace and a breakpoint, Helps in debugging
#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#else
#define GLCall(x) x

#endif // !DEBUG

#else
#define GLCall(x) x

#endif // !_INTELLISENSE_

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
