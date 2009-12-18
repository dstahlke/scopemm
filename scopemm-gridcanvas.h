#ifndef SCOPEMM_GRIDCANVAS_H
#define SCOPEMM_GRIDCANVAS_H

#include <vector>

#include <stdint.h>

#include <gtkmm/drawingarea.h>
#include <gtkmm/window.h>

#ifdef SCOPEMM_ENABLE_BLITZ
#include <blitz/array.h>
#endif // SCOPEMM_ENABLE_BLITZ

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

	void setXRange(double min, double max);
	void setYRange(double min, double max);
	void setSwapAxes(bool state=true);

	void fireChangeEvent();

#ifdef SCOPEMM_ENABLE_BLITZ
	template <class T>
	void setData(blitz::Array<T, 2> data);

	template <class T>
	void setData(
		blitz::Array<T, 2> data_r,
		blitz::Array<T, 2> data_g,
		blitz::Array<T, 2> data_b
	);
#endif // SCOPEMM_ENABLE_BLITZ

	void screenToGrid(double sx, double sy, double *tx, double *ty);

	void gridToScreen(double tx, double ty, double *sx, double *sy);

	RawRGB data_buf;

protected:
	virtual bool on_expose_event(GdkEventExpose* event);

	RawRGB draw_buf;
};

#ifdef SCOPEMM_ENABLE_BLITZ
template <class T>
void GridCanvas::setData(blitz::Array<T, 2> data) {
	setData(data, data, data);
}

template <class T>
void GridCanvas::setData(
	blitz::Array<T, 2> data_r,
	blitz::Array<T, 2> data_g,
	blitz::Array<T, 2> data_b
) {
	blitz::Array<T, 2> data[3] = { data_r, data_g, data_b };
	const size_t w = data[0].shape()[swap_axes ? 1 : 0];
	const size_t h = data[0].shape()[swap_axes ? 0 : 1];

	data_buf.resize(w, h);

	for(size_t band=0; band<3; band++) {
		const T min = blitz::min(data[band]);
		const T max = blitz::max(data[band]);
		if(swap_axes) {
			for(size_t y=0; y<h; y++) {
				for(size_t x=0; x<w; x++) {
					T v = data[band](int(y), int(x));
					data_buf(x, y, band) = uint8_t(255.0 * (v-min) / (max-min));
				}
			}
		} else {
			for(size_t y=0; y<h; y++) {
				for(size_t x=0; x<w; x++) {
					T v = data[band](int(x), int(y));
					data_buf(x, y, band) = uint8_t(255.0 * (v-min) / (max-min));
				}
			}
		}
	}

	fireChangeEvent();
}
#endif // SCOPEMM_ENABLE_BLITZ

} // namespace scopemm

#endif // SCOPEMM_GRIDCANVAS_H
