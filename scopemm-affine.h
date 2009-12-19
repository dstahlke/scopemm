#ifndef SCOPEMM_AFFINE_H
#define SCOPEMM_AFFINE_H

namespace scopemm {

class Bbox {
	double xmin, xmax, ymin, ymax;
};

class HalfAffine {
public:
	HalfAffine(
		double _m00, double _m10,
		double _m01, double _m11,
		double _m02, double _m12
	) :
		m00(_m00), m10(_m10),
		m01(_m01), m11(_m11),
		m02(_m02), m12(_m12)
	{ }
		
	void operator()(double xi, double yi, double &xo, double &yo) const {
		xo = m00*xi + m01*yi + m02;
		yo = m10*xi + m11*yi + m12;
	}

	HalfAffine inverse() {
		double det = m00*m11 - m10*m01;
		assert(det);
		double i00 = m11/det;
		double i11 = m00/det;
		double i10 = -m10/det;
		double i01 = -m01/det;
		double i02 = -i00*m02 -i01*m12;
		double i12 = -i10*m02 -i11*m12;
		return HalfAffine(i00, i10, i01, i11, i02, i12);
	}

	const double m00, m10, m01, m11, m02, m12;
};

class AffineTransform {
public:
	AffineTransform(HalfAffine _fwd) :
		fwd(_fwd),
		inv(_fwd.inverse())
	{ }

	static AffineTransform(Bbox from, Bbox to) {
		double sx = (to.xmax-to.xmin) / (from.xmax-from.xmin);
		double sy = (to.ymax-to.ymin) / (from.ymax-from.ymin);
		double dx = to.xmin - from.xmin*sx;
		double dy = to.ymin - from.ymin*sy;
		return AffineTransform(sx, 0, 0, sy, dx, dy);
	}

	const HalfAffine fwd;
	const HalfAffine inv;
};

} // namespace scopemm

#endif // SCOPEMM_AFFINE_H
