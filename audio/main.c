#include <alsa/asoundlib.h>

#define DEPTH		1
#define CHANNELS	1
#define RATE		48000
#define LATENCY		500000
#define WRITE_FRAMES	(64*1024)

uint8_t buf[DEPTH*CHANNELS*WRITE_FRAMES];

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s audio_file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "could not open \"%s\"\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	int err;
	snd_pcm_t *handle;
	snd_pcm_sframes_t frames;

	err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	err = snd_pcm_set_params(handle, SND_PCM_FORMAT_U8,
		SND_PCM_ACCESS_RW_INTERLEAVED, CHANNELS, RATE, 1, LATENCY);
	if (err)
		fprintf(stderr, "playback setup failed: %s\n",
			snd_strerror(err));

	ssize_t rb = read(fd, buf, sizeof buf);
	while (rb) {
		frames = snd_pcm_writei(handle, buf, rb);
		if (frames < 0) {	/* if error */
			frames = snd_pcm_recover(handle, frames, 0);
			if (frames < 0) {
				fprintf(stderr,
					"snd_pcm_recover failed: %s\n",
					snd_strerror(frames));
				exit(EXIT_FAILURE);
			}
		}
		rb = read(fd, buf, sizeof buf);
	}
	/* pass the ramaining samples, then close */
	snd_pcm_drain(handle);
	snd_pcm_close(handle);
}
