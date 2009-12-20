#ifndef SCOPEMM_AXES_H
#define SCOPEMM_AXES_H

#include "scopemm-plotcanvas.h"

namespace scopemm {

class AxesLayerImpl;

class AxesLayer : public PlotLayerSub<AxesLayerImpl> {
public:
	AxesLayer();
	~AxesLayer() { }

	AxesLayer &setDrawXAxis(bool state);
	AxesLayer &setDrawYAxis(bool state);
};

} // namespace scopemm

#endif // SCOPEMM_AXES_H
