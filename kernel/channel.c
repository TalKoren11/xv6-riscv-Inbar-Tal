#include "types.h"
#include "defs.h"
#include "param.h"
#include "proc.h"
#include "spinlock.h"
#include "channel.h"
#include "riscv.h"
#include "memlayout.h"

// Global array of channels
struct channel channels[NCHANNEL];

// Initialize channels
void channel_init(void) {
  for (int i = 0; i < NCHANNEL; i++) {
    initlock(&channels[i].lock, "channel");
    channels[i].valid = 0;
    channels[i].has_data = 0;
    channels[i].owner = 0;
  }
}

// Create a new channel
int channel_create(void) {
  struct proc *p = myproc();
  for (int i = 0; i < NCHANNEL; i++) {
    acquire(&channels[i].lock);
    if (!channels[i].valid) {
      channels[i].valid = 1;
      channels[i].owner = p;
      release(&channels[i].lock);
      return i;
    }
    release(&channels[i].lock);
  }
  return -1; // No available channel
}

// Put data into the channel
int channel_put(int cd, int data) {
  if (cd < 0 || cd >= NCHANNEL) return -1;

  struct channel *ch = &channels[cd];
  acquire(&ch->lock);
  if (!ch->valid) {
    release(&ch->lock);
    return -1;
  }

  while (ch->has_data) {
    sleep(ch, &ch->lock);
    if (!ch->valid) {
      release(&ch->lock);
      return -1;
    }
  }

  ch->data = data;
  ch->has_data = 1;
  wakeup(ch);
  release(&ch->lock);
  return 0;
}

// Take data from the channel
int channel_take(int cd, int *data) {
  if (cd < 0 || cd >= NCHANNEL || data == 0) return -1;

  struct channel *ch = &channels[cd];
  acquire(&ch->lock);
  if (!ch->valid) {
    release(&ch->lock);
    return -1;
  }

  while (!ch->has_data) {
    sleep(ch, &ch->lock);
    if (!ch->valid) {
      release(&ch->lock);
      return -1;
    }
  }

  if (copyout(myproc()->pagetable, (uint64)data, (char *)&ch->data, sizeof(int)) < 0) {
    release(&ch->lock);
    return -1;
  }

  ch->has_data = 0;
  wakeup(ch);
  release(&ch->lock);
  return 0;
}

// Destroy a channel
int channel_destroy(int cd) {
  if (cd < 0 || cd >= NCHANNEL) return -1;

  struct channel *ch = &channels[cd];
  acquire(&ch->lock);
  if (!ch->valid) {
    release(&ch->lock);
    return -1;
  }

  ch->valid = 0;
  wakeup(ch);
  release(&ch->lock);
  return 0;
}

// Cleanup channels on process exit
void channel_cleanup(struct proc *p) {
  for (int i = 0; i < NCHANNEL; i++) {
    acquire(&channels[i].lock);
    if (channels[i].valid && channels[i].owner == p) {
      channels[i].valid = 0;
      wakeup(&channels[i]);
    }
    release(&channels[i].lock);
  }
}
