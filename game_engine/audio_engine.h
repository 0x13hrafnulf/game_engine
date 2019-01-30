#pragma once

#include <SDL_MIXER/SDL_mixer.h>
#include <string>
#include <map>


namespace game_engine {

	class sound_effect
	{
	public:
		friend class audio_engine;

		void play(int loops = 0);

		

	private:

		Mix_Chunk* m_chunk = nullptr;


	};

	class music
	{
	public:
		friend class audio_engine;
		void play(int loops = 1); // -1 play forever, otherwise play loops time
		static void pause();
		static void stop();
		static void resume();
	private:
		Mix_Music* m_music = nullptr;

	};




	class audio_engine
	{
	public:
		audio_engine();
		~audio_engine();

		void init();
		void destroy();

		sound_effect loadSoundEffect(const std::string& filePath);
		music loadMusic(const std::string& filePath);

	private:
		std::map<std::string, Mix_Chunk*> m_effectMap;
		std::map<std::string, Mix_Music*> m_musictMap;


		bool m_isInitialized;

	};

}