#ifndef SCOPEMM_GRIDCANVAS_H
#define SCOPEMM_GRIDCANVAS_H

#include <vector>

#include <stdint.h>

#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>
#include <blitz/array.h>

#include "scopemm-base.h"

namespace scopemm {

class RawRGB {
public:
	RawRGB() : w(0), h(0) { }

	void resize(size_t _w, size_t _h) {
		w = _w;
		h = _h;
		data.resize(w*h*3);
	}

	uint8_t &operator()(size_t x, size_t y, size_t band) {
		return data[(y*w+x)*3+band];
	}

	const uint8_t &operator()(size_t x, size_t y, size_t band) const {
		return data[(y*w+x)*3+band];
	}

	void scale(const RawRGB &in, size_t new_w, size_t new_h);

	size_t w;
	size_t h;
	std::vector<uint8_t> data;
};

class GridCanvas : public PlotBase {
public:
	GridCanvas();

	~GridCanvas();

	void setSwapAxes(bool state=true);

	void fireChangeEvent();

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

	RawRGB data_buf;
	RawRGB draw_buf;
};

} // namespace scopemm

#endif // SCOPEMM_GRIDCANVAS_H
