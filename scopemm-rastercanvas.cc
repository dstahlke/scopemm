#include <algorithm>
#include <vector>
#include <assert.h>

#include "scopemm.h"
#include "scopemm-layerimpl.h"

namespace scopemm {

class RasterAreaImpl : public PlotLayerImplBase {
public:
	RasterAreaImpl() :
		bbox(0, 1, 0, 1)
	{ }

	virtual void draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context>);
	virtual bool hasMinMax() const { return true; }
	virtual Bbox getBbox() const { return bbox; }

	AffineTransform getAffine();

	Bbox bbox;
	AffineTransform affine;
	bool swap_axes;
	RawRGB data_buf;
	RawRGB draw_buf;
};

void RawRGB::transform(const RawRGB &in, HalfAffine affine) {
	for(size_t out_y=0; out_y<h; out_y++) {
		for(size_t out_x=0; out_x<w; out_x++) {
			double in_x, in_y;
			affine(out_x, out_y, in_x, in_y);
			int i = int(round(in_x));
			int j = int(round(in_y));
			if(i>=0 && j>=0 && size_t(i)<in.w && size_t(j)<in.h) {
				for(size_t band=0; band<3; band++) {
					(*this)(out_x, out_y, band) = in(i, j, band);
				}
			} else {
				// FIXME - make transparent
				for(size_t band=0; band<3; band++) {
					(*this)(out_x, out_y, band) = 0;
				}
			}
		}
	}
}

RasterArea::RasterArea() {
	setZOrder(ZORDER_RASTER_AREA);
}

void RasterArea::setSwapAxes(bool state) {
	impl->swap_axes = state;
	fireChangeEvent();
}

void RasterArea::setBbox(Bbox bbox) {
	impl->bbox = bbox;
	fireChangeEvent();
}

void RasterAreaImpl::draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context> cr) {
	Glib::RefPtr<Gdk::Window> window = parent->get_window();

	if(window && data_buf.w && data_buf.h) {
		const size_t w = parent->get_allocation().get_width();
		const size_t h = parent->get_allocation().get_height();

		draw_buf.resize(w, h);
		draw_buf.transform(
			data_buf,
			(getAffine().inv * parent->getAffine().inv)
		);

		window->draw_rgb_image(
			parent->get_style()->get_fg_gc(Gtk::STATE_NORMAL),
			0, 0, w, h,
			Gdk::RGB_DITHER_NONE,
			&draw_buf.data[0], w*3
		);
	}
}

AffineTransform RasterAreaImpl::getAffine() {
	return AffineTransform::boxToBox(
		Bbox(0, data_buf.w-1, data_buf.h-1, 0), bbox, swap_axes);
}

RawRGB &RasterArea::getDataBuf() { return impl->data_buf; }

AffineTransform RasterArea::getAffine() { return impl->getAffine(); }

} // namespace scopemm
