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

#include "awx_trueTypeFont.h"


vector<fonts>loadedFonts;

void SDL_GL_Enter2DMode()
{
	SDL_Surface *screen = SDL_GetVideoSurface();

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, screen->w, screen->h);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0, (GLdouble)screen->w, (GLdouble)screen->h, 0.0, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

static int power_of_two(int input)
{
	int value = 1;

	while ( value < input ) {
		value <<= 1;
	}
	return value;
}

GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord)
{
	GLuint texture;
	int w, h;
	SDL_Surface *image;
	SDL_Rect area;
	Uint32 saved_flags;
	Uint8  saved_alpha;

	w = power_of_two(surface->w);
	h = power_of_two(surface->h);
	texcoord[0] = 0.0f;			/* Min X */
	texcoord[1] = 0.0f;			/* Min Y */
	texcoord[2] = (GLfloat)surface->w / w;	/* Max X */
	texcoord[3] = (GLfloat)surface->h / h;	/* Max Y */

	image = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			w, h,
			32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			0x000000FF, 
			0x0000FF00, 
			0x00FF0000, 
			0xFF000000
#else
			0xFF000000,
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF
#endif
		       );
	if ( image == NULL ) {
		return 0;
	}

	saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
	saved_alpha = surface->format->alpha;
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, 0, 0);
	}

	area.x = 0;
	area.y = 0;
	area.w = surface->w;
	area.h = surface->h;
	SDL_BlitSurface(surface, &area, image, &area);

	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, saved_flags, saved_alpha);
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_RGBA,
				w, h,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				image->pixels);

	SDL_FreeSurface(image); 

	return texture;
}


void SDL_GL_Leave2DMode()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();
}
	
awx_trueTypeFont::awx_trueTypeFont()
{
    renderstyle = TTF_STYLE_NORMAL;
    caption = "#bui AWX TTF Font Engine\n"
              "========================\n"
              "\n\n"
              "---> rult\n\n";
    maincolor.r = (Uint8) 255;
    maincolor.g = (Uint8) 0;
    maincolor.b = (Uint8) 0;
    ptsize = 12;
    zeilenzaehler = 0;
    zeilenabstand = 0.7;
    path = "Pfad zu Schrift wurde nicht gesetzt";
    
    prepared = false;
    festebreite = 0;
    
    renderFlag = false;
    
    directFont = false;
	
	dynamicScale=1.0;
	
	useAspectCorrection=true;
	
}

awx_trueTypeFont::~awx_trueTypeFont()
{
    //TTF_CloseFont(font);
}

int awx_trueTypeFont::calculate()
{
	longestLine=0;

    zeilenzaehler = 1;
    char temp[2];
    awx_string *zeilen = new awx_string[awx_anzahl_moegliche_zeilen];
    for(int i=0;i<awx_anzahl_moegliche_zeilen;i++)
    {
        zeilen[i] = "";
        quadDaten[i].leerzeile=false;
    }

    TTF_SetFontStyle(font, renderstyle);
    int i=1;
    for(int xi=0;xi<=(int)strlen(caption.cString());xi++)
    {
        if(caption[xi]==13||caption[xi]==10||xi==strlen(caption.cString())||festebreite==0&&strlen(zeilen[i].cString())>=width)
        {
            zeilenzaehler++;
            i++;
            
            if(strlen(zeilen[i-1].cString())==0)
            {
                quadDaten[i-1].leerzeile = true;
                continue;
            } else quadDaten[i-1].leerzeile = false;
        }
        
        else
        {
            sprintf(temp,"%c",caption[xi]);
            zeilen[i] = zeilen[i] + *(new awx_string(temp));
            
            if(festebreite==0||festebreite==-1) continue;
            text = TTF_RenderUTF8_Blended(font, zeilen[i].cString(), maincolor);
            if(text->w>=width)
            {
                int lastWordStart=0;
                // Letztes wort suchen
                for(int u=(int)zeilen[i].length();u>0;u--)
                {
                    if(zeilen[i][u]==' ')
                    {
                        lastWordStart = u+1;
                        break;
                    }
                }
                if(lastWordStart>0)
                {
                    char temmpo[256];
                    int o;
                    for(o=lastWordStart;o<(int)zeilen[i].length();o++) temmpo[o-lastWordStart] = zeilen[i][o];
                    temmpo[o-lastWordStart] = '\0';
            
                    zeilen[i] = zeilen[i].substring(0 , lastWordStart-1);//[lastWordStart-1]='\0';
                    zeilen[i+1]=temmpo;
                    i++;
                    zeilenzaehler++;
                }
                else
                {
                    i++;
                    zeilenzaehler++;
                }
            }
        }
        
    }

    glGenTextures(zeilenzaehler, texture);
    for(int i=1;i<zeilenzaehler;i++)
    {
        //if(quadDaten[i].leerzeile) continue;
        TTF_SetFontStyle(font,renderstyle);
        char formatstring[10];
        int result = sscanf(zeilen[i].cString(),"#%s ",formatstring);
        if(result==1)
        {
            
            int tempstyle = TTF_STYLE_NORMAL;
            bool bold=false;
            bool underline=false;
            bool italic=false;
        
            if(checkFormat(formatstring,'b')) bold = true;
            if(checkFormat(formatstring,'u')) underline = true;
            if(checkFormat(formatstring,'i')) italic = true;

            if(bold) tempstyle |= TTF_STYLE_BOLD;
            if(underline) tempstyle |= TTF_STYLE_UNDERLINE;
            if(italic) tempstyle |= TTF_STYLE_ITALIC;
            
            TTF_SetFontStyle(font,tempstyle);
            
			zeilen[i] = zeilen[i].substring(strlen(formatstring)+2 , zeilen[i].length());
          /*  for(int n=0;n<(int)strlen(zeilen[i].value);n++)
            {
                zeilen[i].value[n] = zeilen[i].value[n+strlen(formatstring)+2];
            }*/
        }

        text = TTF_RenderUTF8_Blended(font, zeilen[i].cString(), maincolor);
        
		if(text!=NULL)
        {
            SDL_Surface *screen = SDL_GetVideoSurface();
            x = (screen->w - text->w)/2;
            y = (screen->h - text->h)/2;
            w = text->w;
            h = text->h;
            wirklichehoehe = h;
            texture[i] = SDL_GL_LoadTexture(text, texcoord);
            SDL_FreeSurface(text);

            texMinX = texcoord[0];
            texMinY = texcoord[1];
            texMaxX = texcoord[2];
            texMaxY = texcoord[3];
        
            quadDaten[i].x=x;
            quadDaten[i].y=y;
            quadDaten[i].w=w;
            quadDaten[i].h=h;
        
            quadDaten[i].texMinX=texMinX;
            quadDaten[i].texMinY=texMinY;
            quadDaten[i].texMaxX=texMaxX;
            quadDaten[i].texMaxY=texMaxY;
			
			if(w>longestLine) longestLine=w;
        }
    }

	
    return 0;
}

void awx_trueTypeFont::calculate(awx_string text)
{
    caption = text;
    calculate();
}

void awx_trueTypeFont::calculate(const char* text)
{
     caption = text;
     calculate();
}

void awx_trueTypeFont::setCaption(awx_string text)
{
    caption = text;
}

void awx_trueTypeFont::setCaption(const char* text)
{
     caption = text;
}

awx_string awx_trueTypeFont::getCaption()
{
    return caption;
}

// Festebreite=0 -> Zeichenbasierende automatische Zeilenumbr√ºche
// Festebreite=1 -> Pixelbasierende automatische Zeilenumbr√ºche
// Festebreite=-1 -> Keine automatischen umbr√ºche

void awx_trueTypeFont::setFormat(bool bold, bool italic, bool underlined, unsigned char r, unsigned char g, unsigned char b,char* pathToFont, int size, float abstand, float width, int festebreite, int useFormating)
{

    if(bold) renderstyle |= TTF_STYLE_BOLD;
    if(italic) renderstyle |= TTF_STYLE_ITALIC;
    if(underlined) renderstyle |= TTF_STYLE_UNDERLINE;
    
    maincolor.r = (Uint8)r;
    maincolor.g = (Uint8)g;
    maincolor.b = (Uint8)b;
    
    path = pathToFont;
    ptsize = size;
    
    prepared = false;
    
    zeilenabstand = abstand;
    
    this->width=width;
    this->festebreite=festebreite;
    
    this->useFormating=useFormating;
    
    init();
}

int awx_trueTypeFont::init()
{
    if(prepared) return 0;
 
    if(TTF_Init()<0)
    {
        fprintf(stderr, "TTF konnte nicht initialisiert werden: %s\n",SDL_GetError());
        return(2);
    }
    
    bool flag=false;

    for(int i=0;i<(int)loadedFonts.size();i++)
    {
        if(strcmp(loadedFonts[i].path.cString(),path.cString())==0&&loadedFonts[i].pt==ptsize)
        {
            flag = true;
            
            // Pointer setzen
            cout << "Benutze Pointer auf font mit index: " << i << endl;
            font = loadedFonts[i].font;
            directFont = false;
        }
    }
    
    if(!flag)
    {
        cout << "Font wird geladen: " << path << endl;
        font = TTF_OpenFont(path.cString(), ptsize);
        directFont = true;
        if ( font == NULL ) {
            fprintf(stderr, "Fehler beim laden der Schrift %s (%i): %s\n",
                    path.cString(), ptsize, SDL_GetError());
            project.error_code = 63;
            sprintf(awx_temp_string,"Die Datei <%s> konnte nicht geöffnet werden.",path.cString());
            project.error_message=awx_temp_string;
            project.getError();
            return(2);
        }
        
        fonts tempfont;
        tempfont.pt=ptsize;
        tempfont.path = path;
        tempfont.font = font;
        loadedFonts.push_back(tempfont);
    }
    
    TTF_SetFontStyle(font, renderstyle);
    
    prepared = true;
    return 0;
}

void awx_trueTypeFont::render(int x, int y)
{
    SDL_GL_Enter2DMode();
    glDisable(GL_LIGHTING);
    for(int i=1;i<=zeilenzaehler;i++)
    {
        if(quadDaten[i].leerzeile) 
        {
            glTranslatef(0,wirklichehoehe,0);
            continue;
        }
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(quadDaten[i].texMinX, quadDaten[i].texMinY); glVertex2i(x,   y  );
        glTexCoord2f(quadDaten[i].texMaxX, quadDaten[i].texMinY); glVertex2i(x+quadDaten[i].w*dynamicScale, y  );
        glTexCoord2f(quadDaten[i].texMinX, quadDaten[i].texMaxY); glVertex2i(x,   y+quadDaten[i].h*dynamicScale);
        glTexCoord2f(quadDaten[i].texMaxX, quadDaten[i].texMaxY); glVertex2i(x+quadDaten[i].w*dynamicScale, y+quadDaten[i].h*dynamicScale);
        glEnd();
        glTranslatef(0,quadDaten[i].h*zeilenabstand,0);
    }
    SDL_GL_Leave2DMode();
}

void awx_trueTypeFont::renderDirect()
{
    if(!renderFlag) return;
    renderFlag = false; 
    
    glPushMatrix();
	
    
    glDisable(GL_LIGHTING);
    for(int i=1;i<zeilenzaehler;i++)
    { 
        if(quadDaten[i].leerzeile) 
        {
            glTranslatef(0,wirklichehoehe,0);
            continue;
        }
        glBindTexture(GL_TEXTURE_2D, texture[i]);
		
		float aspectCorrection = useAspectCorrection ? (1024.0/768.0)/project.aspect : 1;
		
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(quadDaten[i].texMinX, quadDaten[i].texMinY); glVertex2i(renderPositionX*aspectCorrection,  renderPositionY  );
        glTexCoord2f(quadDaten[i].texMaxX, quadDaten[i].texMinY); glVertex2i(renderPositionX*aspectCorrection+quadDaten[i].w*dynamicScale, renderPositionY  );
        glTexCoord2f(quadDaten[i].texMinX, quadDaten[i].texMaxY); glVertex2i(renderPositionX*aspectCorrection,   renderPositionY+quadDaten[i].h*dynamicScale);
        glTexCoord2f(quadDaten[i].texMaxX, quadDaten[i].texMaxY); glVertex2i(renderPositionX*aspectCorrection+quadDaten[i].w*dynamicScale, renderPositionY+quadDaten[i].h*dynamicScale);
        glEnd();
        glTranslatef(0,quadDaten[i].h*zeilenabstand,0);
    }
    
    glPopMatrix();
	
}

int awx_trueTypeFont::getLineHeight()
{
	return wirklichehoehe;
}

int awx_trueTypeFont::getLineWidth(int index)
{
	return quadDaten[index].w;
}

void awx_trueTypeFont::setDynamicScale(float scale)
{
	dynamicScale = scale;
}

void awx_trueTypeFont::renderDirect(int x, int y)
{
    renderPositionX=x;
    renderPositionY=y;
    renderFlag = true;
    
    renderDirect();
}

void awx_trueTypeFont::setRenderPosition(int x, int y)
{
	renderPositionX=x;
    renderPositionY=y;
}

void awx_trueTypeFont::setRenderPositionY(int y)
{
	renderPositionY=y;
}

void awx_trueTypeFont::registerRender(float x, float y)
{
    renderPositionX = (int) x;
    renderPositionY = (int) y;
    
    renderFlag = true;
}

void awx_trueTypeFont::registerRender()
{
    renderFlag = true;
}


void awx_trueTypeFont::renderBlank()
{
    glDisable(GL_LIGHTING);
    for(int i=1;i<=zeilenzaehler;i++)
    {
        if(quadDaten[i].leerzeile) 
        {
            glTranslatef(0,wirklichehoehe,0);
        }
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(quadDaten[i].texMinX, quadDaten[i].texMinY); glVertex2i(x,   y  );
        glTexCoord2f(quadDaten[i].texMaxX, quadDaten[i].texMinY); glVertex2i(x+quadDaten[i].w*dynamicScale, y  );
        glTexCoord2f(quadDaten[i].texMinX, quadDaten[i].texMaxY); glVertex2i(x,   y+quadDaten[i].h*dynamicScale);
        glTexCoord2f(quadDaten[i].texMaxX, quadDaten[i].texMaxY); glVertex2i(x+quadDaten[i].w*dynamicScale, y+quadDaten[i].h*dynamicScale);
        glEnd();
        glTranslatef(0,quadDaten[i].h*zeilenabstand,0);
    }
}



bool awx_trueTypeFont::checkFormat(const char* text, char format)
{
    for(int i=0;i<(int)strlen(text);i++)
    {
        if(text[i]==format) return true;
    }
    return false;
}	

void awx_trueTypeFont::cleanup()
{
    if(directFont) TTF_CloseFont(font);

    glDeleteTextures(zeilenzaehler,texture);
}

int awx_trueTypeFont::getWidth()
{
	return h;
}

	
int awx_trueTypeFont::getHeight()
{
	return w;
}

void awx_trueTypeFont::setAspectCorrection(bool f)
{
	useAspectCorrection=f;
}

int awx_trueTypeFont::getLongestLine()
{
	return longestLine;
}