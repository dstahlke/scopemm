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

class PlotCanvasImpl;
typedef boost::shared_ptr<PlotCanvasImpl> PlotCanvasImplPtr;

class PlotLayerImplBase;
typedef boost::shared_ptr<PlotLayerImplBase> PlotLayerImplPtr;
class PlotLayerBase;

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
	void setDrawAxes(bool state=true);
	void setDrawAxes(bool xaxis, bool yaxis);
	void setDrawGrids(bool state=true);
	void setDrawGrids(bool xgrid, bool ygrid);
	void setDrawXGrid(bool state=true);
	void setDrawYGrid(bool state=true);

	const Bbox &getBbox() const;
	const AffineTransform &getAffine() const;
	bool getSwapAxes() const;

	double mouseX() const;
	double mouseY() const;
	bool mouseIn() const;
	bool mouseButton1() const;
	bool mouseButton2() const;
	bool mouseButton3() const;

	sigc::signal<void, int> &signal_plot_clicked();
	sigc::signal<void> &signal_plot_motion();

	bool on_expose_event(GdkEventExpose* event);
	void fireChangeEvent();

private:
	PlotCanvasImplPtr impl;
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
