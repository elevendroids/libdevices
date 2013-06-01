#ifndef TIMER_H
#define TIMER_H

//
void Timer_Start(void);

//
void Timer_Stop(void);

// Low power sleep for given number of ms
void Timer_Sleep(unsigned interval);

// Return timer ticks in ms
unsigned Timer_GetTicks(void);

#endif
