#include <algorithm>
#include <vector>
#include <assert.h>

#include "scopemm-rastercanvas.h"

using namespace scopemm;

void RawRGB::scale(const RawRGB &in, size_t new_w, size_t new_h) {
	resize(new_w, new_h);
	if(!w || !h) return;
	assert(in.w && in.h);
	for(size_t y=0; y<h; y++) {
		const size_t j = (h==1) ? 0 : y*(in.h-1)/(h-1);
		for(size_t x=0; x<w; x++) {
			const size_t i = (w==1) ? 0 : x*(in.w-1)/(w-1);
			for(size_t band=0; band<3; band++) {
				(*this)(x, y, band) = in(i, j, band);
			}
		}
	}
}

RasterCanvas::GridCanvas() { }

RasterCanvas::~GridCanvas() { }

void RasterCanvas::setSwapAxes(bool state) {
	// FIXME - should transpose data
	swap_axes = state;
	fireChangeEvent();
}

void RasterCanvas::setXRange(double min, double max) {
	xmin = min;
	xmax = max;
}

void RasterCanvas::setYRange(double min, double max) {
	ymin = min;
	ymax = max;
}

void RasterCanvas::fireChangeEvent() {
	queue_draw();
}

bool RasterCanvas::on_expose_event(GdkEventExpose* event __attribute__((unused)) ) {
	Glib::RefPtr<Gdk::Window> window = get_window();

	if(!window) return true;

	screen_w = get_allocation().get_width();
	screen_h = get_allocation().get_height();

	if(data_buf.w && data_buf.h) {
		Gtk::Allocation allocation = get_allocation();
		const size_t w = allocation.get_width();
		const size_t h = allocation.get_height();

		// FIXME - only needed if sizes don't match
		draw_buf.scale(data_buf, w, h);

		get_window()->draw_rgb_image(
			get_style()->get_fg_gc(Gtk::STATE_NORMAL),
			0, 0, w, h,
			Gdk::RGB_DITHER_NONE,
			&draw_buf.data[0], w*3
		);
	}

	return true;
}
