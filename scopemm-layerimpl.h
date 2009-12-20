#ifndef SCOPEMM_LAYERIMPL_H
#define SCOPEMM_LAYERIMPL_H

// NOTE: don't include this in the public interface

#include <set>

#include <gtkmm/main.h>
#include <gtkmm/drawingarea.h>

namespace scopemm {

class PlotLayerImplBase : private boost::noncopyable {
public:
	PlotLayerImplBase() { }
	virtual ~PlotLayerImplBase() { }
	virtual void draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context>) = 0;
	virtual bool hasMinMax() const { return false; }
	virtual Bbox getBbox() const { assert(0); }

	std::set<PlotCanvas *> change_listeners;
	double zorder;
};

} // namespace scopemm

#endif // SCOPEMM_LAYERIMPL_H
