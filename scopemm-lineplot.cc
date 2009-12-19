#include <boost/foreach.hpp>
#include <algorithm>

#include "scopemm.h"

namespace scopemm {

class PlotTraceImpl : public PlotLayerImplBase {
public:
	PlotTraceImpl() { }

	virtual void draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context>);
	virtual bool hasMinMax() { return !xpts.empty(); }
	virtual double getMinX();
	virtual double getMaxX();
	virtual double getMinY();
	virtual double getMaxY();
	virtual double getZOrder() { return 2; }

	std::vector<double> xpts;
	std::vector<double> ypts;
	double rgb[3];
};

void PlotTraceImpl::draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context> cr) {
	cr->save();
	cr->set_line_width(1);
	cr->set_source_rgb(rgb[0], rgb[1], rgb[2]);

	const int w = parent->get_allocation().get_width();
	const int h = parent->get_allocation().get_height();

	const AffineTransform affine = parent->getAffine();

	assert(xpts.size() == ypts.size());
	size_t npts = xpts.size();
	//std::cout << "npts=" << npts << std::endl;
	for(size_t i=0; i<npts; ++i) {
		double x, y;
		affine.fwd(xpts[i], ypts[i], x, y);
		if(!i) {
			cr->move_to(x, y);
		} else {
			cr->line_to(x, y);
		}
	}
	cr->stroke();
	cr->restore();
}

double PlotTraceImpl::getMinX() { 
	assert(!xpts.empty());
	return *std::min_element(xpts.begin(), xpts.end()); 
}

double PlotTraceImpl::getMaxX() { 
	assert(!xpts.empty());
	return *std::max_element(xpts.begin(), xpts.end()); 
}

double PlotTraceImpl::getMinY() { 
	assert(!ypts.empty());
	return *std::min_element(ypts.begin(), ypts.end()); 
}

double PlotTraceImpl::getMaxY() { 
	assert(!ypts.empty());
	return *std::max_element(ypts.begin(), ypts.end()); 
}

PlotTrace::PlotTrace() { 
	setColor(1, 0, 0);
}

PlotTrace& PlotTrace::setColor(double r, double g, double b) {
	impl->rgb[0] = r;
	impl->rgb[1] = g;
	impl->rgb[2] = b;
	fireChangeEvent();
	return *this;
}

std::vector<double> &PlotTrace::getXPts() { return impl->xpts; }

std::vector<double> &PlotTrace::getYPts() { return impl->ypts; }

} // namespace scopemm
