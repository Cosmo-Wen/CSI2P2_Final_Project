#ifndef UTIL_H
#define UTIL_H

#include "Main.h"

// naive static logger class implementation
class Log
{
public:
	template<typename... TArgs>
	static void Info(TArgs&&... args)
	{
		fprintf(LOGFILE, "INFO: ");
		fprintf(LOGFILE, std::forward<TArgs>(args)...);
		fputc('\n', LOGFILE);
	}
	template<typename... TArgs>
	static void Warn(TArgs&&... args)
	{
		fprintf(LOGFILE, "WARN: ");
		fprintf(LOGFILE, std::forward<TArgs>(args)...);
		fputc('\n', LOGFILE);
	}
	template<typename... TArgs>
	static void Fatal(TArgs&&... args)
	{
		fprintf(LOGFILE, "FATAL: ");
		fprintf(LOGFILE, std::forward<TArgs>(args)...);
		fputc('\n', LOGFILE);
		// can not recover from fatal event, abort program
		exit(EXIT_FAILURE);
	}
};

struct Vec2
{
  Vec2(): x(0), y(0), w(x), h(y) {}
  Vec2(double x_, double y_)
    : x(x_), y(y_), w(x), h(y) {}
  double x, y;
  double &w, &h;
};

struct Vec4
{
	Vec4(): x(0), y(0), w(0), h(0) {}
  Vec4(double x, double y, double w, double h)
    : x(x), y(y), w(w), h(h) {}
  double x, y, w, h;
};

struct Color
{
  Color(): r(255), g(255), b(255) {}
  Color(double r, double g, double b)
    : r(r), g(g), b(b) {}
  double r, g, b;
};

struct ColorA
{
  ColorA(): r(255), g(255), b(255), a(255) {}
  ColorA(double r, double g, double b, double a)
    : r(r), g(g), b(b), a(a){}
  double r, g, b, a;
};

bool InRect(Vec2 pos, Vec4 rect);

#endif
