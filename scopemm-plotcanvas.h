#ifndef SCOPEMM_PLOTCANVAS_H
#define SCOPEMM_PLOTCANVAS_H

#include <set>

#include <gtkmm/main.h>
#include <gtkmm/box.h>
#include <gtkmm/drawingarea.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#ifdef SCOPEMM_ENABLE_BLITZ
#include <blitz/array.h>
#endif

namespace scopemm {

class PlotLayerBase;
class PlotLayerImplBase;
typedef boost::shared_ptr<PlotLayerImplBase> PlotLayerImplPtr;
class GridLayer;

class PlotCanvas : 
	public Gtk::DrawingArea,
	public boost::noncopyable 
{
public:
	PlotCanvas();
	~PlotCanvas();

	PlotCanvas &addLayer(PlotLayerBase &layer);
	PlotCanvas &removeLayer(PlotLayerBase &layer);

	// FIXME - these should all return a reference to *this
	void setXAutoRange();
	void setYAutoRange();
	void setXRange(double min, double max);
	void setYRange(double min, double max);
	void setSwapAxes(bool state=true);
	void setDrawAxes(bool state=true); // FIXME - make this a layer
	void setDrawXAxis(bool state=true);
	void setDrawYAxis(bool state=true);
	void setDrawGrids(bool state=true); // FIXME - params for x_grid and y_grid
	void setDrawXGrid(bool state=true);
	void setDrawYGrid(bool state=true);

	double getXMin() const { return bbox.xmin; }
	double getXMax() const { return bbox.xmax; }
	double getYMin() const { return bbox.ymin; }
	double getYMax() const { return bbox.ymax; }
	bool getSwapAxes() const { return swap_axes; }

	void coordToScreen(double xi, double yi, double &xo, double &yo) const {
		affine.fwd(xi, yi, xo, yo);
		//double x = (xmax==xmin) ? 0.5 : (xi-xmin)/(xmax-xmin);
		//double y = (ymax==ymin) ? 0.5 : (yi-ymin)/(ymax-ymin);
		//if(swap_axes) std::swap(x, y);
		//y = 1.0-y;
		//xo = x * (screen_w-1);
		//yo = y * (screen_h-1);
	}

	void screenToCoord(double xi, double yi, double &xo, double &yo) const {
		affine.inv(xi, yi, xo, yo);
		////double x = (xmax==xmin) ? 0.5 : (xi-xmin)/(xmax-xmin);
		//double x = (screen_w<=1) ? 0.5 : xi / (screen_w-1);
		//double y = (screen_w<=1) ? 0.5 : yi / (screen_h-1);
		//if(swap_axes) std::swap(x, y);
		//y = 1.0-y;
		//xo = x * (xmax-xmin) + xmin;
		//yo = y * (ymax-ymin) + ymin;
	}

	bool on_expose_event(GdkEventExpose* event);
	void fireChangeEvent();

private:
	void recalcAutoRange();
	void recalcAffine();

	std::set<PlotLayerImplPtr> layers;
	// pointer is needed here because GridLayer is an incomplete type
	std::auto_ptr<GridLayer> grid_layer;
	bool x_auto, y_auto;
	bool draw_x_axis, draw_y_axis;
	int screen_w, screen_h;
	Bbox bbox;
	AffineTransform affine;
	bool swap_axes;
};

class PlotLayerImplBase : private boost::noncopyable {
public:
	PlotLayerImplBase() { }
	virtual ~PlotLayerImplBase() { }
	virtual void draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context>) = 0;
	virtual bool hasMinMax() { return false; }
	virtual double getMinX() { assert(0); }
	virtual double getMaxX() { assert(0); }
	virtual double getMinY() { assert(0); }
	virtual double getMaxY() { assert(0); }
	virtual double getZOrder() = 0;

	std::set<PlotCanvas *> change_listeners;
};

class PlotLayerBase {
	friend class PlotCanvas;

protected:
	void fireChangeEvent();

	PlotLayerImplPtr impl_base;
};

template <class T>
class PlotLayerSub : public PlotLayerBase {
public:
	PlotLayerSub() : impl(new T()) {
		impl_base = PlotLayerImplPtr(impl);
	}

protected:
	T *impl;
};

} // namespace scopemm

#endif // SCOPEMM_PLOTCANVAS_H
