#ifndef SCOPEMM_GRIDCANVAS_H
#define SCOPEMM_GRIDCANVAS_H

#include <vector>

#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>
#include <blitz/array.h>

namespace scopemm {

class GridCanvas : public Gtk::DrawingArea {
public:
	GridCanvas();

	virtual ~GridCanvas();

	void setFlipAxes(bool state=true);

	void setData(blitz::Array<double, 2> data);

	void setData(
		blitz::Array<double, 2> data_r,
		blitz::Array<double, 2> data_g,
		blitz::Array<double, 2> data_b
	);

	void screenToGrid(double sx, double sy, double *tx, double *ty);

	void gridToScreen(double tx, double ty, double *sx, double *sy);

protected:
	virtual bool on_expose_event(GdkEventExpose* event);

	std::vector<guchar> buf;
	int data_h, data_w;
	bool flip_axes;
};

} // namespace scopemm

#endif // SCOPEMM_GRIDCANVAS_H
