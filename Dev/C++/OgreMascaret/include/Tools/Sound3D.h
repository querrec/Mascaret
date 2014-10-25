#ifndef __SOUND_3D_H__
#define __SOUND_3D_H__
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <string>
#include <vector>
#include <utility>
#include "ogreMascaret_plateform.h"
#include "VEHA/Entity/Vector3.h"

namespace Sound
{
    typedef std::string StlString;
    #define StlVector std::vector
    using std::pair;
    using VEHA::Vector3;

    //-------- Helpers functions -------------------------------------------------

#if defined(AL_SOFT_buffer_samples) && AL_SOFT_buffer_samples
    /* Loads samples into a buffer using the standard alBufferData call, but with a
     *  LPALBUFFERSAMPLESSOFT-compatible prototype. Assumes internalformat is valid
     *  for alBufferData, and that channels and type match it. */
    void AL_APIENTRY _wrap_BufferSamples(ALuint buffer, ALuint samplerate,
        ALenum internalformat, ALsizei samples, ALenum channels, ALenum type,
        const ALvoid *data);

    const char *ChannelsName(ALenum chans);
    const char *TypeName(ALenum type);

    ALsizei FramesToBytes(ALsizei size, ALenum channels, ALenum type);
    ALsizei BytesToFrames(ALsizei size, ALenum channels, ALenum type);

    /* GetFormat retrieves a compatible buffer format given the channel config and
     * sample type. If an alIsBufferFormatSupportedSOFT-compatible function is
     * provided, it will be called to find the closest-matching format from
     * AL_SOFT_buffer_samples. Returns AL_NONE (0) if no supported format can be
     * found. */
    ALenum GetFormat(ALenum channels, ALenum type, LPALISBUFFERFORMATSUPPORTEDSOFT palIsBufferFormatSupportedSOFT);
#endif

    class OGRE_MASCARET_API Sound3DPlayer
    {
        public:
            Sound3DPlayer(StlVector<StlString> const * params = (const StlVector<StlString>*)0);
            ~Sound3DPlayer();
        protected:
            static ALCdevice * device;
            static ALCcontext * globalContext;
    };

    class OGRE_MASCARET_API URLSound3D
    {
        public:
            static ALuint LoadSound(char const * filename);
            URLSound3D(StlString const &url);
            void play();
            void stop();

            void setRepeat(bool loop);
            void setPosition(float x, float y, float z);
            void setPosition(Vector3 const &pos);
            void setVelocity(float x, float y, float z);
            void setVelocity(Vector3 const &pos);
            void setGain(double gain);
            void setAttenuated(bool attenuated);
            void setDistanceAttenuation(double minDistance, double maxDistance);

            Vector3 getPosition() const;
            Vector3 getVelocity() const;
            double getGain() const;
            bool getAttenuated() const;

            bool isStopped() const;
            bool isPaused() const;
        protected:
            void _updatePosition();
            void _updateVelocity();
            ALuint buffer;
            ALuint source;
            ALenum state;
            Vector3 _position;
            Vector3 _velocity;
            double _gain;
            bool _attenuated;
    };

    class OGRE_MASCARET_API Listener3D
    {
        public:
            Listener3D();

            void setPosition(float x, float y, float z);
            void setPosition(Vector3 const &pos);
            void setOrientation(float x1, float y1, float z1, float x2, float y2, float z2);
            void setOrientation(Vector3 const &ori1, Vector3 const &ori2);
            void setOrientation(pair<Vector3, Vector3> const &ori);
            void setVelocity(float x, float y, float z);
            void setVelocity(Vector3 const &pos);
            void setGain(double gain);

            Vector3 getPosition() const;
            pair<Vector3, Vector3> getOrientation() const;
            Vector3 getVelocity() const;
            double getGain() const;
        protected:
            void _updatePosition();
            void _updateOrientation();
            void _updateVelocity();
            Vector3 _position;
            pair<Vector3, Vector3> _orientation;
            Vector3 _velocity;
            double _gain;
    };

    inline Vector3 URLSound3D::getPosition() const
    {
        return _position;
    }
    inline Vector3 URLSound3D::getVelocity() const
    {
        return _velocity;
    }
    inline double URLSound3D::getGain() const
    {
        return _gain;
    }
    inline bool URLSound3D::getAttenuated() const
    {
        return _attenuated;
    }

    inline Vector3 Listener3D::getPosition() const
    {
        return _position;
    }
    inline pair<Vector3, Vector3> Listener3D::getOrientation() const
    {
        return _orientation;
    }
    inline Vector3 Listener3D::getVelocity() const
    {
        return _velocity;
    }
    inline double Listener3D::getGain() const
    {
        return _gain;
    }
}
#endif                           /* #ifndef __SOUND_3D_H__ */
