// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CORE_LEAK_CHECK_TEST_H_
#define CORE_LEAK_CHECK_TEST_H_

#include "gtest/gtest.h"

#include "core/core.h"

#if CORE_PLATFORM_WINDOWS && CORE_CONFIG_DEBUG
#include <crtdbg.h>  // NOLINT
#endif

class LeakCheckTest : public testing::Test {
 public:
  LeakCheckTest() {
#if CORE_PLATFORM_WINDOWS && CORE_CONFIG_DEBUG
    _CrtMemCheckpoint(&initial_memory_state_);
#endif
  }

  virtual ~LeakCheckTest() {
#if CORE_PLATFORM_WINDOWS && CORE_CONFIG_DEBUG
    if (!HasFailure()) {
      _CrtMemState final_memory_state;
      _CrtMemState difference;
      _CrtMemCheckpoint(&final_memory_state);
      if (_CrtMemDifference(
          &difference, &initial_memory_state_, &final_memory_state)) {
        _CrtMemDumpAllObjectsSince(&difference);
        _CrtMemDumpStatistics(&difference);
        FailTest();
      }
    }
#endif
  }

 private:
#if CORE_PLATFORM_WINDOWS && CORE_CONFIG_DEBUG
  _CrtMemState initial_memory_state_;
#endif
  void FailTest() {
    FAIL() << "Memory leaks detected.";
  }
};

#endif  // CORE_LEAK_CHECK_TEST_H_