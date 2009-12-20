#ifndef SCOPEMM_AXES_H
#define SCOPEMM_AXES_H

#include "scopemm-plotcanvas.h"

namespace scopemm {

class AxesLayerImpl;

class AxesLayer : public PlotLayerSub<AxesLayerImpl> {
public:
	AxesLayer();
	~AxesLayer() { }

	void setDrawXAxis(bool state);
	void setDrawYAxis(bool state);
};

} // namespace scopemm

#endif // SCOPEMM_AXES_H
