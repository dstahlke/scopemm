#ifndef SCOPEMM_AFFINE_H
#define SCOPEMM_AFFINE_H

#include <assert.h>

namespace scopemm {

class Bbox {
public:
	Bbox(
		double _xmin, double _xmax,
		double _ymin, double _ymax
	) :
		xmin(_xmin), xmax(_xmax),
		ymin(_ymin), ymax(_ymax)
	{ }

	Bbox transpose() {
		return Bbox(ymin, ymax, xmin, xmax);
	}

	double xmin, xmax, ymin, ymax;
};

class HalfAffine {
public:
	static const HalfAffine IDENTITY;
	static const HalfAffine SWAP_AXES;

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

	HalfAffine &operator*=(const HalfAffine &b) {
		double c00 = m00*b.m00 + m01*b.m10;
		double c10 = m10*b.m00 + m11*b.m10;
		double c01 = m00*b.m01 + m01*b.m11;
		double c11 = m10*b.m01 + m11*b.m11;
		double c02, c12;
		(*this)(b.m02, b.m12, c02, c12);
		m00 = c00; m10 = c10;
		m01 = c01; m11 = c11;
		m02 = c02; m12 = c12;
		return *this;
	}

	HalfAffine operator*(const HalfAffine &b) const {
		HalfAffine r = *this;
		r *= b;
		return r;
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

	static HalfAffine boxToBox(Bbox from, Bbox to) {
		double sx = 
			(from.xmax-from.xmin)
			? (to.xmax-to.xmin) / (from.xmax-from.xmin)
			: 1;
		double sy = 
			(from.ymax-from.ymin)
			? (to.ymax-to.ymin) / (from.ymax-from.ymin)
			: 1;
		double dx = to.xmin - from.xmin*sx;
		double dy = to.ymin - from.ymin*sy;
		return HalfAffine(sx, 0.0, 0.0, sy, dx, dy);
	}

	double m00, m10, m01, m11, m02, m12;
};

class AffineTransform {
public:
	AffineTransform() :
		fwd(HalfAffine::IDENTITY),
		inv(HalfAffine::IDENTITY)
	{ }

//	AffineTransform(HalfAffine _fwd) :
//		fwd(_fwd),
//		inv(fwd.inverse())
//	{ }

	AffineTransform(HalfAffine _fwd, HalfAffine _inv) :
		fwd(_fwd),
		inv(_inv)
	{ }

	static AffineTransform boxToBox(Bbox from, Bbox to, bool swap_axes=false) {
		// NOTE: if either of the bounding boxes is degenerate, this will
		// still produce usable transforms, although they will not really
		// be inverses of each other
		if(swap_axes) {
			const HalfAffine sa = HalfAffine::SWAP_AXES;
			return AffineTransform(
				HalfAffine::boxToBox(from.transpose(), to) * sa,
				sa * HalfAffine::boxToBox(to, from.transpose())
			);
		} else {
			return AffineTransform(
				HalfAffine::boxToBox(from, to),
				HalfAffine::boxToBox(to, from)
			);
		}
	}

	HalfAffine fwd;
	HalfAffine inv;
};

} // namespace scopemm

#endif // SCOPEMM_AFFINE_H
