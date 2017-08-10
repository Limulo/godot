#ifndef PD_PLAYER_H
#define PD_PLAYER_H

#include "scene/main/node.h"
#include "scene/resources/audio_stream.h"
extern "C"{
	#include "z_libpd.h"
}

class PDPlayer : public Node {

	OBJ_TYPE(PDPlayer, Node);

	struct InternalStream : public AudioServer::AudioStream
	{
		PDPlayer *player;
		virtual int get_channel_count() const;
		virtual void set_mix_rate(int p_rate); //notify the stream of the mix rate
		virtual bool mix(int32_t *p_buffer, int p_frames);
		virtual void update();
	};

	InternalStream internal_stream;
	void* patch_handle;
	bool _is_playing;
	const int input_channels = 1;
	const int output_channels = 2;

	int sp_get_channel_count() const;
	void sp_set_mix_rate(int p_rate); //notify the stream of the mix rate
	void sp_update();
	bool sp_mix(int32_t* p_buffer, int p_frames);

	int server_mix_rate;
	float inbuf[4096];
	float outbuf[4096];

	RID stream_rid;
	float volume;
	int32_t block_size;

	void GDString2char(const String &in, char* c, int l); /* string conversion */

protected:
		static void _bind_methods();

public:
		void play();
		void stop();
		bool is_playing() const;
		void set_volume(float p_vol);
		float get_volume() const;
		void set_volume_db(float p_db);
		float get_volume_db() const;
		bool load_patch(const String &patch, const String &folder);
		void unload_patch();
		PDPlayer();
		~PDPlayer();
};
#endif
