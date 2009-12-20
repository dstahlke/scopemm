#include <boost/foreach.hpp>
#include <algorithm>

#include "scopemm.h"
#include "scopemm-layerimpl.h"

namespace scopemm {

/// PlotCanvasImpl ////////////////////////////////////

class PlotCanvasImpl {
public:
	PlotCanvasImpl();

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

PlotCanvasImpl::PlotCanvasImpl() : 
	grid_layer(new GridLayer()),
	x_auto(true), y_auto(true),
	draw_x_axis(false), draw_y_axis(false),
	bbox(0, 1, 0, 1), swap_axes(false)
{ }

void PlotCanvasImpl::recalcAutoRange() {
	if(x_auto) {
		bool first = true;
		double min=0, max=0;
		BOOST_FOREACH(const PlotLayerImplPtr &layer, layers) {
			if(!layer->hasMinMax()) continue;
			Bbox sub_bbox = layer->getBbox();
			if(first || sub_bbox.xmin < min) min = sub_bbox.xmin;
			if(first || sub_bbox.xmax > max) max = sub_bbox.xmax;
			first = false;
		}
		double delta = max-min;
		min -= delta * 0.05;
		max += delta * 0.05;
		// if no layers had min/max
		if(first) { min = 0; max = 1; }
		if(min == max) { min -= 1; max += 1; }
		bbox.xmin = min;
		bbox.xmax = max;
	}
	if(y_auto) {
		bool first = true;
		double min=0, max=0;
		BOOST_FOREACH(const PlotLayerImplPtr &layer, layers) {
			if(!layer->hasMinMax()) continue;
			Bbox sub_bbox = layer->getBbox();
			if(first || sub_bbox.ymin < min) min = sub_bbox.ymin;
			if(first || sub_bbox.ymax > max) max = sub_bbox.ymax;
			first = false;
		}
		double delta = max-min;
		min -= delta * 0.05;
		max += delta * 0.05;
		// if no layers had min/max
		if(first) { min = 0; max = 1; }
		if(min == max) { min -= 1; max += 1; }
		bbox.ymin = min;
		bbox.ymax = max;
	}
}

void PlotCanvasImpl::recalcAffine() {
	Bbox screen_bbox(0, screen_w, screen_h, 0);
	affine = AffineTransform::boxToBox(bbox, screen_bbox, swap_axes);
}

/// PlotCanvas ////////////////////////////////////////

PlotCanvas::PlotCanvas() :
	impl(new PlotCanvasImpl())
{ }

PlotCanvas::~PlotCanvas() {
	// now that our object no longer exists, it must not receive
	// any more events
	BOOST_FOREACH(const PlotLayerImplPtr &layer, impl->layers) {
		layer->change_listeners.erase(this);
	}
}

PlotCanvas &PlotCanvas::addLayer(PlotLayerBase &layer) {
	layer.impl_base->change_listeners.insert(this);
	impl->layers.insert(layer.impl_base);
	fireChangeEvent();
	return *this;
}

PlotCanvas &PlotCanvas::removeLayer(PlotLayerBase &layer) {
	layer.impl_base->change_listeners.erase(this);
	impl->layers.erase(layer.impl_base);
	fireChangeEvent();
	return *this;
}

void PlotCanvas::setXAutoRange() {
	impl->x_auto = true;
	impl->recalcAutoRange();
	fireChangeEvent();
}

void PlotCanvas::setYAutoRange() {
	impl->y_auto = true;
	impl->recalcAutoRange();
	fireChangeEvent();
}

void PlotCanvas::setXRange(double min, double max) {
	impl->bbox.xmin = min;
	impl->bbox.xmax = max;
	impl->x_auto = false;
	impl->recalcAffine();
	fireChangeEvent();
}

void PlotCanvas::setYRange(double min, double max) {
	impl->bbox.ymin = min;
	impl->bbox.ymax = max;
	impl->y_auto = false;
	impl->recalcAffine();
	fireChangeEvent();
}

void PlotCanvas::setBbox(Bbox new_bbox) {
	impl->bbox = new_bbox;
	impl->x_auto = false;
	impl->y_auto = false;
	impl->recalcAffine();
	fireChangeEvent();
}

void PlotCanvas::setSwapAxes(bool state) {
	impl->swap_axes = state;
	fireChangeEvent();
}

void PlotCanvas::setDrawAxes(bool state) {
	impl->draw_x_axis = state;
	impl->draw_y_axis = state;
	fireChangeEvent();
}

void PlotCanvas::setDrawXAxis(bool state) {
	impl->draw_x_axis = state;
	fireChangeEvent();
}

void PlotCanvas::setDrawYAxis(bool state) {
	impl->draw_y_axis = state;
	fireChangeEvent();
}

void PlotCanvas::setDrawGrids(bool state) {
	setDrawGrids(state, state);
}

void PlotCanvas::setDrawGrids(bool xgrid, bool ygrid) {
	if(xgrid || ygrid) {
		impl->grid_layer->setDrawXGrid(xgrid);
		impl->grid_layer->setDrawYGrid(ygrid);
		addLayer(*impl->grid_layer);
	} else {
		removeLayer(*impl->grid_layer);
	}
	fireChangeEvent();
}

const Bbox &PlotCanvas::getBbox() const { return impl->bbox; }

const AffineTransform &PlotCanvas::getAffine() const { return impl->affine; }

bool PlotCanvas::getSwapAxes() const { return impl->swap_axes; }

bool PlotCanvas::on_expose_event(GdkEventExpose* event) {
	Glib::RefPtr<Gdk::Window> window = get_window();

	if(!window) return true;

	impl->screen_w = get_allocation().get_width();
	impl->screen_h = get_allocation().get_height();
	impl->recalcAffine();

	Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
	if(event) {
		cr->rectangle(event->area.x, event->area.y,
			event->area.width, event->area.height);
		cr->clip();
	}

	cr->save();
	cr->set_source_rgb(1, 1, 1);
	cr->paint();
	cr->restore();

	if(impl->draw_x_axis || impl->draw_y_axis) {
		cr->save();
		cr->set_line_width(1);
		cr->set_source_rgb(0.3, 0.3, 0.3);
		cr->set_antialias(Cairo::ANTIALIAS_NONE);
		if(impl->draw_x_axis) {
			double x1, y1, x2, y2;
			coordToScreen(0, impl->bbox.ymin, x1, y1);
			coordToScreen(0, impl->bbox.ymax, x2, y2);
			cr->move_to(x1, y1);
			cr->line_to(x2, y2);
			cr->stroke();
		}
		if(impl->draw_y_axis) {
			double x1, y1, x2, y2;
			coordToScreen(impl->bbox.xmin, 0, x1, y1);
			coordToScreen(impl->bbox.xmax, 0, x2, y2);
			cr->move_to(x1, y1);
			cr->line_to(x2, y2);
			cr->stroke();
		}
		cr->restore();
	}

	typedef std::pair<double, PlotLayerImplPtr> zbuf_element;
	std::set<zbuf_element> zsorted;
	BOOST_FOREACH(const PlotLayerImplPtr &layer, impl->layers) {
		zsorted.insert(zbuf_element(layer->zorder, layer));
	}
	BOOST_FOREACH(const zbuf_element &layer, zsorted) {
		layer.second->draw(this, cr);
	}

	return true;
}

void PlotCanvas::fireChangeEvent() {
	impl->recalcAutoRange();
	queue_draw();
}

/// PlotLayerBase ////////////////////////////////////

void PlotLayerBase::setZOrder(double z) {
	impl_base->zorder = z;
	fireChangeEvent();
}

void PlotLayerBase::fireChangeEvent() {
	BOOST_FOREACH(PlotCanvas *l, impl_base->change_listeners) {
		l->fireChangeEvent();
	}
}

} // namespace scopemm
