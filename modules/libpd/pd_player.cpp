#include <cstdlib>
#include "pd_player.h"

int PDPlayer::InternalStream::get_channel_count() const {
	return player->sp_get_channel_count();
}

int PDPlayer::sp_get_channel_count() const {
	//return playback->get_channels();
	return output_channels; /*TODO check audio stream playback class!!!*/
}

void PDPlayer::InternalStream::set_mix_rate(int p_rate) {
	return player->sp_set_mix_rate(p_rate);
}

void PDPlayer::sp_set_mix_rate(int p_rate) {
	server_mix_rate = p_rate;
}

void PDPlayer::InternalStream::update() {
	//printf("\tupdate()\n");
	player->sp_update();
}

void PDPlayer::sp_update() {

	//printf("\tsp_update() - do nothing\n");

	//_THREAD_SAFE_METHOD_
	/*if (!paused && resampler.is_ready() && playback.is_valid()) {

		if (!playback->is_playing()) {
			//stream depleted data, but there's still audio in the ringbuffer
			//check that all this audio has been flushed before stopping the stream
			int to_mix = resampler.get_total() - resampler.get_todo();
			if (to_mix == 0) {
				if (!stop_request) {
					stop_request = true;
					call_deferred("_do_stop");
				}
				return;
			}

			return;
		}

		int todo = resampler.get_todo();
		int wrote = playback->mix(resampler.get_write_buffer(), todo);
		resampler.write(wrote);
	}*/
	return;
}

bool PDPlayer::InternalStream::mix(int32_t *p_buffer, int p_frames) {
	printf("mix()\n");
	return player->sp_mix(p_buffer, p_frames);
}

bool PDPlayer::sp_mix(int32_t *p_buffer, int p_frames) {

	// size of buffers: # of channels * # of ticks * # of samples per tick (blocksize).
  int todo_ticks = p_frames / block_size;
	int outbuf_size = output_channels * block_size * todo_ticks;
	printf( "sp_mix() - %d; output_size: %d\n", p_frames, outbuf_size );

	//int r = libpd_process_float( todo_ticks, inbuf, p_buffer );
  int r = libpd_process_float( todo_ticks, inbuf, outbuf );
	//printf("r: %d\n", r);
	if( r==0 )
	{
		for(int i=0; i<outbuf_size; i++) {
			//p_buffer[i] = Math::fast_ftoi( outbuf[i] );
			p_buffer[i] = outbuf[i] * 2147483647;
			//printf("\tbuffers: [%d]\t<--\t[%f]\n", p_buffer[i], outbuf[i]);
		}
		return true;
	}
	return false;

	/*if (resampler.is_ready() && !paused) {
		return resampler.mix(p_buffer, p_frames);
	}*/
}

// this method is called from GDScripts
void PDPlayer::play() {
	printf("\tplay()\n");
	ERR_FAIL_COND( !is_inside_tree() );

	if ( is_playing() )
		stop();

	//_THREAD_SAFE_METHOD_

	//feed the ringbuffer as long as no update callback is going on
	//sp_update();

	AudioServer::get_singleton()->stream_set_active(stream_rid, true);
	AudioServer::get_singleton()->stream_set_volume_scale(stream_rid, volume);
	//	if (stream->get_update_mode()!=AudioStream::UPDATE_NONE)
	//		set_idle_process(true);

	for( int i=0; i<4096; i++) {
		inbuf[i] = 0.0f;
		outbuf[i] = 0.0f;
	}

	_is_playing = true;

	// [pd dsp 1(
	libpd_start_message(1);
	libpd_add_float(1.0f);
	libpd_finish_message("pd", "dsp");

	return;
}

void PDPlayer::stop() {
	printf("\tstop()\n");
	if ( !is_inside_tree() )
		return;
	if ( !is_playing() )
		return;

	//_THREAD_SAFE_METHOD_
	AudioServer::get_singleton()->stream_set_active(stream_rid, false);

	_is_playing = false;

	// [pd dsp 0(
	libpd_start_message(1);
	libpd_add_float(0.0f);
	libpd_finish_message("pd", "dsp");

	return;
}

bool PDPlayer::is_playing() const {
	return _is_playing;
}

/* ******************************************************************* */

void PDPlayer::set_volume(float p_vol) {
	volume = p_vol;
	if (stream_rid.is_valid())
		AudioServer::get_singleton()->stream_set_volume_scale(stream_rid, volume);
}

float PDPlayer::get_volume() const {

	return volume;
}

void PDPlayer::set_volume_db(float p_db) {
	if (p_db < -79)
		set_volume(0);
	else
		set_volume(Math::db2linear(p_db));
}

float PDPlayer::get_volume_db() const {
	if (volume == 0)
		return -80;
	else
		return Math::linear2db(volume);
}

void PDPlayer::_bind_methods() {

	ObjectTypeDB::bind_method(_MD("play"), &PDPlayer::play);
	ObjectTypeDB::bind_method(_MD("stop"), &PDPlayer::stop);

	ObjectTypeDB::bind_method(_MD("load_patch", "patch", "folder"), &PDPlayer::load_patch);
	ObjectTypeDB::bind_method(_MD("unload_patch"), &PDPlayer::unload_patch);

	ObjectTypeDB::bind_method(_MD("is_playing"), &PDPlayer::is_playing);

	ObjectTypeDB::bind_method(_MD("set_volume", "volume"), &PDPlayer::set_volume);
	ObjectTypeDB::bind_method(_MD("get_volume"), &PDPlayer::get_volume);

	ObjectTypeDB::bind_method(_MD("set_volume_db", "db"), &PDPlayer::set_volume_db);
	ObjectTypeDB::bind_method(_MD("get_volume_db"), &PDPlayer::get_volume_db);

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "stream/volume_db", PROPERTY_HINT_RANGE, "-80,24,0.01"), _SCS("set_volume_db"), _SCS("get_volume_db"));
}

/* ******************************************************************* */
void PDPlayer::GDString2char(const String &in, char *c, int l) {
	/* string conversion stuff **********/
	char mbs[l+1];
	int r = std::wcstombs( mbs, in.c_str(), l );
	printf( "r: %d\t; mbs: %s\n", r, mbs );
	for(int i=0; i <= l; i++)
  	c[i] = mbs[i];
}


bool PDPlayer::load_patch(const String &patch, const String &folder){
	libpd_init();
	if (libpd_init_audio( input_channels, output_channels, server_mix_rate ) )
		printf("LIBPD error: couldn't init audio\n");
	else
	{
		printf("LIBPD audio on!\n");
	}
	block_size = (int32_t)libpd_blocksize(); // default 64

	/* TODO: find a useful way to load patch via GDScripts */
	int l = patch.length();
	char p[l+1];
	GDString2char(patch, p, l);
	l = folder.length();
	char f[l+1];
	GDString2char(folder,f, l);

	patch_handle = libpd_openfile( 	p, f );
	if(patch_handle==NULL)
	{
		print_line("LIBPD error: couldn't open patch file.\n");
		return false;
	}
	return true;
}

void PDPlayer::unload_patch() {
	if( patch_handle ) {
		print_line("LIBPD: closing patch file\n");
		libpd_closefile( patch_handle );
	}
}

/* ******************************************************************* */

PDPlayer::PDPlayer() {
	volume = 1;
	server_mix_rate = 1;
	internal_stream.player = this;
	stream_rid = AudioServer::get_singleton()->audio_stream_create(&internal_stream);
	_is_playing = false;
}

PDPlayer::~PDPlayer() {
	AudioServer::get_singleton()->free(stream_rid);
}
