/**
 ******************************************************************************
  Copyright (c) 2013-2015 IntoRobot Industries, Inc.  All rights reserved.

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


// #include "spark_wiring_constants.h"
#include "application.h"
#include "unit-test/unit-test.h"
#include <array>
#include <stdlib.h>

bool rand_value_set = false;

void random_seed_from_cloud(unsigned int value)
{
	rand_value_set = true;
}

test(rand_value_set_from_cloud)
{
	assertTrue(rand_value_set);
}

template <std::size_t size> void fillBuf(std::array<int, size>& buf, int min, int max) {
    int count = size;
    while (count-->0) {
        buf[count] = random(min, max);
    }
}

test(Random_values_consistent_with_the_same_seed) {
    std::array<int, 100> buf1;
    std::array<int, 100> buf2;
    randomSeed(42);
    fillBuf(buf1, 20, 30);

    randomSeed(42);
    fillBuf(buf2, 20, 30);

    assertTrue(buf1==buf2);
}


test(Random_values_different_with_different_seeds) {
    std::array<int, 100> buf1;
    std::array<int, 100> buf2;
    randomSeed(42);
    fillBuf(buf1, 20, 30);

    randomSeed(43);
    fillBuf(buf2, 20, 30);
    assertFalse(buf1==buf2);
}

test(random_zero_returns_zero) {
    assertEqual(random(0), 0);
}

test(empty_range_returns_min) {
    assertEqual(random(10, 5),10);
    assertEqual(random(20, 20),20);
}

test(closed_range_returns_same_value) {
    assertEqual(random(10,10),10);
    assertEqual(random(11,11),11);
    assertEqual(random(-3,-3),-3);
}

void assertRandomRange(int min, int max, int iterations) {
    while (iterations-->0) {
        int r = random(min, max);
        assertMoreOrEqual(r,  min);
        assertLess(r, max);
    }
}

test(Positive_range_values_are_within_range) {
    assertRandomRange(20,30, 1000);
}

test(Negative_range_values_are_within_range) {
    assertRandomRange(-30,-20, 1000);
}

test(ZeroCrossing_range_values_are_within_range) {
    assertRandomRange(-10,10, 1000);
}
