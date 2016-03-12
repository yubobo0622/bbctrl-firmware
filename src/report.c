/******************************************************************************\

                   This file is part of the TinyG firmware.

                     Copyright (c) 2016, Buildbotics LLC
                             All rights reserved.

        The C! library is free software: you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public License
        as published by the Free Software Foundation, either version 2.1 of
        the License, or (at your option) any later version.

        The C! library is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
        Lesser General Public License for more details.

        You should have received a copy of the GNU Lesser General Public
        License along with the C! library.  If not, see
        <http://www.gnu.org/licenses/>.

        In addition, BSD licensing may be granted on a case by case basis
        by written permission from at least one of the copyright holders.
        You may request written permission by emailing the authors.

                For information regarding this software email:
                               Joseph Coffland
                            joseph@buildbotics.com

\******************************************************************************/

#include "report.h"
#include "config.h"
#include "canonical_machine.h"
#include "usart.h"
#include "rtc.h"
#include "vars.h"

#include "plan/planner.h"

#include <avr/pgmspace.h>

#include <stdio.h>
#include <stdbool.h>


static bool report_requested = false;
static bool report_full = false;
static uint32_t last_report = 0;

static float velocity;
static float positions[AXES];


void report_init() {
  velocity = 0;

  for (int axis = 0; axis < AXES; axis++)
    positions[axis] = 0;
}


void report_request() {
  report_requested = true;
}


void report_request_full() {
  report_requested = report_full = true;
}


stat_t report_callback() {
  uint32_t now = rtc_get_time();
  if (now - last_report < 100) return STAT_OK;
  last_report = now;

  if (report_requested && usart_tx_empty()) vars_report(report_full);
  report_requested = report_full = false;

  return STAT_OK;
}


float get_pos(int index) {
  return cm_get_absolute_position(0, index);
}


float get_vel() {
  return mp_get_runtime_velocity();
}