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
	void setBbox(Bbox new_bbox);
	void setSwapAxes(bool state=true);
	void setDrawAxes(bool state=true); // FIXME - make this a layer
	void setDrawXAxis(bool state=true);
	void setDrawYAxis(bool state=true);
	void setDrawGrids(bool state=true);
	void setDrawGrids(bool xgrid, bool ygrid);
	void setDrawXGrid(bool state=true);
	void setDrawYGrid(bool state=true);

	const Bbox &getBbox() const { return bbox; }
	const AffineTransform &getAffine() const { return affine; }
	bool getSwapAxes() const { return swap_axes; }

	void coordToScreen(double xi, double yi, double &xo, double &yo) const {
		affine.fwd(xi, yi, xo, yo);
	}

	void screenToCoord(double xi, double yi, double &xo, double &yo) const {
		affine.inv(xi, yi, xo, yo);
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

class PlotLayerBase {
	friend class PlotCanvas;
public:
	void setZOrder(double z);

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
