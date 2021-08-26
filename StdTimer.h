// Timer funtionality using standerd C++ library
/*
Copyright [2021] [Ashish Patil <ashishbpatil123@gmail.com>]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
 */

#pragma once

#include <future>
#include <stdint.h>
#include <thread>

namespace StdTimerNS
{
   typedef void (* TimerHandler)(void* handlerParam);  ///< callback function type for timers
   enum class StdTimerStates
   {
      IDLE,
      START,
      STOP,
   };

   class StdTimer
   {
   public:
      StdTimer();
      ~StdTimer();

      inline bool IsSingleShot() const { return Single; }
      inline void SetSingleShot(bool singleShot) { Single = singleShot; }
      inline void SetInterval(uint64_t msec) { Interval = msec; }
      void StartTimer(TimerHandler handler, void* userParam, uint64_t timeout);
      void Stop();
      StdTimerStates GetTimerStatus();

   private:
      void Start();
      void ThreadFunction();

      TimerHandler TimerCB;
      void* UserParam;
      bool Single;
      uint64_t Interval;
      StdTimerStates CurrentState;
      std::promise<void> ExitSignal;
      std::thread* TimerThread;
   };
} // end of namespace StdTimerNS
