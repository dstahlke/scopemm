#include <algorithm>
#include <vector>
#include <assert.h>

#include <algorithm>
#include <math.h>

#include "scopemm.h"
#include "scopemm-layerimpl.h"

namespace scopemm {

class RasterAreaImpl : public PlotLayerImplBase {
public:
	RasterAreaImpl() :
		bbox(0, 1, 0, 1),
		swap_axes(false),
		bilinear(false)
	{ }

	virtual void draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context>);
	virtual bool hasMinMax() const { return true; }
	virtual Bbox getBbox() const { return bbox; }

	AffineTransform getAffine();

	Bbox bbox;
	AffineTransform affine;
	bool swap_axes;
	bool bilinear;
	RawRGB data_buf;
	RawRGB draw_buf;
};

void RawRGB::transform(const RawRGB &in, HalfAffine affine, bool bilinear) {
	for(size_t out_y=0; out_y<h; out_y++) {
	for(size_t out_x=0; out_x<w; out_x++) {
		double in_x, in_y;
		affine(out_x, out_y, in_x, in_y);
		if(bilinear) {
			if(
				in_x>=0 && 
				in_y>=0 && 
				in_x<=double(in.w) && 
				in_y<=double(in.h)
			) {
				in_x -= 0.5;
				in_y -= 0.5;
				int i = int(floor(in_x));
				int j = int(floor(in_y));
				double dx = in_x-i;
				double dy = in_y-j;
				for(size_t band=0; band<3; band++) {
					int i0 = (i>=0) ? i : i+1;
					int i1 = (i+1<int(in.w)) ? i+1 : i;
					int j0 = (j>=0) ? j : j+1;
					int j1 = (j+1<int(in.h)) ? j+1 : j;
					double v0 = (1.0-dx)*in(i0, j0, band) + dx*in(i1, j0, band);
					double v1 = (1.0-dx)*in(i0, j1, band) + dx*in(i1, j1, band);
					double v  = (1.0-dy)*v0 + dy*v1;
					(*this)(out_x, out_y, band) = v;
				}
			} else {
				// FIXME - make transparent
				for(size_t band=0; band<3; band++) {
					(*this)(out_x, out_y, band) = 0;
				}
			}
		} else {
			int i = int(floor(in_x));
			int j = int(floor(in_y));
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
	} } // out_x, out_y
}

RasterArea::RasterArea() {
	setZOrder(ZORDER_RASTER_AREA);
}

void RasterArea::setSwapAxes(bool state) {
	impl->swap_axes = state;
	fireChangeEvent();
}

void RasterArea::setBilinear(bool state) {
	impl->bilinear = state;
	fireChangeEvent();
}

void RasterArea::setBbox(Bbox bbox) {
	impl->bbox = bbox;
	fireChangeEvent();
}

void RasterAreaImpl::draw(
	PlotCanvas *parent,
	Cairo::RefPtr<Cairo::Context> cr
) {
	Glib::RefPtr<Gdk::Window> window = parent->get_window();

	if(window && data_buf.w && data_buf.h) {
		const int w = parent->get_allocation().get_width();
		const int h = parent->get_allocation().get_height();

		HalfAffine screen_to_data = 
			getAffine().inv * parent->getAffine().inv;

		int clip_xmin, clip_xmax, clip_ymin, clip_ymax;
		{
			double x, y;
			parent->getAffine().fwd(bbox.xmin, bbox.ymin, x, y);
			Bbox clip_bbox(x, x, y, y);              
			parent->getAffine().fwd(bbox.xmin, bbox.ymax, x, y);
			clip_bbox |= Bbox(x, x, y, y) ;          
			parent->getAffine().fwd(bbox.xmax, bbox.ymin, x, y);
			clip_bbox |= Bbox(x, x, y, y) ;          
			parent->getAffine().fwd(bbox.xmax, bbox.ymax, x, y);
			clip_bbox |= Bbox(x, x, y, y);

			clip_xmin = std::max(int(ceil(clip_bbox.xmin)), 0);
			clip_xmax = std::min(int(floor(clip_bbox.xmax)), w);
			clip_ymin = std::max(int(ceil(clip_bbox.ymin)), 0);
			clip_ymax = std::min(int(floor(clip_bbox.ymax)), h);
		}

		draw_buf.resize(w, h);
		draw_buf.transform(data_buf, screen_to_data, bilinear);

		window->draw_rgb_image(
			parent->get_style()->get_fg_gc(Gtk::STATE_NORMAL),
			clip_xmin, clip_ymin, clip_xmax-clip_xmin, clip_ymax-clip_ymin,
			Gdk::RGB_DITHER_NONE,
			&draw_buf(clip_xmin, clip_ymin, 0),
			w*3
		);
	}
}

AffineTransform RasterAreaImpl::getAffine() {
	return AffineTransform::boxToBox(
		Bbox(0, data_buf.w, data_buf.h, 0), bbox, swap_axes);
}

RawRGB &RasterArea::getDataBuf() { return impl->data_buf; }

AffineTransform RasterArea::getAffine() { return impl->getAffine(); }

} // namespace scopemm
