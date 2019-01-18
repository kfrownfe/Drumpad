/*
  *Drumpad
  *ncurses implementation of a drum machine and MIDI sequencer using fluidsynth
  *(2018), Kyle Frownfelter <ksf@fastmail.fm>
  *Released under the terms of the GNU General Public License*/


#include <fluidsynth.h>
#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_KEYS 47

struct key_type {
  char key;
  int note;};

struct key_type key_table[]={
  {'`',70},
  {'a',46},
  {'b',40},
  {'c',38},
  {'d',48},
  {'e',59},
  {'f',49},
  {'g',50},
  {'h',51},
  {'i',64},
  {'j',52},
  {'k',53},
  {'l',54},
  {'m',42},
  {'n',41},
  {'o',65},
  {'p',66},
  {'q',57},
  {'r',60},
  {'s',47},
  {'t',61},
  {'u',63},
  {'v',39},
  {'w',58},
  {'x',37},
  {'y',62},
  {'z',36},
  {'[',67},
  {'\\',69},
  {']',68},
  {'\'',56},
  {',',43},
  {'-',81},
   {'.',44},
  {'/',45},
  {'0',80},
  {'1',71},
  {'2',72},
  {'3',73},
  {'4',74},
  {'5',75},
  {'6',76},
  {'7',77},
  {'8',78},
  {'9',79},
  {';',55},
  {'=',82},
  };

int key_hash(int keycode) ;

int main (int argc, char **argv) {
  int program=0;
float gain=1.0;
  initscr();
  noecho();
  keypad(stdscr,true);
  raw();
   if (argc==1) {printw("Usage: %s <sound font>", argv[0]);refresh();endwin();return 1;}
  int velocity=100;
  fluid_settings_t *settings=new_fluid_settings();
		fluid_synth_t *synth;
  fluid_audio_driver_t *driver;
		fluid_settings_setstr(settings,"audio.driver","alsa");
  fluid_settings_setnum(settings,"synth.gain",gain);
		synth=new_fluid_synth(settings);
  driver = new_fluid_audio_driver(settings,synth);
  fluid_synth_sfload(synth,argv[1],1);
  while (1){
    int ch=getch();
    if (ch==KEY_LEFT) { if (gain==0.0) continue; else { gain-=0.5;  
fluid_settings_setnum(settings,"synth.gain",gain);printw("Gain set to %f.\n",gain);refresh();continue;}}
    else if (ch==KEY_RIGHT) { if (gain==10.0) continue; else { gain+=0.5;  
fluid_settings_setnum(settings,"synth.gain",gain);printw("Gain set to %f.\n",gain);refresh();continue;}}
    else if (ch==KEY_BACKSPACE) break;
    else if (ch==KEY_DOWN) { if (program==128) continue;else { 
program+=1;fluid_synth_program_change(synth,0,program);printw("Changed to program %d.\n",program);refresh();continue;}}
    else if (ch==KEY_UP) { if (program==0) continue; else { 
program-=1;fluid_synth_program_change(synth,0,program);printw("Program changed to %d.\n",program);refresh();continue;}}
    else if (ch==32) {   fluid_settings_setnum(settings,"synth.gain",0.0);usleep(100000);   
fluid_settings_setnum(settings,"synth.gain",gain);printw("Silenced.");refresh();continue;}
    int hash=key_hash(ch);
    if (hash==-1) continue; else fluid_synth_noteon(synth,0,key_table[hash].note,velocity);
    }
  delete_fluid_audio_driver(driver);
  delete_fluid_synth(synth);
  delete_fluid_settings(settings);
  endwin();
  sleep(1);
  return 0;
}

int key_hash(int keycode) {
  if (keycode>=96) return (keycode%96);
  else if (keycode>=91) return ((keycode%91)+27);
  else if (keycode>=39 && keycode<=61) {
    if (keycode==39) return 30;
    else if (keycode==59) return 45;
    else if (keycode==61) return 46;
    else return ((keycode%44)+31);}
  else return -1;}
