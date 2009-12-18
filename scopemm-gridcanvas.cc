#include <algorithm>
#include <vector>

#include "scopemm-gridcanvas.h"

using namespace scopemm;

void RawRGB::scale(const RawRGB &in, size_t new_w, size_t new_h) {
	assert(in.w && in.h);
	resize(new_w, new_h);
	for(size_t y=0; y<h; y++) {
		const size_t j = y * (in.h-1) / (h-1);
		for(size_t x=0; x<w; x++) {
			const size_t i = x * (in.w-1) / (w-1);
			for(size_t band=0; band<3; band++) {
				(*this)(x, y, band) = in(i, j, band);
			}
		}
	}
}

GridCanvas::GridCanvas() { }

GridCanvas::~GridCanvas() { }

void GridCanvas::setSwapAxes(bool state) {
	swap_axes = state;
	fireChangeEvent();
}

void GridCanvas::fireChangeEvent() {
	queue_draw();
}

void GridCanvas::setData(blitz::Array<double, 2> data) {
	setData(data, data, data);
}

void GridCanvas::setData(
	blitz::Array<double, 2> data_r,
	blitz::Array<double, 2> data_g,
	blitz::Array<double, 2> data_b
) {
	blitz::Array<double, 2> data[3] = { data_r, data_g, data_b };
	const size_t w = data[0].shape()[swap_axes ? 0 : 1];
	const size_t h = data[0].shape()[swap_axes ? 1 : 0];

	// FIXME
	xmin = 0;
	xmax = 0;
	ymin = h-1;
	ymax = w-1;

	data_buf.resize(w, h);

	for(size_t band=0; band<3; band++) {
		const double min = blitz::min(data[band]);
		const double max = blitz::max(data[band]);
		if(swap_axes) {
			for(size_t y=0; y<h; y++) {
				for(size_t x=0; x<w; x++) {
					double v = data[band](int(x), int(y));
					data_buf(x, y, band) = uint8_t(255.0 * (v-min) / (max-min));
				}
			}
		} else {
			for(size_t y=0; y<h; y++) {
				for(size_t x=0; x<w; x++) {
					double v = data[band](int(y), int(x));
					data_buf(x, y, band) = uint8_t(255.0 * (v-min) / (max-min));
				}
			}
		}
	}

	fireChangeEvent();
}

bool GridCanvas::on_expose_event(GdkEventExpose* event __attribute__((unused)) ) {
	Glib::RefPtr<Gdk::Window> window = get_window();
	if(window && data_buf.w && data_buf.h) {
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
