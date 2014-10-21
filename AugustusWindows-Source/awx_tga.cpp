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

#include "awx_tga.h"

static TGAHeader tgaheader_u;									// TGA header
static TGA tga;												// TGA image data
static Texture texture;


static GLubyte uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};	// Header einer TGA
static GLubyte cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// Header einer Komprimierten TGA

bool LoadTGA(Texture * texture, char * filename)			
{
	FILE * fTGA;											
	fTGA = fopen(filename, "rb");							

	if(fTGA == NULL)										
	{
		project.error_code = 63;
		sprintf(awx_temp_string,"Die Datei <%s> konnte nicht ge‚Äö√†√∂‚Äö√†√áffnet werden.",filename);
		project.error_message=awx_temp_string;
		project.getError();
		
		return false;												
	}

	if(fread(&tgaheader_u, sizeof(tgaheader_u), 1, fTGA) == 0)		
	{
		if(fTGA != NULL)												
		{
			fclose(fTGA);												
		}
		
		project.error_code = 63;
		sprintf(awx_temp_string,"Header von <%s> konnte nicht gelesen werden",filename);
		project.error_message=awx_temp_string;
		project.getError();
		
		return false;									
	}

	if(memcmp(uTGAcompare, &tgaheader_u, sizeof(tgaheader_u)) == 0)			
	{																		
		LoadUncompressedTGA(texture, filename, fTGA);						
	}
	else if(memcmp(cTGAcompare, &tgaheader_u, sizeof(tgaheader_u)) == 0)		
	{																	
		project.error_code = 63;
		sprintf(awx_temp_string,"Datei <%s> ist eine komprimierte Datei - wird nicht unterst‚Äö√†√∂¬¨‚à´tzt",filename);
		project.error_message=awx_temp_string;
		project.getError();
	}
	else																
	{
		project.error_code = 63;
		sprintf(awx_temp_string,"Datei <%s> ist in einem unbekannten Format gespeichert",filename);
		project.error_message=awx_temp_string;
		project.getError();
		
		fclose(fTGA);
		return false;												
	}
	return true;														
}

bool LoadUncompressedTGA(Texture * texture, char * filename, FILE * fTGA)	
{																			
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)			
	{										
		cout << "error4" << endl;
		if(fTGA != NULL)											
		{
			fclose(fTGA);										
		}
		return false;												
	}	

	texture->width  = tga.header[1] * 256 + tga.header[0];					// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = tga.header[3] * 256 + tga.header[2];					// Determine The TGA Height	(highbyte*256+lowbyte)
	texture->bpp	= tga.header[4];										// Determine the bits per pixel
	tga.Width		= texture->width;										// Copy width into local structure						
	tga.Height		= texture->height;										// Copy height into local structure
	tga.Bpp			= texture->bpp;											// Copy BPP into local structure

	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp !=32)))	
	{
		if(fTGA != NULL)													
		{
			fclose(fTGA);												
		}
		
		project.error_code = 63;
		sprintf(awx_temp_string,"Datei <%s> liegt in einem nicht-erkannten Format vor",filename);
		project.error_message=awx_temp_string;
		project.getError();
		
		return false;													
	}
	
	if(texture->width %8 !=0 || texture->height %8 !=0) 
	{
		project.error_code = 63;
		sprintf(awx_temp_string,"Texturdatei <%s> ist in der falschen Aufl‚Äö√†√∂‚Äö√†√ásung",filename);
		project.error_message=awx_temp_string;
		project.getError();
		
		if(fTGA != NULL)
		{
			fclose(fTGA);
		}
	}

	if(texture->bpp == 24)												
		texture->type	= GL_RGB;											
	else																
		texture->type	= GL_RGBA;										

	tga.bytesPerPixel	= (tga.Bpp / 8);									
	tga.imageSize		= (tga.bytesPerPixel * tga.Width * tga.Height);		
	texture->imageData	= (GLubyte *)malloc(tga.imageSize);					

	if(texture->imageData == NULL)								
	{
		project.error_code = 63;
		sprintf(awx_temp_string,"Zu wenig Arbeitsspeicher (Datei: <%s>)",filename);
		project.error_message=awx_temp_string;
		project.getError();
		
		fclose(fTGA);												
		return false;												
	}

	if(fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize)
	{
		project.error_code = 63;
		sprintf(awx_temp_string,"Header von <%s> passt nicht mit Daten zusammen",filename);
		project.error_message=awx_temp_string;
		project.getError();
		
		if(texture->imageData != NULL)									
		{
			free(texture->imageData);									
		}
		fclose(fTGA);											
		return false;										
	} 
	
	for(GLuint loop = 0; loop < (GLuint)tga.imageSize; loop += tga.bytesPerPixel)
	{
		GLubyte red = texture->imageData[loop+2];
		GLubyte blue = texture->imageData[loop];
		texture->imageData[loop]=red;
		texture->imageData[loop+2]=blue;
	}

	fclose(fTGA);													
	return true;													
}

GLuint load_tgaAndReturnObject(char *file_name, bool mipmaps, bool allowCompression)
{
	if(project.doclevel>=2) awx_utilities.strich();
	if(project.doclevel>=2) printf("Texturdatei in RAM laden: %s\n",file_name);
	
	LoadTGA(&texture, file_name);	

	if(project.doclevel>=2) printf("Texturdatei in VRAM laden: %s\n",file_name);

	GLuint textureObject;
	glGenTextures(1,&textureObject);
	
	glBindTexture(GL_TEXTURE_2D, textureObject);
	if(!mipmaps)
	{
		if(allowCompression&&project.hardwareCompression)
		{
			if(texture.bpp/8==3)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
			}
			else if(texture.bpp/8==4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
			}
			else glTexImage2D(GL_TEXTURE_2D, 0, texture.bpp / 8, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
		}
		else
			glTexImage2D(GL_TEXTURE_2D, 0, texture.bpp / 8, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
			
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		if(project.anisotropicamount>0) glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, &project.anisotropicamount);
	}
	else
	{
		if(allowCompression&&project.hardwareCompression)
		{
			if(texture.bpp/8==3)
			{
				gluBuild2DMipmaps(GL_TEXTURE_2D,GL_COMPRESSED_RGB,texture.width,texture.height,texture.type,GL_UNSIGNED_BYTE,texture.imageData);
			}
			else if(texture.bpp/8==4)
			{
				gluBuild2DMipmaps(GL_TEXTURE_2D,GL_COMPRESSED_RGBA,texture.width,texture.height,texture.type,GL_UNSIGNED_BYTE,texture.imageData);
			}
			else gluBuild2DMipmaps(GL_TEXTURE_2D,texture.bpp/8,texture.width,texture.height,texture.type,GL_UNSIGNED_BYTE,texture.imageData);
		}
		else
			gluBuild2DMipmaps(GL_TEXTURE_2D,texture.bpp/8,texture.width,texture.height,texture.type,GL_UNSIGNED_BYTE,texture.imageData);
	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		if(project.anisotropicamount>0) glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, &project.anisotropicamount);
	}
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	
	//GLfloat v=16.0f;
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, &v);
	

	printf("Textur in VRAM (%s)(ID=%i Mipmaps=%i | Compression=%i/%i)\n",file_name,textureObject,mipmaps,allowCompression,project.hardwareCompression);
	free(texture.imageData);	
	if(project.doclevel>=2) printf("Texturdatei aus RAM gel‚Äö√†√∂‚Äö√†√áscht. %s\n",file_name);


	return textureObject;
}

void load_tga(char *file_name, int current, bool mipmaps, bool allowCompression)
{
	cout << endl << endl << "####### WARNING ----- using depreciated function load_tga. #######" << endl << endl;
	if(project.doclevel>=2) awx_utilities.strich();
	if(project.doclevel>=2) printf("Texturdatei in RAM laden: %s\n",file_name);
	
	LoadTGA(&texture, file_name);	

	if(project.doclevel>=2) printf("Texturdatei in VRAM laden: %s\n",file_name);


	glBindTexture(GL_TEXTURE_2D, textur_liste[current]);
	if(!mipmaps)
	{
		if(allowCompression&&project.hardwareCompression)
		{
			if(texture.bpp/8==3)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
			}
			else if(texture.bpp/8==4)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
			}
			else glTexImage2D(GL_TEXTURE_2D, 0, texture.bpp / 8, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
		}
		else
			glTexImage2D(GL_TEXTURE_2D, 0, texture.bpp / 8, texture.width, texture.height, 0, texture.type, GL_UNSIGNED_BYTE, texture.imageData);
			
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		if(project.anisotropicamount>0) glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, &project.anisotropicamount);
	}
	else
	{
		if(allowCompression&&project.hardwareCompression)
		{
			if(texture.bpp/8==3)
			{
				gluBuild2DMipmaps(GL_TEXTURE_2D,GL_COMPRESSED_RGB,texture.width,texture.height,texture.type,GL_UNSIGNED_BYTE,texture.imageData);
			}
			else if(texture.bpp/8==4)
			{
				gluBuild2DMipmaps(GL_TEXTURE_2D,GL_COMPRESSED_RGBA,texture.width,texture.height,texture.type,GL_UNSIGNED_BYTE,texture.imageData);
			}
			else gluBuild2DMipmaps(GL_TEXTURE_2D,texture.bpp/8,texture.width,texture.height,texture.type,GL_UNSIGNED_BYTE,texture.imageData);
		}
		else
			gluBuild2DMipmaps(GL_TEXTURE_2D,texture.bpp/8,texture.width,texture.height,texture.type,GL_UNSIGNED_BYTE,texture.imageData);
	
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		if(project.anisotropicamount>0) glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, &project.anisotropicamount);
	}
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	
	//GLfloat v=16.0f;
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, &v);
	

	printf("Textur in VRAM (%s)(ID=%i Mipmaps=%i | Compression=%i/%i)\n",file_name,current,mipmaps,allowCompression,project.hardwareCompression);
	free(texture.imageData);	
	if(project.doclevel>=2) printf("Texturdatei aus RAM gel‚Äö√†√∂‚Äö√†√áscht. %s\n",file_name);
}