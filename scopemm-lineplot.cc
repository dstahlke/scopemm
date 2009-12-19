#include "scopemm-lineplot.h"
#include <boost/foreach.hpp>
#include <algorithm>

using namespace scopemm;

void PlotTraceImpl::draw(Plot1D *parent, Cairo::RefPtr<Cairo::Context> cr) {
	cr->save();
	cr->set_line_width(1);
	cr->set_source_rgb(rgb[0], rgb[1], rgb[2]);

	const int w = parent->get_allocation().get_width();
	const int h = parent->get_allocation().get_height();

	const double xmin = parent->getXMin();
	const double xmax = parent->getXMax();
	const double ymin = parent->getYMin();
	const double ymax = parent->getYMax();
	const bool swap_axes = parent->getSwapAxes();

	size_t npts = xpts.size();
	//std::cout << "npts=" << npts << std::endl;
	for(size_t i=0; i<npts; ++i) {
		double x = (xpts[i]-xmin)/(xmax-xmin);
		double y = (ypts[i]-ymin)/(ymax-ymin);
		if(swap_axes) std::swap(x, y);
		y = 1.0-y;
		x *= (w-1);
		y *= (h-1);
		//if(i<5) {
		//	std::cout << "xpts=" << xpts[i] << std::endl;
		//	std::cout << "ypts=" << ypts[i] << std::endl;
		//	std::cout << "xy=" << x << "," << y << std::endl;
		//}
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

PlotTrace::PlotTrace() :
	impl(new PlotTraceImpl())
{ 
	impl_base = PlotLayerImplPtr(impl);
	setColor(1, 0, 0);
}

PlotTrace& PlotTrace::setColor(double r, double g, double b) {
	impl->rgb[0] = r;
	impl->rgb[1] = g;
	impl->rgb[2] = b;
	fireChangeEvent();
	return *this;
}
