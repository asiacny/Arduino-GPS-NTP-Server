#ifndef __HWDEP_H
#define __HWDEP_H


#ifdef SIMULATE

#include <stdio.h>

#define CPU_CLOCK 16000000L

#define debug(x) printf("%s", x)
#define debug_int(x) printf("%d", x)
#define debug_long(x) printf("%d", (signed int)x)
#define debug_ulong(x) printf("%u", (unsigned int)x)
#define debug_float(x) printf("%f", x)

typedef signed int int32;
typedef unsigned int uint32;

struct _timer_struct {
  unsigned char prediv;
  unsigned char prediv_count;
  unsigned short top;
  unsigned short counter;
  unsigned short capture;
};

extern struct _timer_struct timer;

extern void sim_clk();

inline unsigned short timer_get_counter () {
  return timer.counter;
}

inline unsigned short timer_get_capture () {
  return timer.capture;
}

inline char timer_get_pending () {
  return 0;
}

inline int gps_can_read() {
  return 0; /* XXX not implemented */
}

inline int gps_read() {
  return -1; /* XXX not implemented */
}

inline void gps_write(const char * data) {
  return; /* XXX not implemented */
}

inline void gps_writebyte(const char ch) {
  return; /* XXX not implemented */
}

inline void gps_set_baud(int baud) {
  return; /* Does nothing */
}

inline void delay(int millis) {
  return; /* Does nothing */
}

#else

#include "WProgram.h"
#include "HardwareSerial.h"

#define CPU_CLOCK 16000000L

#if 1
#define debug(x) Serial.print(x)
#define debug_int(x) Serial.print(x)
#define debug_long(x) Serial.print(x)
#define debug_float(x) Serial.print(x)
#else
#define debug(x) do { } while(0)
#define debug_int(x) do { } while(0)
#define debug_long(x) do { } while(0)
#define debug_float(x) do { } while(0)
#endif

typedef long int int32;
typedef unsigned long int uint32;

inline unsigned short timer_get_counter () {
  return TCNT4;
}

inline unsigned short timer_get_capture () {
  return ICR4;
}

inline char timer_get_pending () {
  return ((TIFR4 & _BV(OCF4A)) ? 1 : 0);
}

#define GPSPORT Serial1

inline int gps_can_read() {
  return GPSPORT.available();
}

inline int gps_read() {
  return GPSPORT.read();
}

inline void gps_write(const char *data) {
  Serial.print(data);
  GPSPORT.print(data);
}

inline void gps_writebyte(const char ch) {
  Serial.print(ch, BYTE);
  GPSPORT.print(ch, BYTE);
}

inline void gps_set_baud(long baud) {
  GPSPORT.flush();
  delay(500);
  GPSPORT.begin(baud);
  GPSPORT.flush();
}


#endif

#include "timing.h"

#define DEF_TIMER_VAL 62500
#define MAX_TIMER_VAL 65535

#define PREDIV 8
#define NS_PER_COUNT (1000000000L / (CPU_CLOCK / PREDIV))

// adding tm/2 here biases by 1/2 so it rounds :)
#define NSPC(tm) ((tm / 2 + NS_PER_COUNT * DEF_TIMER_VAL) / tm)
#define NSPI(tm) (1000000000L / ((CPU_CLOCK / PREDIV) / tm))
#define NS_PER_INT NSPI(DEF_TIMER_VAL)
#define INT_PER_SEC ((CPU_CLOCK / PREDIV) / DEF_TIMER_VAL)

extern void timer_init();
extern void timer_set_interval(unsigned short);
extern unsigned short timer_get_interval();

#endif

