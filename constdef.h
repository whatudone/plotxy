#ifndef _CONST_DEF_H_
#define _CONST_DEF_H_

#pragma once
enum PlotType
{
	Type_PlotScatter = 0,
	Type_PlotAScope,
	Type_PlotRTI,
	Type_PlotText,
	Type_PlotLight,
	Type_PlotBar,
	Type_PlotDial,
	Type_PlotAttitude,
	Type_PlotPolar,
	Type_PlotTrack,
	Type_PlotDoppler
};

struct BaseInfo
{
	QString Base_TabName;
	QString Base_PlotName;
};


#endif // _CONST_DEF_H_