#include <boost/foreach.hpp>
#include <algorithm>

#include "scopemm.h"

namespace scopemm {

class GridLayerImpl : public PlotLayerImplBase {
public:
	virtual void draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context>);
	virtual double getZOrder() const { return 1; }

	void drawStripes(
		const PlotCanvas *parent,
		const Cairo::RefPtr<Cairo::Context> &cr,
		double from, double to, double step,
		bool vertical
	) const;

	GridLayerImpl();

	bool draw_x_grid, draw_y_grid;
};

GridLayerImpl::GridLayerImpl() :
	draw_x_grid(true), draw_y_grid(true)
{ }

GridLayer::GridLayer() { }

void GridLayer::setDrawXGrid(bool state) {
	impl->draw_x_grid = state;
	fireChangeEvent();
}

void GridLayer::setDrawYGrid(bool state) {
	impl->draw_y_grid = state;
	fireChangeEvent();
}

void GridLayerImpl::drawStripes(
	const PlotCanvas *parent,
	const Cairo::RefPtr<Cairo::Context> &cr,
	double from, double to, double step,
	bool vertical
) const {
	if(from > to) std::swap(from, to);

	const Bbox bbox = parent->getBbox();
	const AffineTransform affine = parent->getAffine();

	for(double p=from; p<=to; p+=step) {
		double x1, y1, x2, y2;
		affine.fwd(vertical ? p : bbox.xmin, vertical ? bbox.ymin : p, x1, y1);
		affine.fwd(vertical ? p : bbox.xmax, vertical ? bbox.ymax : p, x2, y2);
		cr->move_to(x1, y1);
		cr->line_to(x2, y2);
	}

	cr->stroke();
}

static double rangeMagnitude(double span) {
	return log(fabs(span) * 1.0) / log(10);
}

void GridLayerImpl::draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context> cr) {
	double grid_fg = 0.7;
	double canvas_bg = 1.0;

	const Bbox bbox = parent->getBbox();

	cr->save();
	cr->set_line_width(1);
	cr->set_antialias(Cairo::ANTIALIAS_NONE);

	if(draw_x_grid) {
		double mag = rangeMagnitude(bbox.xmax-bbox.xmin);
		double frac = ceil(mag) - mag;
		frac = frac*2.0 - 1.0;
		if(frac > 0) {
			double color = grid_fg*frac + canvas_bg*(1.0-frac);
			cr->set_source_rgb(color, color, color);
			double step = pow(10, floor(mag)-1.0);
			drawStripes(parent, cr, step*ceil(bbox.xmin/step), bbox.xmax, step, true);
		}
	}

	if(draw_y_grid) {
		double mag = rangeMagnitude(bbox.ymax-bbox.ymin);
		double frac = ceil(mag) - mag;
		frac = frac*2.0 - 1.0;
		if(frac > 0) {
			double color = grid_fg*frac + canvas_bg*(1.0-frac);
			cr->set_source_rgb(color, color, color);
			double step = pow(10, floor(mag)-1.0);
			drawStripes(parent, cr, step*ceil(bbox.ymin/step), bbox.ymax, step, false);
		}
	}

	if(draw_x_grid) {
		double mag = rangeMagnitude(bbox.xmax-bbox.xmin);

		cr->set_source_rgb(grid_fg, grid_fg, grid_fg);
		double step = pow(10, floor(mag));
		drawStripes(parent, cr, step*ceil(bbox.xmin/step), bbox.xmax, step, true);
	}

	if(draw_y_grid) {
		double mag = rangeMagnitude(bbox.ymax-bbox.ymin);

		cr->set_source_rgb(grid_fg, grid_fg, grid_fg);
		double step = pow(10, floor(mag));
		drawStripes(parent, cr, step*ceil(bbox.ymin/step), bbox.ymax, step, false);
	}

	cr->restore();
}

} // namespace scopemm
