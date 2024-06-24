#ifndef CHANNEL_H
#define CHANNEL_H

#include "types.h"
#include "spinlock.h"

#define NCHANNEL 16

// Channel structure
struct channel {
  struct spinlock lock; 
  int valid;            
  int data;             
  int has_data;         
  struct proc *owner;   
};

// Function prototypes
int channel_create(void);
int channel_put(int cd, int data);
int channel_take(int cd, int *data);
int channel_destroy(int cd);

#endif