GLuint untertitelObjectList=0;

void initUntertitelObjects()
{
	untertitelObjectList = glGenLists(1);
	
	GLfloat balkenhoehe=0.1;
	
	glNewList(untertitelObjectList,GL_COMPILE);
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	
	glColor4f(0.0f,0.0f,0.0f,1.0f);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0,1.0,0.0,1.0);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBegin(GL_QUADS);
	
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(1.0f,0.0f,0.0f);
	glVertex3f(1.0f,balkenhoehe,0.0f);
	glVertex3f(0.0f,balkenhoehe,0.0f);
	
	glVertex3f(0.0f,1.0f,0.0f);
	glVertex3f(1.0f,1.0f,0.0f);
	glVertex3f(1.0f,1.0f-balkenhoehe,0.0f);
	glVertex3f(0.0f,1.0f-balkenhoehe,0.0f);
	
	glEnd();

	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	
	glPopAttrib();
		
	
	glEndList();
	

	// TTFFont für Untertitel initialisieren
	awx_trueTypeFont tempFont;
	tempFont.setFormat(false,false,false,255,255,255,"Data/fonts/arial.ttf",22,0.7,150,-1,true);
	tempFont.init();
	tempFont.calculate("Untertitel");
	tempFont.setAspectCorrection(false);
	untertitelFontIndex = (int)ttfFonts.size();
	ttfFonts.push_back(tempFont);
	
	
	awx_trueTypeFont tempFont2;
	tempFont2.setFormat(false,false,false,255,255,255,"Data/fonts/arial.ttf",22,0.7,150,-1,true);
	tempFont2.init();
	tempFont2.calculate("Untertitel");
	tempFont2.setAspectCorrection(false);
	untertitelTimedFontIndex = (int)ttfFonts.size();
	ttfFonts.push_back(tempFont2);
}



void renderUntertitelDisplay()
{
	glCallList(untertitelObjectList);
	
	if(currentLevel!=NULL && untertitelFontIndex>=0 && untertitelFontIndex<ttfFonts.size() && untertitelTimedFontIndex>=0 && untertitelTimedFontIndex<ttfFonts.size())
	{
		float w=ttfFonts[untertitelFontIndex].w*0.5;
		//ttfFonts[currentLevel->cameraDrives[0].untertitelTTFFontIdentifier].setAspectCorrection(false);
		ttfFonts[untertitelFontIndex].registerRender(project.width*0.5-w,project.height*0.925);
		ttfFonts[untertitelTimedFontIndex].registerRender(project.width*0.5-ttfFonts[untertitelTimedFontIndex].w*0.5,project.height*0.025);
	}
}