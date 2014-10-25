#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <string>
#include <vector>
#include <iostream>

#include "Tools/Sound3D.h"
#include "Tools/alffmpeg.h"

namespace Sound
{
    using std::cout;
    using std::cerr;
    using std::endl;

    using std::make_pair;

    //-------- Helpers functions -------------------------------------------------

#if defined(AL_SOFT_buffer_samples) && AL_SOFT_buffer_samples
    void AL_APIENTRY _wrap_BufferSamples(ALuint buffer, ALuint samplerate,
        ALenum internalformat, ALsizei samples, ALenum channels, ALenum type, const ALvoid *data)
    {
        alBufferData(buffer, internalformat, data,
            FramesToBytes(samples, channels, type),
            samplerate);
    }

    const char *ChannelsName(ALenum chans)
    {
        switch(chans)
        {
            case AL_MONO_SOFT: return "Mono";
            case AL_STEREO_SOFT: return "Stereo";
            case AL_REAR_SOFT: return "Rear";
            case AL_QUAD_SOFT: return "Quadraphonic";
            case AL_5POINT1_SOFT: return "5.1 Surround";
            case AL_6POINT1_SOFT: return "6.1 Surround";
            case AL_7POINT1_SOFT: return "7.1 Surround";
        }
        return "Unknown Channels";
    }

    const char *TypeName(ALenum type)
    {
        switch(type)
        {
            case AL_BYTE_SOFT: return "S8";
            case AL_UNSIGNED_BYTE_SOFT: return "U8";
            case AL_SHORT_SOFT: return "S16";
            case AL_UNSIGNED_SHORT_SOFT: return "U16";
            case AL_INT_SOFT: return "S32";
            case AL_UNSIGNED_INT_SOFT: return "U32";
            case AL_FLOAT_SOFT: return "Float32";
            case AL_DOUBLE_SOFT: return "Float64";
        }
        return "Unknown Type";
    }

    ALsizei FramesToBytes(ALsizei size, ALenum channels, ALenum type)
    {
        switch(channels)
        {
            case AL_MONO_SOFT:    size *= 1; break;
            case AL_STEREO_SOFT:  size *= 2; break;
            case AL_REAR_SOFT:    size *= 2; break;
            case AL_QUAD_SOFT:    size *= 4; break;
            case AL_5POINT1_SOFT: size *= 6; break;
            case AL_6POINT1_SOFT: size *= 7; break;
            case AL_7POINT1_SOFT: size *= 8; break;
        }

        switch(type)
        {
            case AL_BYTE_SOFT:           size *= sizeof(ALbyte); break;
            case AL_UNSIGNED_BYTE_SOFT:  size *= sizeof(ALubyte); break;
            case AL_SHORT_SOFT:          size *= sizeof(ALshort); break;
            case AL_UNSIGNED_SHORT_SOFT: size *= sizeof(ALushort); break;
            case AL_INT_SOFT:            size *= sizeof(ALint); break;
            case AL_UNSIGNED_INT_SOFT:   size *= sizeof(ALuint); break;
            case AL_FLOAT_SOFT:          size *= sizeof(ALfloat); break;
            case AL_DOUBLE_SOFT:         size *= sizeof(ALdouble); break;
        }

        return size;
    }

    ALsizei BytesToFrames(ALsizei size, ALenum channels, ALenum type)
    {
        return size / FramesToBytes(1, channels, type);
    }

    ALenum GetFormat(ALenum channels, ALenum type, LPALISBUFFERFORMATSUPPORTEDSOFT palIsBufferFormatSupportedSOFT)
    {
        ALenum format = AL_NONE;

        /* If using AL_SOFT_buffer_samples, try looking through its formats */
        if(palIsBufferFormatSupportedSOFT)
        {
            /* AL_SOFT_buffer_samples is more lenient with matching formats. The
             * specified sample type does not need to match the returned format,
             * but it is nice to try to get something close. */
            if(type == AL_UNSIGNED_BYTE_SOFT || type == AL_BYTE_SOFT)
            {
                if(channels == AL_MONO_SOFT) format = AL_MONO8_SOFT;
                else if(channels == AL_STEREO_SOFT) format = AL_STEREO8_SOFT;
                else if(channels == AL_QUAD_SOFT) format = AL_QUAD8_SOFT;
                else if(channels == AL_5POINT1_SOFT) format = AL_5POINT1_8_SOFT;
                else if(channels == AL_6POINT1_SOFT) format = AL_6POINT1_8_SOFT;
                else if(channels == AL_7POINT1_SOFT) format = AL_7POINT1_8_SOFT;
            }
            else if(type == AL_UNSIGNED_SHORT_SOFT || type == AL_SHORT_SOFT)
            {
                if(channels == AL_MONO_SOFT) format = AL_MONO16_SOFT;
                else if(channels == AL_STEREO_SOFT) format = AL_STEREO16_SOFT;
                else if(channels == AL_QUAD_SOFT) format = AL_QUAD16_SOFT;
                else if(channels == AL_5POINT1_SOFT) format = AL_5POINT1_16_SOFT;
                else if(channels == AL_6POINT1_SOFT) format = AL_6POINT1_16_SOFT;
                else if(channels == AL_7POINT1_SOFT) format = AL_7POINT1_16_SOFT;
            }
            else if(type == AL_UNSIGNED_BYTE3_SOFT || type == AL_BYTE3_SOFT ||
                type == AL_UNSIGNED_INT_SOFT || type == AL_INT_SOFT ||
                type == AL_FLOAT_SOFT || type == AL_DOUBLE_SOFT)
            {
                if(channels == AL_MONO_SOFT) format = AL_MONO32F_SOFT;
                else if(channels == AL_STEREO_SOFT) format = AL_STEREO32F_SOFT;
                else if(channels == AL_QUAD_SOFT) format = AL_QUAD32F_SOFT;
                else if(channels == AL_5POINT1_SOFT) format = AL_5POINT1_32F_SOFT;
                else if(channels == AL_6POINT1_SOFT) format = AL_6POINT1_32F_SOFT;
                else if(channels == AL_7POINT1_SOFT) format = AL_7POINT1_32F_SOFT;
            }

            if(format != AL_NONE && !palIsBufferFormatSupportedSOFT(format))
                format = AL_NONE;

            /* A matching format was not found or supported. Try 32-bit float. */
            if(format == AL_NONE)
            {
                if(channels == AL_MONO_SOFT) format = AL_MONO32F_SOFT;
                else if(channels == AL_STEREO_SOFT) format = AL_STEREO32F_SOFT;
                else if(channels == AL_QUAD_SOFT) format = AL_QUAD32F_SOFT;
                else if(channels == AL_5POINT1_SOFT) format = AL_5POINT1_32F_SOFT;
                else if(channels == AL_6POINT1_SOFT) format = AL_6POINT1_32F_SOFT;
                else if(channels == AL_7POINT1_SOFT) format = AL_7POINT1_32F_SOFT;

                if(format != AL_NONE && !palIsBufferFormatSupportedSOFT(format))
                    format = AL_NONE;
            }
            /* 32-bit float not supported. Try 16-bit int. */
            if(format == AL_NONE)
            {
                if(channels == AL_MONO_SOFT) format = AL_MONO16_SOFT;
                else if(channels == AL_STEREO_SOFT) format = AL_STEREO16_SOFT;
                else if(channels == AL_QUAD_SOFT) format = AL_QUAD16_SOFT;
                else if(channels == AL_5POINT1_SOFT) format = AL_5POINT1_16_SOFT;
                else if(channels == AL_6POINT1_SOFT) format = AL_6POINT1_16_SOFT;
                else if(channels == AL_7POINT1_SOFT) format = AL_7POINT1_16_SOFT;

                if(format != AL_NONE && !palIsBufferFormatSupportedSOFT(format))
                    format = AL_NONE;
            }
            /* 16-bit int not supported. Try 8-bit int. */
            if(format == AL_NONE)
            {
                if(channels == AL_MONO_SOFT) format = AL_MONO8_SOFT;
                else if(channels == AL_STEREO_SOFT) format = AL_STEREO8_SOFT;
                else if(channels == AL_QUAD_SOFT) format = AL_QUAD8_SOFT;
                else if(channels == AL_5POINT1_SOFT) format = AL_5POINT1_8_SOFT;
                else if(channels == AL_6POINT1_SOFT) format = AL_6POINT1_8_SOFT;
                else if(channels == AL_7POINT1_SOFT) format = AL_7POINT1_8_SOFT;

                if(format != AL_NONE && !palIsBufferFormatSupportedSOFT(format))
                    format = AL_NONE;
            }

            return format;
        }

        /* We use the AL_EXT_MCFORMATS extension to provide output of Quad, 5.1,
         * and 7.1 channel configs, AL_EXT_FLOAT32 for 32-bit float samples, and
         * AL_EXT_DOUBLE for 64-bit float samples. */
        if(type == AL_UNSIGNED_BYTE_SOFT)
        {
            if(channels == AL_MONO_SOFT)
                format = AL_FORMAT_MONO8;
            else if(channels == AL_STEREO_SOFT)
                format = AL_FORMAT_STEREO8;
            else if(alIsExtensionPresent("AL_EXT_MCFORMATS"))
            {
                if(channels == AL_QUAD_SOFT)
                    format = alGetEnumValue("AL_FORMAT_QUAD8");
                else if(channels == AL_5POINT1_SOFT)
                    format = alGetEnumValue("AL_FORMAT_51CHN8");
                else if(channels == AL_6POINT1_SOFT)
                    format = alGetEnumValue("AL_FORMAT_61CHN8");
                else if(channels == AL_7POINT1_SOFT)
                    format = alGetEnumValue("AL_FORMAT_71CHN8");
            }
        }
        else if(type == AL_SHORT_SOFT)
        {
            if(channels == AL_MONO_SOFT)
                format = AL_FORMAT_MONO16;
            else if(channels == AL_STEREO_SOFT)
                format = AL_FORMAT_STEREO16;
            else if(alIsExtensionPresent("AL_EXT_MCFORMATS"))
            {
                if(channels == AL_QUAD_SOFT)
                    format = alGetEnumValue("AL_FORMAT_QUAD16");
                else if(channels == AL_5POINT1_SOFT)
                    format = alGetEnumValue("AL_FORMAT_51CHN16");
                else if(channels == AL_6POINT1_SOFT)
                    format = alGetEnumValue("AL_FORMAT_61CHN16");
                else if(channels == AL_7POINT1_SOFT)
                    format = alGetEnumValue("AL_FORMAT_71CHN16");
            }
        }
        else if(type == AL_FLOAT_SOFT && alIsExtensionPresent("AL_EXT_FLOAT32"))
        {
            if(channels == AL_MONO_SOFT)
                format = alGetEnumValue("AL_FORMAT_MONO_FLOAT32");
            else if(channels == AL_STEREO_SOFT)
                format = alGetEnumValue("AL_FORMAT_STEREO_FLOAT32");
            else if(alIsExtensionPresent("AL_EXT_MCFORMATS"))
            {
                if(channels == AL_QUAD_SOFT)
                    format = alGetEnumValue("AL_FORMAT_QUAD32");
                else if(channels == AL_5POINT1_SOFT)
                    format = alGetEnumValue("AL_FORMAT_51CHN32");
                else if(channels == AL_6POINT1_SOFT)
                    format = alGetEnumValue("AL_FORMAT_61CHN32");
                else if(channels == AL_7POINT1_SOFT)
                    format = alGetEnumValue("AL_FORMAT_71CHN32");
            }
        }
        else if(type == AL_DOUBLE_SOFT && alIsExtensionPresent("AL_EXT_DOUBLE"))
        {
            if(channels == AL_MONO_SOFT)
                format = alGetEnumValue("AL_FORMAT_MONO_DOUBLE");
            else if(channels == AL_STEREO_SOFT)
                format = alGetEnumValue("AL_FORMAT_STEREO_DOUBLE");
        }

        /* NOTE: It seems OSX returns -1 from alGetEnumValue for unknown enums, as
         * opposed to 0. Correct it. */
        if(format == -1)
            format = 0;

        return format;
    }
#endif

    //-------- Static data -------------------------------------------------------

    ALCdevice * Sound3DPlayer::device = NULL;
    ALCcontext * Sound3DPlayer::globalContext = NULL;
    
#if defined(AL_SOFT_buffer_samples) && AL_SOFT_buffer_samples
    static LPALBUFFERSAMPLESSOFT alBufferSamplesSOFT = _wrap_BufferSamples;
    static LPALISBUFFERFORMATSUPPORTEDSOFT alIsBufferFormatSupportedSOFT;
#endif

    //-------- Sound3DPlayer -----------------------------------------------------

    Sound3DPlayer::Sound3DPlayer(StlVector<StlString> const * params)
    {
        //Sound3D::_initSuccess=false;
        StlString openArgs;
        if(params&&!params->empty())
        {
            for(unsigned int i = 0; i < params->size(); i++)
            {
                if(i) openArgs+=" ";
                openArgs += (*params)[i];
            }
            openArgs = "'( ( devices '( " + openArgs + " ) ) )";
            device = alcOpenDevice(openArgs.c_str());
        }

        if(!device)
        {
            device = alcOpenDevice(NULL);
            if(!device)
            {
                cerr << "ERROR: Sound3DPlayer init: unable to open sound3D device !!!" << endl;
                //return(false);
                return;
            }
        }

        globalContext = alcCreateContext(device, NULL);

        if(!globalContext)
        {
            alcCloseDevice(device);
            globalContext = NULL;
            cerr << "ERROR: Sound3DPlayer init: unable to create al context !!!" << endl;
            //return(false);
            return;
        }

        if(alcMakeContextCurrent(globalContext) == ALC_FALSE)
        {
            alcDestroyContext(globalContext);
            alcCloseDevice(device);
            cerr << "ERROR: Sound3DPlayer init: unable to make context current !!!" << endl;
            return;
        }

        if(!alcIsExtensionPresent(alcGetContextsDevice(alcGetCurrentContext()), "ALC_EXT_EFX"))
        {
            alcDestroyContext(globalContext);
            alcCloseDevice(device);
            cerr << "ERROR: Sound3DPlayer init: EFX not supported" << endl;
            return;
        }

#if defined(AL_SOFT_buffer_samples) && AL_SOFT_buffer_samples
        if(alIsExtensionPresent("AL_SOFT_buffer_samples"))
        {
            alIsBufferFormatSupportedSOFT = (LPALISBUFFERFORMATSUPPORTEDSOFT)alGetProcAddress("alIsBufferFormatSupportedSOFT");
            alBufferSamplesSOFT = (LPALBUFFERSAMPLESSOFT)alGetProcAddress("alBufferSamplesSOFT");
        }
#endif

        alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
        //alDistanceModel(AL_EXPONENT_DISTANCE_CLAMPED);

        cout << "Sound3DPlayer Opened \"" << alcGetString(device, ALC_DEVICE_SPECIFIER) << "\"" << endl;
#if 0
        cout << "AL_DOPPLER_FACTOR: " << alGetDouble(AL_DOPPLER_FACTOR) << endl;
        cout << "AL_SPEED_OF_SOUND: " << alGetDouble(AL_SPEED_OF_SOUND) << endl;
        cout << "AL_DISTANCE_MODEL: " << alGetInteger(AL_DISTANCE_MODEL) << endl;
#endif
    }

    Sound3DPlayer::~Sound3DPlayer()
    {
        //Sound3D::_initSuccess=false;
        if(globalContext)
        {
            alcDestroyContext(globalContext);
            globalContext = NULL;
        }
        if(device)
        {
            alcCloseDevice(device);
            device = NULL;
        }
    }

    /* LoadBuffer loads the named audio file into an OpenAL buffer object, and
     * returns the new buffer ID. */
    ALuint URLSound3D::LoadSound(char const * filename)
    {
        ALenum err, format, type, channels;
        ALuint rate, buffer;
        size_t datalen;
        void *data;
        FilePtr audiofile;
        StreamPtr sound;

        /* Open the file and get the first stream from it */
        audiofile = openAVFile(filename);
        sound = getAVAudioStream(audiofile, 0);
        if(!sound)
        {
            cerr << "Could not open audio in " << filename << endl;
            closeAVFile(audiofile);
            return 0;
        }

#if defined(AL_SOFT_buffer_samples) && AL_SOFT_buffer_samples
        /* Get the sound format, and figure out the OpenAL format */
        if(getAVAudioInfo(sound, &rate, &channels, &type) != 0)
        {
            cerr << "Error getting audio info for " << filename << endl;
            closeAVFile(audiofile);
            return 0;
        }
        format = GetFormat(channels, type, alIsBufferFormatSupportedSOFT);
#else
        /* Get the sound format, and figure out the OpenAL format */
        if(getAVAudioInfo(sound, &rate, &format) != 0)
        {
            cerr << "Error getting audio info for " << filename << endl;
            closeAVFile(audiofile);
            return 0;
        }
#endif
        if(format == AL_NONE)
        {
#if defined(AL_SOFT_buffer_samples) && AL_SOFT_buffer_samples
            cerr << "Unsupported audio format (" << ChannelsName(channels) << ", " << TypeName(type) << ") for " << filename << endl;
#else
            cerr << "Unsupported audio format (" << format << ") for " << filename << endl;
#endif
            closeAVFile(audiofile);
            return 0;
        }

        /* Decode the whole audio stream to a buffer. */
        data = decodeAVAudioStream(sound, &datalen);
        if(!data)
        {
            cerr << "Failed to read audio from " << filename << endl;
            closeAVFile(audiofile);
            return 0;
        }

        /* Buffer the audio data into a new buffer object, then free the data and
         * close the file. */
        buffer = 0;
        alGenBuffers(1, &buffer);
#if defined(AL_SOFT_buffer_samples) && AL_SOFT_buffer_samples
        alBufferSamplesSOFT(buffer, rate, format, BytesToFrames(datalen, channels, type),
                channels, type, data);
#else
        alBufferData(buffer, format, data, datalen, rate);
#endif
        free(data);
        closeAVFile(audiofile);

        /* Check if an error occured, and clean up if so. */
        err = alGetError();
        if(err != AL_NO_ERROR)
        {
            cerr << "OpenAL Error (" << err << "): " << alGetString(err) << endl;
            if(alIsBuffer(buffer))
                alDeleteBuffers(1, &buffer);
            return 0;
        }

        return buffer;
    }

    URLSound3D::URLSound3D(StlString const &url) :
        _gain(0),
        _attenuated(false)
    {
        buffer = LoadSound(url.c_str());
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, buffer);
    }

    void URLSound3D::play()
    {
        alSourcePlay(source);
    }

    void URLSound3D::stop()
    {
        alSourceStop(source);
    }

    void URLSound3D::setRepeat(bool loop)
    {
        alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    }

    void URLSound3D::_updatePosition()
    {
        if(_attenuated)
        {
            alSource3f(source, AL_POSITION, _position.x, _position.y, _position.z);
        }
        else
        {
            alSource3f(source, AL_POSITION, 0, 0, 0);
        }
        //cerr << "URLSound Moved To: (" << _position.x << "," << _position.y << "," << _position.z << ")" << endl;
    }

    void URLSound3D::setPosition(float x, float y, float z)
    {
        _position = Vector3(x, y, z);
        _updatePosition();
    }

    void URLSound3D::setPosition(Vector3 const &pos)
    {
        _position = pos;
        _updatePosition();
    }

    void URLSound3D::_updateVelocity()
    {
        alSource3f(source, AL_VELOCITY, _velocity.x, _velocity.y, _velocity.z);
    }

    void URLSound3D::setVelocity(float x, float y, float z)
    {
        _velocity = Vector3(x, y, z);
        _updateVelocity();
    }
    
    void URLSound3D::setVelocity(Vector3 const &vel)
    {
        _velocity = vel;
        _updateVelocity();
    }

    void URLSound3D::setGain(double gain)
    {
        _gain = gain;
        alSourcef(source, AL_GAIN, _gain);
    }

    void URLSound3D::setAttenuated(bool attenuated)
    {
        _attenuated = attenuated;
        alSourcei(source, AL_SOURCE_RELATIVE, _attenuated ? AL_FALSE : AL_TRUE);
        _updatePosition();
    }

    void URLSound3D::setDistanceAttenuation(double minDistance, double maxDistance)
    {
        alSourcef(source, AL_MAX_DISTANCE, maxDistance);
        alSourcef(source, AL_REFERENCE_DISTANCE, minDistance);
    }

    bool URLSound3D::isStopped() const
    {
        int state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        switch(state)
        {
            case AL_PAUSED:
            case AL_PLAYING:
                return false;
            default:
                return true;
        }
    }

    bool URLSound3D::isPaused() const
    {
        int state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
        switch(state)
        {
            case AL_PAUSED:
                return true;
            case AL_PLAYING:
            default:
                return false;
        }
    }

    Listener3D::Listener3D() :
    _position(Vector3(0.0, 0.0, 0.0)),
    _orientation(make_pair<Vector3,Vector3>(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 0.0))),
    _velocity(Vector3(0.0, 0.0, 0.0))
    {
        _updatePosition();
        _updateOrientation();
        _updateVelocity();
    }
    
    void Listener3D::_updatePosition()
    {
        alListener3f(AL_POSITION, _position.x, _position.y, _position.z);
        //cerr << "Listener3D Moved To: (" << _position.x << "," << _position.y << "," << _position.z << ")" << endl;
    }

    void Listener3D::setPosition(float x, float y, float z)
    {
        _position = Vector3(x, y, z);
        _updatePosition();
    }

    void Listener3D::setPosition(Vector3 const &pos)
    {
        _position = pos;
        _updatePosition();
    }
    
    void Listener3D::_updateOrientation()
    {
        ALfloat al_ori[6];
        al_ori[0] = _orientation.first.x;
        al_ori[1] = _orientation.first.y;
        al_ori[2] = _orientation.first.z;
        al_ori[3] = _orientation.second.x;
        al_ori[4] = _orientation.second.y;
        al_ori[5] = _orientation.second.z;
        
        alListenerfv(AL_ORIENTATION, al_ori);
        
        /* cout << "Listener3D Rotate To: ("
            << _orientation.first.x << "," << _orientation.first.y << "," << _orientation.first.z << "),("
            << _orientation.second.x << "," << _orientation.second.y << "," << _orientation.second.z << ")"
            << endl; */
    }

    void Listener3D::setOrientation(float x1, float y1, float z1, float x2, float y2, float z2)
    {
        _orientation = make_pair(Vector3(x1, y1, z1), Vector3(x2, y2, z2));
        _updateOrientation();
    }
    
    void Listener3D::setOrientation(Vector3 const &ori1, Vector3 const &ori2)
    {
        _orientation = make_pair(ori1, ori2);
        _updateOrientation();
    }
    
    void Listener3D::setOrientation(pair<Vector3,Vector3> const &ori)
    {
        _orientation = ori;
        _updateOrientation();
    }
    
    void Listener3D::_updateVelocity()
    {
        alListener3f(AL_VELOCITY, _velocity.x, _velocity.y, _velocity.z);
    }

    void Listener3D::setVelocity(float x, float y, float z)
    {
        _velocity = Vector3(x, y, z);
        _updateVelocity();
    }

    void Listener3D::setVelocity(Vector3 const &vel)
    {
        _velocity = vel;
        _updateVelocity();
    }

    void Listener3D::setGain(double gain)
    {
        _gain = gain;
        alListenerf(AL_GAIN, _gain);
    }
}
