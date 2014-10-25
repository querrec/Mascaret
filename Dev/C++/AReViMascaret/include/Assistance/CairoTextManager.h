/*
 * CairoTextManager.h
 *
 *  Created on: 25 mars 2011
 *      Author: soler
 */

#ifndef CAIROTEXTMANAGER_H_
#define CAIROTEXTMANAGER_H_

#include "AReVi/arObject.h"
using namespace AReVi;

namespace AReViMascaret{
class CairoTextManager : public ArObject
{
public:
	AR_CLASS (CairoTextManager)
	AR_CONSTRUCTOR_1(CairoTextManager,ArRef<CairoTexture>, cairoTexture)

	bool printPage(const StlString text, size_t numPage);

	int getFontSize();
	void setFontSize(int val);

protected:
	StlVector<int> findCuts(StlString str);
	ArRef<CairoTexture> _ctexture;
	int _index;
	int _fontSize;
	double _lineHeight;
	int _nbLineByPage;
};
}
#endif /* CAIROTEXTMANAGER_H_ */
