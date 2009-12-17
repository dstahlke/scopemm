#ifndef SCOPEMM_BASE_H
#define SCOPEMM_BASE_H

class PlotBase {
public:
	PlotBase() :
		xmin(0), xmax(1), ymin(0), ymax(1)
	{ }

	double getXMin() const { return xmin; }
	double getXMax() const { return xmax; }
	double getYMin() const { return ymin; }
	double getYMax() const { return ymax; }

protected:
	double xmin, xmax, ymin, ymax;
};

#endif // SCOPEMM_BASE_H
