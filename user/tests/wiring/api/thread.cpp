/**
 ******************************************************************************
 Copyright (c) 2015 IntoRobot Industries, Inc.  All rights reserved.

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

#include "testapi.h"

#if 0
#if PLATFORM_THREADING

test(api_thread) {
    void (*thread_fn)(void *) = NULL;
    void *param = (void *)0x123;
    int x = 0;

    API_COMPILE(Thread("test", thread_fn));
    API_COMPILE(Thread("test", thread_fn, param));
    API_COMPILE(Thread("test", thread_fn, param, OS_THREAD_PRIORITY_DEFAULT + 1));
    API_COMPILE(Thread("test", thread_fn, param, OS_THREAD_PRIORITY_DEFAULT + 1, 512));
    API_COMPILE(Thread("test", [&]() { x++; }));

    Thread t;
    API_COMPILE(t.dispose());
    API_COMPILE(t.join());
    API_COMPILE(t.is_valid());
    API_COMPILE(t.is_current());
    API_COMPILE(t = Thread("test", thread_fn));
}

test(api_single_threaded_section) {
    API_COMPILE(SingleThreadedSection sts);
}

test(api_atomic_section) {
    API_COMPILE(AtomicSection as);
}

test(api_application_watchdog) {
    unsigned stack_size = 512;
    application_checkin();
    ApplicationWatchdog wd(30000, System.reset);
    ApplicationWatchdog wd2(30000, System.reset, stack_size);
}

#endif

#endif
