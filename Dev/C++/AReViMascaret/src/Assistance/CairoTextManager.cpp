/*
 * CairoTextManager.cpp
 *
 *  Created on: 25 mars 2011
 *      Author: soler
 */

#include <boost/algorithm/string.hpp>
#include "AReVi/Contrib/cairoTexture.h"
#include "Assistance/CairoTextManager.h"
using std::cerr;
using std::string;
using std::vector;
using std::endl;
using namespace AReVi;

namespace AReViMascaret{

AR_CLASS_NOVOID_DEF(CairoTextManager,ArObject)

CairoTextManager::CairoTextManager(ArCW& arCW, ArRef<CairoTexture> cairoTexture)
: ArObject(arCW), _ctexture(cairoTexture), _index(0), _fontSize(25)
{
	setFontSize(_fontSize);
}
CairoTextManager::~CairoTextManager()
{
}
void CairoTextManager::setFontSize(int size)
{
	unsigned int w,h;
	_ctexture->getSize(w, h);
	_fontSize=size;
	_ctexture->setFontSize(_fontSize);
	_ctexture->setFontFace("helvetica",CairoTexture::FONT_SLANT_ITALIC,CairoTexture::FONT_WEIGHT_BOLD);
	double width2;
	_ctexture->computeTextExtents("test", width2, _lineHeight);
	_nbLineByPage=(int)(h/(_lineHeight*1.25));
}
bool CairoTextManager::printPage(const StlString text, size_t numPage)
{
	unsigned int w, h;
	_ctexture->getSize(w, h);

	// Ajout du nouveau texte
	StlVector<int> cuts = findCuts(text);
	int ligneMax=cuts.size()-1;
	int startIndex = _index;
	int i=1;		//Numéro de ligne
	int j=1;		//Numéro de page
	int lineOffset=0;
	if(ligneMax < _nbLineByPage-1)
	{
		lineOffset=(_nbLineByPage-ligneMax)/2;
	}
	bool somethingToPrint=false;
	while (_index<ligneMax){
		somethingToPrint=true;
		i=_index%_nbLineByPage+1;
		j=_index/_nbLineByPage+1;
		if (_index - startIndex == _nbLineByPage){
			return true;
		}
		string lineText=text.substr(cuts[_index],cuts[_index+1]-cuts[_index]);
		double width2, height2;
		_ctexture->computeTextExtents(lineText, width2, height2);
		_ctexture->moveTo((w-width2)/2.0,(i+lineOffset)*_lineHeight*1.2);
		cerr << "line: "<<lineText << endl;
		_ctexture->showText(lineText);
		_index++;
	}
	return somethingToPrint;
}
StlVector<int> CairoTextManager::findCuts(StlString str)
{
	vector<string> words;
	boost::split(words,str,boost::is_any_of(" "));
	StlVector <int> result;
	result.push_back(0);

	size_t lineBeginWord=0;
	size_t currentWord=0;
	size_t currentIndex=0;
	string previousLine="";
	while(currentWord < words.size())
	{
		string line="";
		for(size_t i=lineBeginWord;i <= currentWord;i++)
		{
			line+=words[i];
			if(i < currentWord)
				line+=" ";
		}
		unsigned int w,h;
		double lineW,lineH;
		_ctexture->getSize(w,h);
		_ctexture->computeTextExtents(line,lineW,lineH);
		if(lineW >= w-10 && lineBeginWord != currentWord)
		{
			currentIndex+=previousLine.length()+1;
			result.push_back(currentIndex);
			lineBeginWord=currentWord;
		}
		else
		{
			previousLine=line;
		}
		currentWord++;
	}
	result.push_back(str.size());

	return result;
}
}
