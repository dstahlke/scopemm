#ifndef SCOPEMM_LINEPLOT_H
#define SCOPEMM_LINEPLOT_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#ifdef SCOPEMM_ENABLE_BLITZ
#include <blitz/array.h>
#endif

#include "scopemm-plotcanvas.h"

namespace scopemm {

class PlotTraceImpl : public PlotLayerImplBase {
	friend class PlotTrace;
public:
	virtual void draw(Plot1D *parent, Cairo::RefPtr<Cairo::Context>);
	virtual bool hasMinMax() { return !xpts.empty(); }
	virtual double getMinX();
	virtual double getMaxX();
	virtual double getMinY();
	virtual double getMaxY();

private:
	PlotTraceImpl() { }

	std::vector<double> xpts;
	std::vector<double> ypts;
	double rgb[3];
};

class PlotTrace : public PlotLayerBase {
public:
	PlotTrace();

	~PlotTrace() { }

	PlotTrace& setColor(double r, double g, double b);

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

private:
	PlotTraceImpl *impl;
};

template <class Iter>
PlotTrace& PlotTrace::setYData(Iter yfirst, Iter ylast, bool steps) {
	size_t npts;
	if(steps) {
		impl->ypts.clear();
		for(Iter p=yfirst; p!=ylast; ++p) {
			impl->ypts.push_back(*p);
			impl->ypts.push_back(*p);
		}
		npts = impl->ypts.size()/2;
	} else {
		impl->ypts.assign(yfirst, ylast);
		npts = impl->ypts.size();
	}

	impl->xpts.clear();
	if(steps) {
		for(size_t i=0; i<npts; ++i) {
			impl->xpts.push_back(i);
			impl->xpts.push_back(i+1);
		}
	} else {
		for(size_t i=0; i<npts; ++i) impl->xpts.push_back(i);
	}

	fireChangeEvent();

	return *this;
}

template <class IterX, class IterY>
PlotTrace& PlotTrace::setXYData(IterX xfirst, IterX xlast, IterY yfirst, IterY ylast) {
	impl->xpts.assign(xfirst, xlast);
	impl->ypts.assign(yfirst, ylast);

	assert(impl->xpts.size() == impl->ypts.size());

	fireChangeEvent();

	return *this;
}

template <class Iter>
PlotTrace& PlotTrace::setXYData(Iter xyfirst, Iter xylast) {
	impl->xpts.clear();
	impl->ypts.clear();

	for(Iter p=xyfirst; p!=xylast; ++p) {
		impl->xpts.push_back(p->first);
		impl->ypts.push_back(p->second);
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
