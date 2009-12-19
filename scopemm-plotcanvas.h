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

class PlotCanvas : 
	public Gtk::DrawingArea,
	public boost::noncopyable 
{
public:
	PlotCanvas();
	~PlotCanvas();

	PlotCanvas &addTrace(PlotLayerBase &layer);

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

	double getXMin() const { return xmin; }
	double getXMax() const { return xmax; }
	double getYMin() const { return ymin; }
	double getYMax() const { return ymax; }
	bool getSwapAxes() const { return swap_axes; }

	inline void coordToScreen(double xi, double yi, double &xo, double &yo) const {
		double x = (xmax==xmin) ? 0.5 : (xi-xmin)/(xmax-xmin);
		double y = (ymax==ymin) ? 0.5 : (yi-ymin)/(ymax-ymin);
		if(swap_axes) std::swap(x, y);
		y = 1.0-y;
		xo = x * (screen_w-1);
		yo = y * (screen_h-1);
	}

	inline void screenToCoord(double xi, double yi, double &xo, double &yo) const {
		double x = (screen_w<=1) ? 0.5 : xi / (screen_w-1);
		double y = (screen_w<=1) ? 0.5 : yi / (screen_h-1);
		if(swap_axes) std::swap(x, y);
		y = 1.0-y;
		xo = x * (xmax-xmin) + xmin;
		yo = y * (ymax-ymin) + ymin;
	}

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
	int screen_w, screen_h;
	double xmin, xmax, ymin, ymax;
	bool swap_axes;
};

class PlotLayerImplBase : private boost::noncopyable {
public:
	PlotLayerImplBase() { }
	virtual ~PlotLayerImplBase() { }
	virtual void draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context>) = 0;
	virtual bool hasMinMax() = 0;
	virtual double getMinX() = 0;
	virtual double getMaxX() = 0;
	virtual double getMinY() = 0;
	virtual double getMaxY() = 0;

	std::set<PlotCanvas *> change_listeners;
};

class PlotLayerBase {
	friend class PlotCanvas;

protected:
	void fireChangeEvent();

	PlotLayerImplPtr impl_base;
};

} // namespace scopemm

#endif // SCOPEMM_PLOTCANVAS_H
