#ifndef SCOPEMM_GRID_H
#define SCOPEMM_GRID_H

#include "scopemm-plotcanvas.h"

namespace scopemm {

class GridLayerImpl;

class GridLayer : public PlotLayerSub<GridLayerImpl> {
public:
	GridLayer();
	~GridLayer() { }

	GridLayer &setDrawXGrid(bool state);
	GridLayer &setDrawYGrid(bool state);
};

} // namespace scopemm

#endif // SCOPEMM_GRID_H
