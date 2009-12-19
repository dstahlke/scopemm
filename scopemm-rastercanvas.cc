#include <algorithm>
#include <vector>
#include <assert.h>

#include "scopemm-rastercanvas.h"

using namespace scopemm;

void RawRGB::scale(const RawRGB &in, size_t new_w, size_t new_h, bool transpose) {
	resize(new_w, new_h);
	if(!w || !h) return;
	assert(in.w && in.h);
	if(transpose) {
		for(size_t y=0; y<h; y++) {
			const size_t i = (h==1) ? 0 : y*(in.w-1)/(h-1);
			for(size_t x=0; x<w; x++) {
				const size_t j = (w==1) ? 0 : x*(in.h-1)/(w-1);
				for(size_t band=0; band<3; band++) {
					(*this)(x, y, band) = in(i, j, band);
				}
			}
		}
	} else {
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
}

RasterArea::RasterArea() :
	impl(new RasterAreaImpl())
{
	impl_base = PlotLayerImplPtr(impl);
}

void RasterArea::setSwapAxes(bool state) {
	impl->swap_axes = state;
	fireChangeEvent();
}

void RasterArea::setXRange(double min, double max) {
	impl->xmin = min;
	impl->xmax = max;
	fireChangeEvent();
}

void RasterArea::setYRange(double min, double max) {
	impl->ymin = min;
	impl->ymax = max;
	fireChangeEvent();
}

void RasterAreaImpl::draw(Plot1D *parent, Cairo::RefPtr<Cairo::Context> cr) {
	Glib::RefPtr<Gdk::Window> window = parent->get_window();

	if(window && data_buf.w && data_buf.h) {
		const size_t w = parent->get_allocation().get_width();
		const size_t h = parent->get_allocation().get_height();

		// FIXME - it is necessary to consider our range in relation to the plot range
		draw_buf.scale(data_buf, w, h, swap_axes);

		window->draw_rgb_image(
			parent->get_style()->get_fg_gc(Gtk::STATE_NORMAL),
			0, 0, w, h,
			Gdk::RGB_DITHER_NONE,
			&draw_buf.data[0], w*3
		);
	}
}
