#ifndef SCOPEMM_LINEPLOT_H
#define SCOPEMM_LINEPLOT_H

#ifdef SCOPEMM_ENABLE_BLITZ
#include <blitz/array.h>
#endif

#include "scopemm-plotcanvas.h"

namespace scopemm {

class PlotTraceImpl;

class PlotTrace : public PlotLayerSub<PlotTraceImpl> {
public:
	PlotTrace();

	~PlotTrace() { }

	PlotTrace& setColor(double r, double g, double b);

	std::vector<double> &getXPts();
	std::vector<double> &getYPts();

	template <class Iter>
	PlotTrace& setYData(Iter yfirst, Iter ylast, bool steps=false);

	template <class IterX, class IterY>
	PlotTrace& setXYData(IterX xfirst, IterX xlast, IterY yfirst, IterY ylast);

	template <class Iter>
	PlotTrace& setXYData(Iter xyfirst, Iter xylast);

#ifdef SCOPEMM_ENABLE_BLITZ
	template <class T>
	PlotTrace& setYData(blitz::Array<T, 1> ydata, bool steps=false);
	template <class T>
	PlotTrace& setXYData(blitz::Array<blitz::TinyVector<T, 2>, 1> xydata);
#endif // SCOPEMM_ENABLE_BLITZ
};

template <class Iter>
PlotTrace& PlotTrace::setYData(Iter yfirst, Iter ylast, bool steps) {
	std::vector<double> &xpts = getXPts();
	std::vector<double> &ypts = getYPts();

	size_t npts;
	if(steps) {
		ypts.clear();
		for(Iter p=yfirst; p!=ylast; ++p) {
			ypts.push_back(*p);
			ypts.push_back(*p);
		}
		npts = ypts.size()/2;
	} else {
		ypts.assign(yfirst, ylast);
		npts = ypts.size();
	}

	xpts.clear();
	if(steps) {
		for(size_t i=0; i<npts; ++i) {
			xpts.push_back(i);
			xpts.push_back(i+1);
		}
	} else {
		for(size_t i=0; i<npts; ++i) xpts.push_back(i);
	}

	fireChangeEvent();

	return *this;
}

template <class IterX, class IterY>
PlotTrace& PlotTrace::setXYData(IterX xfirst, IterX xlast, IterY yfirst, IterY ylast) {
	std::vector<double> &xpts = getXPts();
	std::vector<double> &ypts = getYPts();

	xpts.assign(xfirst, xlast);
	ypts.assign(yfirst, ylast);

	assert(xpts.size() == ypts.size());

	fireChangeEvent();

	return *this;
}

template <class Iter>
PlotTrace& PlotTrace::setXYData(Iter xyfirst, Iter xylast) {
	std::vector<double> &xpts = getXPts();
	std::vector<double> &ypts = getYPts();

	xpts.clear();
	ypts.clear();

	for(Iter p=xyfirst; p!=xylast; ++p) {
		xpts.push_back(p->first);
		ypts.push_back(p->second);
	}

	fireChangeEvent();

	return *this;
}

#ifdef SCOPEMM_ENABLE_BLITZ
template <class T>
PlotTrace& PlotTrace::setYData(blitz::Array<T, 1> ydata, bool steps) {
	return setYData(ydata.begin(), ydata.end(), steps);
}

template <class T>
PlotTrace& PlotTrace::setXYData(blitz::Array<blitz::TinyVector<T, 2>, 1> xydata) {
	return setXYData(
		xydata[0].begin(), xydata[0].end(), 
		xydata[1].begin(), xydata[1].end()
	);
}

#endif // SCOPEMM_ENABLE_BLITZ

} // namespace scopemm

#endif // SCOPEMM_LINEPLOT_H
