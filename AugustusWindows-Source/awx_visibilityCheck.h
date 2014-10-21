///////////////////////////////////////////////////////////////////////////////
//                                                                       //  //
//       AA     WW             WW  XX       XX                           //  //
//	    A  A     WW           WW    XX     XX                            //  //
//     AA  AA     WW         WW      XX   XX                             //  //
//    AAAAAAAA     WW   W   WW         XXX				                 //  //
//   AA	     AA     WW WWW WW        XX   XX                             //  //
//  AA        AA     WWW WWW        XX     XX                            //  //
// AA          AA     W   W        XX       XX   © Alphaworx Corporation //  //
//                                                                       //  //
///////////////////////////////////////////////////////////////////////////////

#ifndef AWX_VISIBILITY_CHECK_H
#define AWX_VISIBILITY_CHECK_H

class visibilityCheck
{
	public:
	
	bool visible;
	bool enabled;
	
	GLdouble winx,winy,winz;
	
	visibilityCheck()
	{
		enabled = true;
		visible = true;
	}
	
	void checkVisibility(float x, float y, float z)
	{
		gluProject(x,y,z,project.mvmatrix,project.projmatrix,project.viewport,&winx,&winy,&winz);

		if(winx>-80&&winx<project.viewport[2]+80&&winy>-20&&winy<project.viewport[3]+80&&winz>0&&winz<0.98)
		{
			visible=true;
		} else visible=false;
	}
	
	bool isVisible()
	{
		return visible;
	}
	
	bool isCheckEnabled()
	{
		return enabled;
	}
};

#endif
	
	