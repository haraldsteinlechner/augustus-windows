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

/*
 *
 *	awx_sound.h
 *
 */

#ifndef AWX_SOUND_H
#define AWX_SOUND_H

#include <vector>
#include <time.h>

#include "awx_vector.h"
#include "awx_string.h"

#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"
#include "fmod/wincompat.h"


void ERRCHECK(FMOD_RESULT result);

struct SoundStruct
{
		unsigned short ID;
		FMOD_VECTOR    position;
		FMOD::Channel  *channel;
		FMOD::Sound	   *sound;
		float		   volume;
};
 
class SoundSystem
{
	public:

	FMOD_RESULT	result;
	FMOD::System *system;
	
	float music_volume;
	bool jukebox_activated;
	int jukebox_current_song;
	
	FMOD_VECTOR listener_pos;
	FMOD_VECTOR listener_vel;
	FMOD_VECTOR listener_fwd;
	FMOD_VECTOR listener_up;

	vector<SoundStruct> sound;
	vector<SoundStruct> music;
    
	/*
	*	Allgemeine Funktionen
	*/
	
	SoundSystem(int ChannelsAtTheSameTime);			/* Parameter bestimmt wieviele Tonspuren gleichzeitig abgespielt werden koennen */
	~SoundSystem();
	
	void SetListenerPosition(awx_vector3f vec);		/* Setzt die Position der "Ohren" */
	void Update();									/* Diese Funktion sollte einmal (oder ein halbes Mal) pro Bild aufgerufen werden */
	
	void CleanupAndQuit();							/* Wird im Destructor aufgerufen */
	
	/*
	*	Musik abspielen
	*/
	
	int AddMusic(awx_string fn);					/* Fuegt dem Soundsystem eine Musikdatei hinzu */
	
	void SetMusicVolume(float vol);					/* Regelt die allgemeine Laustaerke der Hintergrundmusik */
	
	void PlayMusic(int ID);							/* Streamt eine Musikdatei von der Festplatte und gibt diese von Beginn an wieder */
	void PauseMusic(int ID);						/* Pausiert die Wiedergabe (und setzt sie bei erneutem Aufruf fort) */
	void StopMusic(int ID);							/* Stoppt die Wiedergabe der Musikdatei */
	
	int GetMusicPosition(int ID);					/* Liefert die seit Beginn der Wiedergabe vergangenen Millisekunden */
	int GetMusicLength(int ID);						/* Liefert die Laenge einer Musikdatei in Millisekunden */
	
	void RemoveMusic(int ID);						/* Loescht Hinweise auf eine Musikdatei und saeubert den Speicher */
	
	void StartJukebox();							/* Schaltet Wiedergabe aller im SoundSystem vorhandenen Musikdateien
													   in zufaelliger Reihenfolge ein */
	void UpdateJukebox();							/* Hin und wieder aufrufen, am besten stets vor der allgemeinen Update() */
	void StopJukebox();
	
	/*
	*	Sounds abspielen
	*/
	
	int AddSound(awx_string fn, float volume, float min_distance, float max_distance, awx_vector3f vec);
		/*
		 *  Liest eine Sounddatei von der Festplatte und speichert sie im RAM.
		 *
		 *	min_distance: Distanz bis zu welcher die Lautstaerke des Geraeusches nicht abnimmt;
		 *				  Beispiel: min_distance=2 
		 *							--> Ist man zwischen 0 und 2 Metern vom Geraeusch entfernt ist die Lautstaerke 100%
		 *
		 *	max_distance: Distanz ab welcher das Geraeusch nicht mehr wahrgenommen wird (Lautstaerke ist 0%)
		 *
		 *	Zwischen min_distance und max_distance nimmt die Lautstaerke linear ab.
		 */
		 
	void PlaySound(int ID);								/* Spielt einen Soundeffekt ab */
	
	void SetSoundLooping(int ID, bool looping);			/* Bestimmt ob ein Soundeffekt wiederholt abgespielt wird */
	void SetSoundPosition(int ID, awx_vector3f vec);	/* Aendert die Position im 3D-Raum eines Soundeffekts */
	void SetSoundVolume(int ID, float vol);				/* Aendert die Lautstaerke eines Soundeffekts */
	
	void RemoveSound(int ID);							/* Loescht Hinweise auf eine Sounddatei und saeubert den Speicher */
};

#endif