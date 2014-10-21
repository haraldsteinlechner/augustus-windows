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

void mouseOverHolz(float x, float y){
	awx_show_info(holzTextID);
}

void mouseOverStein(float x, float y){
	awx_show_info(steinTextID);
}

void mouseOverGold(float x, float y){
	awx_show_info(goldTextID);
}

void mouseOverNahrung(float x, float y){
	awx_show_info(nahrungsTextID);
}

void closeButtonDown(float x , float y , int button){
	awx_shutdown(0);
}

void changeAttitudeAggressive(float x,float y,int button)
{
	for(int i=0;i<(int)controls.selection.size();i++)
	{
		player[controlIndex].units[controls.selection[i]].status = AWX_AGGRESSIVE;
	}
}

void mouseOverAggressive(float x,float y)
{
	awx_show_info(aggressivTextID);
}

void changeAttitudeDefensive(float x,float y,int button)
{
	for(int i=0;i<(int)controls.selection.size();i++)
	{
		player[controlIndex].units[controls.selection[i]].status = AWX_DEFENSIVE;
	}
}

void mouseOverDefensive(float x,float y)
{
	awx_show_info(defensivTextID);
}

void changeAttitudePassive(float x,float y,int button)
{
	for(int i=0;i<(int)controls.selection.size();i++)
	{
		player[controlIndex].units[controls.selection[i]].status = AWX_PASSIVE;
	}
	awx_show_info(passivTextID);

}

void mouseOverPassive(float x,float y)
{
	awx_show_info(passivTextID);
}

void changeFormationSquare(float x,float y,int button)
{
	for(int i=0;i<(int)controls.selection.size();i++)
	{
		player[controlIndex].units[controls.selection[i]].formation = AWX_SQUARE;
	}
	//awx_show_info(squareTextID);
}

void changeFormationTriangle(float x,float y,int button)
{
	for(int i=0;i<(int)controls.selection.size();i++)
	{
		player[controlIndex].units[controls.selection[i]].formation = AWX_TRIANGLE;
	}
	//awx_show_info(triangleTextID);
}

void changeFormationNone(float x,float y,int button)
{
	for(int i=0;i<(int)controls.selection.size();i++)
	{
		player[controlIndex].units[controls.selection[i]].formation = AWX_NONE;
	}
//	awx_show_info(noneTextID);
}

void setCurrentPlayer(int ID)
{
	controlIndex = ID;
	cout << "SetPlayer:" << ID << endl;
	controls.selection.clear();
}

void useFlagPositioning(float x,float y,int button)
{
	controls.draggingFlag=true;
}

