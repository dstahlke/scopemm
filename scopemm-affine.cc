#include "scopemm-affine.h"

namespace scopemm {
	const HalfAffine HalfAffine::IDENTITY (1, 0, 0, 1, 0, 0);
	const HalfAffine HalfAffine::SWAP_AXES(0, 1, 1, 0, 0, 0);
}
