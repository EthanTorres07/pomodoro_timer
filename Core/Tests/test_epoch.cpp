/**
 * @file    test_epoch.cpp
 * @brief   Tests syncEpochTime()
 *
 * @author  Ethan Torres
 * @date    Jun 28, 2026
 * @version 1.0.0
 * @copyright Copyright (c) 2026 All rights reserved.
 */

#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "app.hpp"



uint32_t epochTime;

// Unit Test Suite
void run_epoch_tests()
{
    printf("Running Epoch Conversion Tests...\n\n");

    // Test Case 1: Standard Current Date (June 28, 2026 @ 14:30:00 UTC)
    TimeStamp test1 = {0, 30, 14, 0, 28, 6, 2026};
    syncEpochTime(test1);
    printf("Test 1 (Standard Date): Got %u | Expected 1782647400\n", epochTime);
    assert(epochTime == 1782647400);

    // Test Case 2: Boundary Check - Exact Unix Epoch Start (Jan 1, 1970 @ 00:00:00 UTC)
    TimeStamp test2 = {0, 0, 0, 0, 1, 1, 1970};
    syncEpochTime(test2);
    printf("Test 2 (Epoch Zero):    Got %u | Expected 0\n", epochTime);
    assert(epochTime == 0);

    // Test Case 3: January Corner Case (Jan 1, 2027 @ 00:00:00 UTC)
    TimeStamp test3 = {0, 0, 0, 0, 1, 1, 2027};
    syncEpochTime(test3);
    printf("Test 3 (Jan Rollover):  Got %u | Expected 1798761600\n", epochTime);
    assert(epochTime == 1798761600);

    // Test Case 4: Leap Year Check - Last Second of Leap Day (Feb 29, 2028 @ 23:59:59 UTC)
    TimeStamp test4 = {59, 59, 23, 0, 29, 2, 2028};
    syncEpochTime(test4);
    printf("Test 4 (Leap Day):      Got %u | Expected 1835481599\n", epochTime);
    assert(epochTime == 1835481599);

    // Test Case 5: End of Century Non-Leap Year (Feb 28, 2100 @ 12:00:00 UTC)
    TimeStamp test5 = {0, 0, 12, 0, 28, 2, 2100};
    syncEpochTime(test5);
    printf("Test 5 (Year 2100):     Got %u | Expected 4107576000\n", epochTime);
    assert(epochTime == 4107576000);

    printf("\n>>> All Tests Passed Successfully! <<<\n\n");
}

int main()
{
    run_epoch_tests();
    return 0;
}
