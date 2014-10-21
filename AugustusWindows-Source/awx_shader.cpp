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

#include "awx_shader.h"

char *textFileRead(char *fn) {


	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = (int)fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	} else return NULL;
	return content;
}

int textFileWrite(char *fn, char *s) {

	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {
			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}

#define printOpenGLError() printOglError(__FILE__, __LINE__)

int printOglError(char *file, int line)
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}


void printInfoLog(GLhandleARB obj)
{
    GLint infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
                                         &infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
			glGetInfoLogARB(obj, infologLength, (GLsizei*) &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

	
void awx_shader::set(char* vertexshader, char* fragmentshader)
{
        // Extensions durchsuchen
    if(OpenGLExtensionIsSupported("GL_ARB_shading_language_100")!=1||OpenGLExtensionIsSupported("GL_ARB_fragment_shader")!=1||OpenGLExtensionIsSupported("GL_ARB_vertex_shader")!=1)
    {
        cout << "Shader wurden angefordert sind jedoch auf diesem Chipsatz mit den zurzeit installierten Treibern nicht unterst√ºtzt" << endl;
        project.fragmentshader=false;
        project.vertexshader=false;
        return;
    }

	p = glCreateProgramObjectARB();

    char *vs = NULL,*fs = NULL;
    

    if(strlen(vertexshader)>0)
    {
        v = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);

        vs = textFileRead(vertexshader);

        if(vs==NULL) return;

        const char * vv = vs;

        glShaderSourceARB(v, 1, &vv,NULL);

        free(vs);

        glCompileShaderARB(v);

        printInfoLog(v);

        glAttachObjectARB(p,v);
    }
    
    if(strlen(fragmentshader)>0)
    {
        f = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

        fs = textFileRead(fragmentshader);

        if(fs==NULL) return;

        const char * ff = fs;

        glShaderSourceARB(f, 1, &ff,NULL);

        free(fs);

        glCompileShaderARB(f);

        printInfoLog(f);

        glAttachObjectARB(p,f);
    }
    
    
    
    glLinkProgramARB(p);
	cout << "Shader <" << vertexshader << "/" << fragmentshader << ">" << endl;
    printInfoLog(p);

    //glUseProgramObjectARB(p);
}

void awx_shader::install()
{
	glUseProgramObjectARB(p);
}

void awx_shader::deinstall()
{
	glUseProgramObjectARB(NULL);
}