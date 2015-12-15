#ifndef PTPDALARMS_H_
#define PTPDALARMS_H_

/*-
 * Copyright (c) 2015      Wojciech Owczarek,
 *
 * All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    alarms.h
 * @authors Wojciech Owczarek
 * @date   Wed Dec 9 19:13:10 2015
 * Data type and function definitions related to
 * handling raising and clearing alarms.
 */

#include "../datatypes.h"

#define DOMAIN_MISMATCH_MIN 10	/* trigger domain mismatch alarm after at least 10 mismatches */
#define ALARM_UPDATE_INTERVAL 1 /* how often we process alarms */
#define ALARM_TIMEOUT_PERIOD 30	/* minimal alarm age to clear */

typedef enum {
	ALRM_PORT_STATE = 0,		/*x done*/
	ALRM_OFM_THRESHOLD,
	ALRM_OFM_SECONDS, 		/*x done*/
	ALRM_CLOCK_STEP,		/*x done*/
	ALRM_NO_SYNC,			/*x done*/
	ALRM_NO_DELAY, 			/*x done*/
	ALRM_MASTER_CHANGE, 		/*x done*/
	ALRM_NETWORK_FLT,		/*x done*/
	ALRM_FAST_ADJ,			/*+/- currently only at maxppb */
	ALRM_TIMEPROP_CHANGE,		/*x done*/
	ALRM_DOMAIN_MISMATCH, 		/*+/- currently only when all packets come from an incorrect domain */
	ALRM_MAX
} AlarmType;


typedef enum {
	ALARM_UNSET = 0,		/* idle */
	ALARM_SET,		/* condition has been triggerd */
	ALARM_CLEARED		/* condition has cleared */
} AlarmState;

typedef struct {
	uint8_t id; 			/* alarm ID */
	Boolean eventOnly;		/* this is only an event - don't manage state, just dispatch/inform when condition is met */
	uint32_t age;			/* age of alarm in current state (seconds) */
	AlarmState state;		/* state of the alarm */
	Boolean condition;		/* is the alarm condition met? (so we can check conditions and set alarms separately */
	TimeInternal timeSet;		/* time when set */
	TimeInternal timeCleared;	/* time when cleared */
	Boolean internalOnly;		/* do not display in status file / indicate that the alarm is internal only */
	PtpEventData eventData;		/* the event data union - so we can capture any data we need without the need to capture a whole PtpClock */
} AlarmData;

typedef struct {
	char shortName[5];		/* short code i.e. OFS, DLY, SYN, FLT etc. */
	char name[31];			/* full name i.e. OFFSET_THRESHOLD, NO_DELAY, NO_SYNC etc. */
	char description[101];		/* text description */
	Boolean enabled;		/* is the alarm operational ? */
	AlarmData data;			/* alarm data container (so it's easier to do a static initialisation */
} AlarmEntry;

void initAlarms(AlarmEntry* alarms, int count); 					/* fill an array with initial alarm data */
void updateAlarms(AlarmEntry *alarms, int count);					/* dispatch alarms: age, set, clear alarms etc. */
void displayAlarms(AlarmEntry *alarms, int count);					/* display a formatted alarm summary table */
int  getAlarmSummary(char * output, int size, AlarmEntry *alarms, int count);		/* produce a one-line alarm summary string */


#endif /*PTPDALARMS_H_*/
