#ifndef GTK_BLITZ_LINEPLOT
#define GTK_BLITZ_LINEPLOT

#include <gtkmm/main.h>
#include <gtkmm/box.h>
#include <gtkmm/drawingarea.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

class Plot1D;
class PlotTrace;

typedef boost::shared_ptr<PlotTrace> PlotTracePtr;

class Plot1D : public Gtk::HBox {
	friend class PlotTrace;

public:
	Plot1D();
	virtual ~Plot1D() { }

	virtual PlotTracePtr addTrace();
	virtual void setXAutoRange();
	virtual void setYAutoRange();
	virtual void setXRange(double min, double max);
	virtual void setYRange(double min, double max);
	virtual bool on_expose_event(GdkEventExpose* event);

private:
	void dataChanged();
	void recalcAutoRange();

	std::vector<PlotTracePtr> traces;
	bool x_auto, y_auto;
	double xmin, xmax, ymin, ymax;
};

class PlotTrace : private boost::noncopyable {
public:
	PlotTrace(Plot1D *_parent);

	~PlotTrace();

	void setXAutoRange();
	void setYAutoRange();
	void setXRange(double min, double max);
	void setYRange(double min, double max);
	void setColor(double r, double g, double b);

	template <class Iter>
	void setYData(Iter _yfirst, Iter _ylast);
	template <class Iter>
	void setXYData(Iter _xfirst, Iter _xlast, Iter _yfirst, Iter _ylast);
	void setYData(double *_ypts, size_t _npts);
	void setXYData(double *_xpts, double *_ypts, size_t _npts);
	template <class T>
	void setYData(T _ypts);
	template <class T>
	void setXYData(T _xpts, T _ypts);

	void draw(Cairo::RefPtr<Cairo::Context>);

private:
	void dataChanged();
	
	Plot1D *parent;
	std::vector<double> xpts;
	std::vector<double> ypts;
	bool x_auto, y_auto;
	double xmin, xmax, ymin, ymax;
	double rgb[3];
};

template <class Iter>
void PlotTrace::setYData(Iter _yfirst, Iter _ylast) {
	ypts.clear();
	ypts.insert(ypts.begin(), _yfirst, _ylast);

	xpts.clear();
	size_t npts = ypts.size();
	for(size_t i=0; i<npts; ++i) xpts.push_back(i);

	dataChanged();
}

template <class Iter>
void PlotTrace::setXYData(Iter _xfirst, Iter _xlast, Iter _yfirst, Iter _ylast) {
	ypts.clear();
	ypts.insert(ypts.begin(), _yfirst, _ylast);

	xpts.clear();
	xpts.insert(xpts.begin(), _xfirst, _xlast);

	assert(xpts.size() == ypts.size());

	dataChanged();
}

template <class T>
void PlotTrace::setYData(T _ypts) {
	setYData(_ypts.begin(), _ypts.end());
}

template <class T>
void PlotTrace::setXYData(T _xpts, T _ypts) {
	setXYData(_xpts.begin(), _xpts.end(), _ypts.begin(), _ypts.end());
}

#endif // GTK_BLITZ_LINEPLOT
