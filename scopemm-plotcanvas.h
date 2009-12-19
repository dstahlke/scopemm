#ifndef SCOPEMM_PLOTCANVAS_H
#define SCOPEMM_PLOTCANVAS_H

#include <set>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#ifdef SCOPEMM_ENABLE_BLITZ
#include <blitz/array.h>
#endif

#include "scopemm-base.h"

namespace scopemm {

class PlotLayerBase;
class PlotLayerImplBase;
typedef boost::shared_ptr<PlotLayerImplBase> PlotLayerImplPtr;

class Plot1D : // FIXME - rename
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

} // namespace scopemm

#endif // SCOPEMM_PLOTCANVAS_H
