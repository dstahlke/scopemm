#ifndef SCOPEMM_LINEPLOT_H
#define SCOPEMM_LINEPLOT_H

#include <set>

#include <gtkmm/main.h>
#include <gtkmm/box.h>
#include <gtkmm/drawingarea.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#ifdef SCOPEMM_ENABLE_BLITZ
#include <blitz/array.h>
#endif

#include "scopemm-base.h"

namespace scopemm {

class Plot1D;
class PlotLayerBase;
class PlotLayerImplBase;
typedef boost::shared_ptr<PlotLayerImplBase> PlotLayerImplPtr;

class Plot1D : 
	public PlotBase,
	public boost::noncopyable 
{
public:
	Plot1D();
	~Plot1D();

	Plot1D &addTrace(PlotLayerBase &layer);

	// FIXME - these should all return a reference to *this
	void setXAutoRange();
	void setYAutoRange();
	void setXRange(double min, double max);
	void setYRange(double min, double max);
	void setSwapAxes(bool state=true);
	void setDrawAxes(bool state=true);
	void setDrawXAxis(bool state=true);
	void setDrawYAxis(bool state=true);
	void setDrawGrids(bool state=true);
	void setDrawXGrid(bool state=true);
	void setDrawYGrid(bool state=true);

	bool on_expose_event(GdkEventExpose* event);
	void fireChangeEvent();

//#ifdef SCOPEMM_ENABLE_BLITZ
//	template <class T, int N>
//	void setYData(blitz::Array<blitz::TinyVector<T, N>, 1> ydata);
//#endif // SCOPEMM_ENABLE_BLITZ

private:
	void drawStripes(
		const Cairo::RefPtr<Cairo::Context> &cr,
		double from, double to, double step,
		bool horiz
	) const;

	void drawGrid(const Cairo::RefPtr<Cairo::Context> &cr) const;

	void recalcAutoRange();

	std::vector<PlotLayerImplPtr> layers;
	bool x_auto, y_auto;
	bool draw_x_axis, draw_y_axis;
	bool draw_x_grid, draw_y_grid;
};

class PlotLayerImplBase : private boost::noncopyable {
public:
	PlotLayerImplBase() { }
	virtual ~PlotLayerImplBase() { }
	virtual void draw(Plot1D *parent, Cairo::RefPtr<Cairo::Context>) = 0;
	virtual bool hasMinMax() = 0;
	virtual double getMinX() = 0;
	virtual double getMaxX() = 0;
	virtual double getMinY() = 0;
	virtual double getMaxY() = 0;

	std::set<Plot1D *> change_listeners;
};

class PlotLayerBase {
	friend class Plot1D;

protected:
	void fireChangeEvent();

	PlotLayerImplPtr impl_base;
};

///////////////////////////
// FIXME - move this to another file

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

	bool empty() { return impl->xpts.empty(); }

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

//template <class T, int N>
//void Plot1D::setYData(blitz::Array<blitz::TinyVector<T, N>, 1> ydata) {
//	assert(N == traces.size());
//	for(int i=0; i<N; i++) {
//		traces[i].setYData(ydata[i]);
//	}
//}
#endif // SCOPEMM_ENABLE_BLITZ

} // namespace scopemm

#endif // SCOPEMM_LINEPLOT_H
