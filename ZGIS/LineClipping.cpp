#include "LineClipping.h"

#pragma region cohen_sutherland_clipping

#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8
double XL, XR, YB, YT;

int encode(double x, double y)
{
	int c = 0;
	if (x < XL) c |= LEFT;
	if (x > XR) c |= RIGHT;
	if (y < YB) c |= BOTTOM;
	if (y > YT) c |= TOP;
	return c;
}

bool cohen_sutherland_clipping(double &x1, double &y1, double &x2, double &y2,
	double xmin, double ymin, double xmax, double ymax)
{

	double x, y;
	XL = xmin, XR = xmax, YB = ymin, YT = ymax;
	int code1, code2, code;
	code1 = encode(x1, y1);
	code2 = encode(x2, y2);

	while (code1 != 0 || code2 != 0)
	{
		if (code1 & code2)
			return false;
		if (code1 != 0)
			code = code1;
		else
			code = code2;

		if (LEFT & code)
		{
			x = XL;
			y = y1 + (y2 - y1)*(XL - x1) / (x2 - x1);
		}
		else if (RIGHT & code)
		{
			x = XR;
			y = y1 + (y2 - y1)*(XR - x1) / (x2 - x1);
		}
		else if (BOTTOM & code)
		{
			y = YB;
			x = x1 + (x2 - x1)*(YB - y1) / (y2 - y1);
		}
		else if (TOP & code)
		{
			y = YT;
			x = x1 + (x2 - x1)*(YT - y1) / (y2 - y1);
		}
		if (code == code1)
		{
			x1 = x; y1 = y; code1 = encode(x1, y1);
		}
		else
		{
			x2 = x; y2 = y; code2 = encode(x2, y2);
		}
	}
	return true;
}

#pragma endregion

#pragma region liang_barsky_clipping

bool ClipT(double p, double q, double &u1, double &u2)
{
	double r;
	if (p < 0.0) {
		r = q / p;
		if (r > u2) return false;
		else if (r > u1) {
			u1 = r;
			return true;
		}
	}
	else if (p > 0.0) {
		r = q / p;
		if (r < u1) return false;
		else if (r < u2) {
			u2 = r;
			return true;
		}
	}
	else if (q < 0.0)
		return false;
	return true;
}


bool liang_barsky_clipping(double &x1, double &y1, double &x2, double &y2,
	double XL, double YB, double XR, double YT)
{
	double dx, dy, u1, u2;
	u1 = 0.0; u2 = 1.0;

	dx = x2 - x1;
	dy = y2 - y1;
	if (ClipT(-dx, x1 - XL, u1, u2))
		if (ClipT(dx, XR - x1, u1, u2))
			if (ClipT(-dy, y1 - YB, u1, u2))
				if (ClipT(dy, YT - y1, u1, u2))
				{
					double temp = x1;
					x1 = x1 + u1*dx;
					x2 = temp + u2*dx;
					temp = y1;
					y1 = y1 + u1*dy;
					y2 = temp + u2*dy;
					return true;
				}
	return false;
}


#pragma endregion

bool line_clipping(double &x0, double &y0, double &x1, double &y1,
	double xmin, double ymin, double xmax, double ymax)
{
	return cohen_sutherland_clipping(x0, y0, x1, y1, xmin, ymin, xmax, ymax);
	//return liang_barsky_clipping(x0, y0, x1, y1, xmin, ymin, xmax, ymax);
}
