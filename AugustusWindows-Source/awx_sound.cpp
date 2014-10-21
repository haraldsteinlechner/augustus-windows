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
 *	awx_sound.cpp
 *
 */

#include "awx_sound.h"

/*
*	Allgemeine Funktionen
*/

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
	{
        printf("FMOD-Problem: (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

SoundSystem::SoundSystem(int ChannelsAtTheSameTime)
{
    unsigned int version;
	music_volume = 1.0;
	jukebox_activated = false;
	jukebox_current_song = 0;
	
    listener_pos.x = 0.0f; 
	listener_pos.y = 0.0f; 
	listener_pos.z = 0.0f;

	listener_vel.x = 0.0f;
	listener_vel.y = 0.0f;
	listener_vel.z = 0.0f;
	
	
    listener_fwd.x = 0.0f;
	listener_fwd.y = 0.0f;
	listener_fwd.z = -1.0f;
	
	listener_up.x = 0.0f;
	listener_up.y = 1.0f;
	listener_up.z = 0.0f;

    result = FMOD::System_Create(&system);
    ERRCHECK(result);
    
    result = system->getVersion(&version);
    ERRCHECK(result);
    if (version < FMOD_VERSION)
    {
        printf("Veraltete FMOD-Version!", version, FMOD_VERSION);
        exit(-1);
    }
    
    result = system->init(ChannelsAtTheSameTime, FMOD_INIT_NORMAL, NULL);
    ERRCHECK(result);
}

SoundSystem::~SoundSystem()
{
    CleanupAndQuit();
}

void SoundSystem::SetListenerPosition(awx_vector3f vec)
{
    listener_pos.x = vec.getX();
	listener_pos.y = vec.getY();
	listener_pos.z = vec.getZ();
}

void SoundSystem::Update()
{
    result = system->set3DListenerAttributes(0, &listener_pos, &listener_vel, &listener_fwd, &listener_up);
    ERRCHECK(result);
    system->update();
}

void SoundSystem::CleanupAndQuit()
{
    for(int i=0; i<sound.size(); i++)
    {
        result = sound[i].sound->release();
        ERRCHECK(result);
    }
    for(int i=0; i<music.size(); i++)
    {
        result = music[i].sound->release();
        ERRCHECK(result);
    }
    sound.clear();
    music.clear();
    result = system->close();
    ERRCHECK(result);
    result = system->release();
    ERRCHECK(result);
}

/*
*	Musik abspielen
*/

int SoundSystem::AddMusic(awx_string fn)
{
    int ID = music.size();
    SoundStruct tempstruct = {ID, NULL, NULL, NULL, NULL};
    music.push_back(tempstruct);
    result = system->createSound(fn.cString(), (FMOD_MODE)(FMOD_HARDWARE | FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_OFF), 0, &music.at(ID).sound);
    ERRCHECK(result);
    return ID;
}

void SoundSystem::SetMusicVolume(float vol)
{
    music_volume = vol;
}

void SoundSystem::PlayMusic(int ID)
{
    for(int i=0; i<music.size(); i++)
    {
        if(music.at(i).ID==ID)
        {
            result = system->playSound(FMOD_CHANNEL_FREE, music.at(i).sound, true, &music.at(i).channel);
            ERRCHECK(result);
            result = music.at(i).channel->setVolume(music_volume);
            ERRCHECK(result);
            result = music.at(i).channel->setPaused(false);
            ERRCHECK(result);
            break;
        }
    }
}

void SoundSystem::PauseMusic(int ID)
{
    for(int i=0; i<music.size(); i++)
    {
        if(music.at(i).ID==ID)
        {
            if(music.at(i).channel)
            {
                bool paused;
                result = music.at(i).channel->getPaused(&paused);
                ERRCHECK(result);
                result = music.at(i).channel->setPaused(!paused);
                ERRCHECK(result);
                break;
            }
        }
    }
}

void SoundSystem::StopMusic(int ID)
{
    for(int i=0; i<music.size(); i++)
    {
        if(music.at(i).ID==ID)
        {
            if(music.at(i).channel)
            {
                result = music.at(i).channel->stop();
                ERRCHECK(result);
                break;
            }
        }
    }
}

int SoundSystem::GetMusicPosition(int ID)
{
    for(int i=0; i<music.size(); i++)
    {
        if(music.at(i).ID==ID)
        {
            if(music.at(i).channel)
            {
                unsigned int ms;
                result = music.at(i).channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
                ERRCHECK(result);
                return ms;
            }
        }
    }
    
    return NULL;
}

int SoundSystem::GetMusicLength(int ID)
{
    for(int i=0; i<music.size(); i++)
    {
        if(music.at(i).ID==ID)
        {
            unsigned int ms;
            result = music.at(i).sound->getLength(&ms, FMOD_TIMEUNIT_MS);
            ERRCHECK(result);
            return ms;
        }
    }
    
    return NULL;
}

void SoundSystem::RemoveMusic(int ID)
{
    vector<SoundStruct>::iterator theIterator;
    theIterator = music.begin();
    for(int i=0; i<music.size(); i++)
    {
        if(music.at(i).ID==ID)
        {
            result = music.at(i).channel->stop();
            ERRCHECK(result);
            result = music.at(i).sound->release();
            ERRCHECK(result);
            music.erase(theIterator);
            break;
        }
        theIterator++;
    }
}

void SoundSystem::StartJukebox()
{
	if(!music.empty())
	{
		srand(time(NULL));
		jukebox_activated=true;
	
		jukebox_current_song = rand()%music.size();
		PlayMusic(jukebox_current_song);
	}
}

void SoundSystem::UpdateJukebox()
{
	if(jukebox_activated && !music.empty())
	{
			if(GetMusicPosition(jukebox_current_song)+10 > GetMusicLength(jukebox_current_song))
			{
					int next;
					
					do
					{
						next = rand()%music.size();
					}
					while(next==jukebox_current_song);
					
					jukebox_current_song=next;
					PlayMusic(jukebox_current_song);
			}
	}
}
					
void SoundSystem::StopJukebox()
{
	jukebox_activated=false;
	StopMusic(jukebox_current_song);
}

	
/*
*	Sounds abspielen
*/

/*
*	MinDistance is the minimum distance that the sound emitter will cease to continue growing louder at (as it
*	approaches the listener).
*	Within the mindistance it stays at the constant loudest volume possible. Outside of this mindistance it begins to
*	attenuate.
*	MinDistance is useful to give the impression that the sound is loud or soft in 3d space. An example of this is a
*	small quiet object, such as a bumblebee, which you could set a mindistance of to 0.1 for example, which would
*	cause it to attenuate quickly and dissapear when only a few meters away from the listener.
*	Another example is a jumbo jet, which you could set to a mindistance of 100.0, which would keep the sound
*	volume at max until the listener was 100 meters away, then it would be hundreds of meters more before it would
*	fade out.
*
*	In summary, increase the mindistance of a sound to make it 'louder' in a 3d world, and decrease it to make it
*	'quieter' in a 3d world.
*
*	MaxDistance sets the point where a sound definitely cannot be heard anymore.
*/

int SoundSystem::AddSound(awx_string fn, float volume, float min_distance, float max_distance, awx_vector3f vec)
{
    int ID = sound.size();
    FMOD_VECTOR pos;
	pos.x = vec.getX();
	pos.y = vec.getY();
	pos.z = vec.getZ();
    
    SoundStruct tempstruct = {ID, pos, NULL, NULL, volume};
    sound.push_back(tempstruct);
    
    result = system->createSound(fn.cString(), (FMOD_MODE)(FMOD_HARDWARE | FMOD_3D | FMOD_3D_LINEARROLLOFF | FMOD_LOOP_OFF), 0, &sound.at(ID).sound);
    ERRCHECK(result);
    result = sound.at(ID).sound->set3DMinMaxDistance(min_distance, max_distance);
    ERRCHECK(result);
    
    
    return ID;
}

void SoundSystem::SetSoundLooping(int ID, bool looping)
{
    for(int i=0; i<sound.size(); i++)
    {
        if(sound.at(i).ID==ID)
        {
            if(looping)
            {
                result = sound.at(i).sound->setMode(FMOD_LOOP_NORMAL);
                ERRCHECK(result);
            }
            else if(!looping)
            {
                result = sound.at(i).sound->setMode(FMOD_LOOP_OFF);
                ERRCHECK(result);
            }
        }
    }
}

void SoundSystem::SetSoundPosition(int ID, awx_vector3f vec)
{
    for(int i=0; i<sound.size(); i++)
    {
        if(sound.at(i).ID==ID)
        {
            bool playing;
			sound.at(i).position.x = vec.getX();
			sound.at(i).position.y = vec.getY();
			sound.at(i).position.z = vec.getZ();
			
            result = sound.at(i).channel->isPlaying(&playing);
            ERRCHECK(result);
            if(playing)
            {
                result = sound.at(i).channel->set3DAttributes(&sound.at(i).position, NULL);
                ERRCHECK(result);
            }
        }
    }
}

void SoundSystem::SetSoundVolume(int ID, float vol)
{
    for(int i=0; i<sound.size(); i++)
    {
        if(sound.at(i).ID==ID)
        {
			sound.at(i).volume = vol;
			
            if(sound.at(i).channel)
            {
                result = sound.at(i).channel->setVolume(sound.at(i).volume);
                ERRCHECK(result);
                break;
            }
        }
    }
}

void SoundSystem::PlaySound(int ID)
{
    for(int i=0; i<sound.size(); i++)
    {
        if(sound.at(i).ID==ID)
        {
            result = system->playSound(FMOD_CHANNEL_FREE, sound.at(i).sound, true, &sound.at(i).channel);
            ERRCHECK(result);
            result = sound.at(i).channel->set3DAttributes(&sound.at(i).position, NULL);
            ERRCHECK(result);
            result = sound.at(i).channel->setVolume(sound.at(i).volume);
            ERRCHECK(result);
            result = sound.at(i).channel->setPaused(false);
            ERRCHECK(result);
            break;
        }
    }
}

void SoundSystem::RemoveSound(int ID)
{
    vector<SoundStruct>::iterator theIterator;
    theIterator = sound.begin();
    for(int i=0; i<sound.size(); i++)
    {
        if(sound.at(i).ID==ID)
        {
            result = sound.at(i).channel->stop();
            ERRCHECK(result);
            result = sound.at(i).sound->release();
            ERRCHECK(result);
            sound.erase(theIterator);
            break;
        }
        theIterator++;
    }
}
