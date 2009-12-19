#ifndef SCOPEMM_BASE_H
#define SCOPEMM_BASE_H

#include <gtkmm/drawingarea.h>

namespace scopemm {

class PlotBase : public Gtk::DrawingArea { // FIXME - absorb this into PlotCanvas
public:
	PlotBase() :
		xmin(0), xmax(1), ymin(0), ymax(1), swap_axes(false)
	{ }

	double getXMin() const { return xmin; }
	double getXMax() const { return xmax; }
	double getYMin() const { return ymin; }
	double getYMax() const { return ymax; }
	bool getSwapAxes() const { return swap_axes; }

	inline void coordToScreen(double xi, double yi, double &xo, double &yo) const {
		double x = (xmax==xmin) ? 0.5 : (xi-xmin)/(xmax-xmin);
		double y = (ymax==ymin) ? 0.5 : (yi-ymin)/(ymax-ymin);
		if(swap_axes) std::swap(x, y);
		y = 1.0-y;
		xo = x * (screen_w-1);
		yo = y * (screen_h-1);
	}

	inline void screenToCoord(double xi, double yi, double &xo, double &yo) const {
		double x = (screen_w<=1) ? 0.5 : xi / (screen_w-1);
		double y = (screen_w<=1) ? 0.5 : yi / (screen_h-1);
		if(swap_axes) std::swap(x, y);
		y = 1.0-y;
		xo = x * (xmax-xmin) + xmin;
		yo = y * (ymax-ymin) + ymin;
	}

protected:
	int screen_w, screen_h;
	double xmin, xmax, ymin, ymax;
	bool swap_axes;
};

} // namespace scopemm

#endif // SCOPEMM_BASE_H
