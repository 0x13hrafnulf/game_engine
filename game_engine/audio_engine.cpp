#include "audio_engine.h"
#include "errors.h"

namespace game_engine {

	void sound_effect::play(int loops)
	{
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1)
		{
			if (Mix_PlayChannel(0, m_chunk, loops) == -1)
			fatalError("Mix play sound effect error: " + std::string(Mix_GetError()));
		}
	}
	void music::play(int loops)
	{
		if (Mix_PlayMusic(m_music, loops))
		{
			fatalError("Mix play music error: " + std::string(Mix_GetError()));

		}
	}
	void music::pause()
	{
		Mix_PauseMusic();
	}
	void music::stop()
	{
		Mix_HaltMusic();
	}
	void music::resume()
	{
		Mix_ResumeMusic();
	}


	audio_engine::audio_engine()
	{
	}


	audio_engine::~audio_engine()
	{
		destroy();
	}

	void audio_engine::init()
	{
		if (m_isInitialized)
		{
			fatalError("Tried to init audio engine again");

		}

		//bitwise combination mix_init_fac, mix_init_mod, mix_init_mp3, mix_init_ogg
		if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == - 1)
		{
			fatalError("Mix init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			fatalError("Mix init error: " + std::string(Mix_GetError()));

		}
		m_isInitialized = true;
	}
	void audio_engine::destroy()
	{
		if (m_isInitialized)
		{
			for (auto& it : m_effectMap)
			{
				Mix_FreeChunk(it.second);
			}
			for (auto& it : m_musictMap)
			{
				Mix_FreeMusic(it.second);
			}
			m_effectMap.clear();
			m_musictMap.clear();

			Mix_CloseAudio();
			m_isInitialized = false;
			Mix_Quit();
		}
	}

	sound_effect audio_engine::loadSoundEffect(const std::string& filePath)
	{
		auto it = m_effectMap.find(filePath);

		sound_effect effect;
		if (it == m_effectMap.end())
		{
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr)
			{
				fatalError("MixLoadWAV error: " + std::string(Mix_GetError()));
			}
			effect.m_chunk = chunk;
			m_effectMap[filePath] = chunk;
		}
		else
		{
			effect.m_chunk = it->second;
		}

		return effect;
	}
	music audio_engine::loadMusic(const std::string& filePath)
	{
		auto it = m_musictMap.find(filePath);

		music effect;
		if (it == m_musictMap.end())
		{
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (mixMusic == nullptr)
			{
				fatalError("MixLoadMUS error: " + std::string(Mix_GetError()));
			}
			effect.m_music = mixMusic;
			m_musictMap[filePath] = mixMusic;
		}
		else
		{
			effect.m_music = it->second;
		}

		return effect;
	}

}