#include "Music.h"

#define OPEN_MUSIC

void play_game_music(void)
{
#ifdef OPEN_MUSIC
	PlaySound(TEXT(".//music/play.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
#endif // OPEN_MUSIC

}

void play_end(void)
{
#ifdef OPEN_MUSIC
	PlaySound(NULL, NULL, SND_FILENAME | SND_ASYNC);
#endif // OPEN_MUSIC

}

void end_game_music(void)
{
#ifdef OPEN_MUSIC
	PlaySound(TEXT("../music/end.wav"), NULL, SND_FILENAME | SND_ASYNC);
#endif // OPEN_MUSIC

}
