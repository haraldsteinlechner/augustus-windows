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

vector<awx_trueTypeFont> ttfFonts;

int untertitelFontIndex=-1;
int untertitelTimedFontIndex=-1;

bool allowTerrainCulling=true;


GLuint loadingTexture=0;

awx_fuzzy_rule_container *fuzzyLogicContainers;

class saveGame
{
	public:
	
	awx_string path;
	awx_string name;
	
	int fontIndex;
};

vector<saveGame> saveGames;

float fowDarkValue=0.14;
bool useAutoCheckPoints=false;
bool useAStarAlgo=true;

GLuint farbAusgleichQuad;
awx_shader farbausgleichsShader;

class awx_particleSystem;
awx_particleSystem *globalFire,*globalFireSmoke;

awxRandom *globalRandomGenerator;

GLuint iAmInInterfaceOcclusionQueryObject=-1;

typedef enum awx_gamestate
{
	AWXPlayingIntroVideo,
	AWXExplainingGameToUser,
	AWXStandardGame,
	AWXInternalSecondaryMovie,
	AWXInMainMenu,
	AWXNoGamestate
};

awx_gamestate currentGameState=AWXInMainMenu;
