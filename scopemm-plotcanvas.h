#ifndef SCOPEMM_PLOTCANVAS_H
#define SCOPEMM_PLOTCANVAS_H

#include <set>

#include <gtkmm/main.h>
#include <gtkmm/drawingarea.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#ifdef SCOPEMM_ENABLE_BLITZ
#include <blitz/array.h>
#endif

namespace scopemm {

class PlotCanvasImpl;
typedef boost::shared_ptr<PlotCanvasImpl> PlotCanvasImplPtr;

class PlotCanvas;
typedef boost::shared_ptr<scopemm::PlotCanvas> PlotCanvasPtr;

class PlotLayerImplBase;
typedef boost::shared_ptr<PlotLayerImplBase> PlotLayerImplPtr;
class PlotLayerBase;

// This is noncopyable mainly because I don't understand what goes on when Gtk
// objects are copied.  Also, copies would have to be inserted into
// PlotLayerImpl::change_listeners for each layer in order to receive queue_draw
// events.
class PlotCanvas : 
	public Gtk::DrawingArea,
	public boost::noncopyable 
{
public:
	PlotCanvas();
	~PlotCanvas();

	PlotCanvas &addLayer(PlotLayerBase &layer);
	PlotCanvas &removeLayer(PlotLayerBase &layer);

	PlotCanvas &setXAutoRange();
	PlotCanvas &setYAutoRange();
	PlotCanvas &setXRange(double min, double max);
	PlotCanvas &setYRange(double min, double max);
	PlotCanvas &setBbox(Bbox new_bbox);
	PlotCanvas &setSwapAxes(bool state=true);
	PlotCanvas &setDrawAxes(bool state=true);
	PlotCanvas &setDrawAxes(bool xaxis, bool yaxis);
	PlotCanvas &setDrawGrids(bool state=true);
	PlotCanvas &setDrawGrids(bool xgrid, bool ygrid);
	PlotCanvas &setDrawXGrid(bool state=true);
	PlotCanvas &setDrawYGrid(bool state=true);

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
	PlotCanvas &fireChangeEvent();

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
