/**
 ******************************************************************************
  Copyright (c) 2016 IntoRobot Industries, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

#if 0
#include "application.h"
#include "unit-test/unit-test.h"


#if PLATFORM_THREADING
test(Thread_creation)
{
    volatile bool threadRan = false;
    Thread testThread = Thread("test", [&]() {
        threadRan = true;
        for(;;) {}
    });

    for(int tries = 5; !threadRan && tries >= 0; tries--) {
        delay(1);
    }

    testThread.dispose();

    assertTrue((bool)threadRan);
}

test(thread_SingleThreadedBlock)
{
	SINGLE_THREADED_BLOCK() {

	}
	SINGLE_THREADED_BLOCK() {

	}
}

test(thread_with_lock)
{
	WITH_LOCK(Serial) {

	}

	WITH_LOCK(Serial) {

	}

}

test(thread_try_lock)
{
	TRY_LOCK(Serial) {

	}
}

// todo - test for SingleThreadedSection



volatile bool timeout_called = 0;
void timeout()
{
	timeout_called++;
}

void waitForComplete(ApplicationWatchdog& wd)
{
	while (!wd.isComplete()) {
		HAL_Delay_Milliseconds(10);
	}
}


test(application_watchdog_fires_timeout)
{
	timeout_called = 0;
	ApplicationWatchdog wd(5, timeout);
	HAL_Delay_Milliseconds(10);

	assertEqual(timeout_called, 1);
	waitForComplete(wd);
}

test(application_watchdog_doesnt_fire_when_app_checks_in)
{
	timeout_called = 0;
	unsigned t = 100;
	ApplicationWatchdog wd(t, timeout);

	for (int i=0; i<10; i++) {
		assertEqual(timeout_called, 0);
		application_checkin();
		os_thread_yield();
	}
	HAL_Delay_Milliseconds(t);
	assertEqual(timeout_called, 1);
	waitForComplete(wd);
}

#endif

#endif
