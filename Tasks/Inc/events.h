/*
----------------------------------------------------------------------
File    : events.h
Purpose : Define ALL posiable events in tasks for RTOS.
Revision: $Rev: 2023.49$
----------------------------------------------------------------------
*/

#ifndef _EVENTS_H_
#define _EVENTS_H_

#define EVENT_BIT(x) (1 << (x))

enum {
  EVENT_FS_INIT = 0,
  EVENT_CONFIG_LOADED,
  EVENT_LOGGING,
  EVENT_PRECHARGE,
};

#endif
