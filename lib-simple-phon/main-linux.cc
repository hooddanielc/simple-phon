#include <iostream>
#include <alsa/asoundlib.h>
#include <phon.h>

void phon::list_devices() {
  snd_ctl_t *handle;
  snd_ctl_card_info_t *info;
  snd_ctl_card_info_alloca(&info);
  int card_idx = 0;
  int err;

  if (snd_card_next(&card_idx) < 0) {
    std::cout << "Error trying to determine next audio card" << std::endl;
  }

  while (card_idx > 0) {
    std::cout << "=============" << std::endl;
    std::cout << "= Card #" << card_idx << std::endl;

    char name[32];
    sprintf(name, "hw:%d", card_idx);

    if ((err = snd_ctl_open(&handle, name, 0)) < 0) {
      std::cout << snd_strerror(err) << std::endl;
    } else if ((err = snd_ctl_card_info(handle, info)) < 0) {
      std::cout << snd_strerror(err) << std::endl;
    } else {
      std::cout << "  * Found Card " << name << std::endl; 
    }

    snd_card_next(&card_idx);
  }
}

int main (int, char*[]) {
  phon::list_devices();
  return 0;
}
