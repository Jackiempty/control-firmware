/*
----------------------------------------------------------------------
File    : control.h
Purpose : Implementation the vehicle control loop.
Revision: $Rev: 2024.11$
----------------------------------------------------------------------
*/

#ifndef CONTROL_H
#define CONTROL_H

#include "tx_api.h"

#ifdef DEBUG
#include "SEGGER_RTT.h"
#define CONTROL_DEBUG(...) SEGGER_RTT_printf(0, __VA_ARGS__)
#else
#define CONTROL_DEBUG(...)
#endif

typedef enum {
  CONTROL_STOPPED,
  CONTROL_CALIBRATE,
  CONTROL_RTD,
  CONTROL_RUNNING,
} control_state_t;

void control_thread_entry(ULONG thread_input);

#endif
