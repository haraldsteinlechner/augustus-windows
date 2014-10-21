///////////////////////////////////////////////////////////////////////////////
//                                                                       //  //
//       AA     WW             WW  XX       XX                           //  //
//	    A  A     WW           WW    XX     XX                            //  //
//     AA  AA     WW         WW      XX   XX                             //  //
//    AAAAAAAA     WW   W   WW         XXX				                 //  //
//   AA	     AA     WW WWW WW        XX   XX                             //  //
//  AA        AA     WWW WWW        XX     XX                            //  //
// AA          AA     W   W        XX       XX   ¬© Alphaworx Corporation //  //
//                                                                       //  //
///////////////////////////////////////////////////////////////////////////////


#ifndef AWX_APX_H
#define AWX_APX_H


#include "includeOpenGLAndSDL.h"

#include <iostream>
#include <vector>
using namespace std;

#include "awx_vector.h"

#include "awx_project.h"
#include "globals_extern.h"

#include "awx_blob.h"

#include "awx_matrix.h"
#include "awx_cpp_utilities.h"

#define RANDOM_FLOAT (((float)rand())/RAND_MAX)

#ifndef NVIDIACompileError
#define NVIDIACompileError
#endif

/*							AWX APX Version 3.0 Header						*/

typedef struct
{
	float data[4];
} vec4;


typedef struct
{
    int anzahl_frames;		// Anzahl der Frames
	
	// Objektinfos
	int normalenzahl;
	int planezahl;
	int texzahl;
	int vertexzahl;
	
	// Objektgeometrie
	float *vertex;
	float *normal;
	float *tex_koord;
	int *plane;
	int *normalsequenz;
	int *textursequenz;
	int *texturIDs;
	int *texturWinkel;
	vec4 *vertexAttributes;
	
	// Patterneinstellungen
	float pattern_v;
	float pattern_u;
	
	// Falls Moped -> ID
	int part_ID;
} awx_part; 

typedef struct
{
	char *pfad;
	int u,v,id_tex;
} texture_table_typ;

class bildaktionen
{
	public:
	int aktionsID;
	char akthelp[20];
	int akthelp_int;
	char bezeichnung[20];
	bildaktionen()
	{
		strcpy(akthelp,"");
		strcpy(bezeichnung,"");
		aktionsID=-1;
		akthelp_int=-1;
	}
};

typedef struct
{
	awx_vector3f p1,p2;
} edge;

typedef struct
{
	int p1,p2;
} edgeIndex;

class apx: public awx_blob
{
	public:
	
	FILE *input;			
	int anzahl_frames;		// Anzahl der Bilder der Animation
	awx_part *part;			// Datenpointerstructinstanz
	int loop;
	float current_frame;	// Derzeitiges Frame der Animation
	float animationspeed;   // FPS der animation
	float u;				// Pattern in U
	float v;				// Pattern in V
	
	int sektorflag;
	int sektoranzahl;
	
	float xmin, zmin , xmax , zmax, ymin, ymax;
	float bounding_radius;
	
	awx_string workingdirectory;
	
	int IDzaehler;
	int texturanzahl;
	
	texture_table_typ *texture_table;
	bildaktionen *aktionen;
	
	int shadowID;
	bool shadowflag;
	int sichttestflag;
	
	bool cullFace;
	GLint alpha_func;
	float alphaParam;
	bool alpha_funcFlag;
	bool lightModelTwoSided;
	bool allowMipmapsInAPX;
	
	float modelview[16];
	bool sektorisiert;
	
	// Transformationen
	float x;
	float y;
	float z;
	float scale_x;
	float scale_y;
	float scale_z;
	float rotation;
	float rot_x, rot_y, rot_z;
	float trans_x, trans_y,trans_z;
	float simplerotation,simple_rot_x,simple_rot_y,simple_rot_z;
	
	int shadowVolumeID;
	
	// ListenObjekte
	GLuint texture;
	GLuint *IDs;	
	GLuint blobTexture;
	
	bool hasShadowVolume;
	
	bool useWaterShading;
	
	float rotation_x, rotation_y, rotation_z;
	
	// Speichermanagement
	
	unsigned short version;
	vector<edge> silhuetteEdges;
	vector<edgeIndex> edges;

	bool useBumpMapping;
	bool usePerPixelLighting;
		
	GLuint vboVertex,vboTexCoord,vboNormal,vboTextureWeights,vboBegehbarkeit,vboTangents;
	float *updatePtr;
	float *updatePtr2;
	
	apx();	
	~apx();
	void fillVRAM(GLuint loadTexture);
	void load_apx(const char *filename);
	void display_data(int frame);
	void render_frame(int frame);
	void render_frameWithTangents(int frame);
	void fillVRAM(awx_vector3f lPos, bool t,float winkel, GLuint loadTexture);
	void fillVRAM();
	void prepareShadowVolume(float winkel);
	void renderExtrusion();
	void createSilhuette(float winkel);
	void renderShadowVolumeCPU(float infinity);
	void renderFrameFromList();
	void play(float &current_frame);
	void play();
	void renderBlob();
	void gotoBezeichnung(char* bezeichnung);
	void gotoBezeichnung(char* bezeichnung , int random);
	void setSpeed(int speed);
	void setPositionf(float x, float y, float z);
	void movef(float x, float y, float z);
	void scalef(float x, float y, float z);
	void rotate_achsef(float winkel, float trans_x, float trans_y, float trans_z, float rot_x, float rot_y, float rot_z);
	void rotate_um_mittelpunktf(float winkel, float x, float y, float z);
	void render();
	awx_vector3f getPositionOfVector(awx_vector3f vec);
	void renderBlobTexture(awx_vector3f position,awx_vector3f pos);
	void renderExtrusionGPU();
	void renderAtOrigin();
	
	void createVBO();
	void renderVBO();
	void fillVRAMWithBump();
	void renderGamePlayMode();
	
	void startUpdate();	
	void stopUpdate();
	void changeTextureWeight(int loop,int l,int tex,float change);
	void setAllTextureWeights(int index);
	
	void startUpdateBegehbarkeit();
	void stopUpdateBegehbarkeit();
	void changeTextureWeightBegehbarkeit(int loop,int ID);
	void generateBegehbarkeit(float steigung);
	void resetBegehbarkeit(int ID);
	//const apx & operator= (const apx &x);
};

#endif