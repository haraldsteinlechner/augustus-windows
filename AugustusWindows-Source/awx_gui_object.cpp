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

#include "awx_gui_object.h"

void awx_gui_object::setMouseDownAction( void (*down)(float x , float y , int button))
{
    mouseDownFunc = down;
    mdown = true;
}

void awx_gui_object::setMouseUpAction( void (*up)(float x , float y , int button))
{
    mouseUpFunc = up;
    mup = true;
}

void awx_gui_object::setKeyDownAction( void (*kdown)(int code))
{
    keyDownFunc = kdown;
    this->kdown = true;
}

void awx_gui_object::setMouseOverAction( void (*over)(float x , float y))
{
    mouseOverFunc = over;
    this->mover = true;
}

void awx_gui_object::setKeyUpAction( void (*kup)(int code))
{
    keyUpFunc = kup;
    this->kup = true;
}

void awx_gui_object::mouseOver(float x , float y){
    if(this->x <= x && this->y <= y && this->x + width >= x && this->y + height >= y){
        if(mover){
            mouseOverFunc(x,y);
        }
    }
}

void awx_gui_object::mouseDown(float x , float y , int button){
    if(mdown){
        mouseDownFunc(x , y , button);
    }
}

void awx_gui_object::mouseUp(float x , float y , int button){
    if(mup){
        mouseUpFunc(x , y , button);
    }
}

void awx_gui_object::keyDown(int code){
    if(kdown){
        keyDownFunc(code);
    }
}

void awx_gui_object::keyUp(int code){
    if(kup){
        keyUpFunc(code);
    }
}


awx_gui_object::awx_gui_object(){
    resize = false;
    
    mdown = false;
    kdown = false;
    kup = false;
    mup = false;
    mover = false;
    hidden = false;
}