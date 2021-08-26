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

#include "StdTimer.h"
#include <chrono>
#include <stdio.h>

using namespace StdTimerNS;
const uint64_t SMALL_INTERVAL_MS = 2;

StdTimer::StdTimer()
   : TimerCB(nullptr)
   , UserParam(nullptr)
   , Single(true)
   , Interval(0)
   , CurrentState(StdTimerStates::IDLE)
   , ExitSignal()
   , TimerThread(nullptr)
{
}

StdTimer::~StdTimer()
{
   if(StdTimerStates::START == CurrentState)
   {
      Stop();
   }

   delete TimerThread;
   TimerThread = nullptr;
}

void StdTimer::StartTimer(TimerHandler handler, void* userParam, uint64_t timeout)
{
   TimerCB = handler;
   UserParam = userParam;
   Interval = timeout;
   Start();
}

void StdTimer::Start()
{
   if(StdTimerStates::IDLE == CurrentState)
   {
      CurrentState = StdTimerStates::START;
      ExitSignal = std::promise<void>();
      if(nullptr != TimerThread)
      {
         delete TimerThread;
         TimerThread = nullptr;
      }
      TimerThread = new std::thread(&StdTimer::ThreadFunction, this);
      TimerThread->detach();
   }
   else
   {
      ExitSignal.set_value();
      std::this_thread::sleep_for(std::chrono::milliseconds(SMALL_INTERVAL_MS));
   }
}

void StdTimer::Stop()
{
   CurrentState = StdTimerStates::STOP;
   ExitSignal.set_value();
   std::this_thread::sleep_for(std::chrono::milliseconds(SMALL_INTERVAL_MS));
}

StdTimerStates StdTimer::GetTimerStatus()
{
   return CurrentState;
}

void StdTimer::ThreadFunction()
{
   bool stop = false;
   std::future<void> futureObj = ExitSignal.get_future();
   while(!stop)
   {
      if(std::future_status::timeout == futureObj.wait_for(std::chrono::milliseconds(Interval)))
      {
         if(Single)
         {
            CurrentState = StdTimerStates::STOP;
         }
         TimerCB(UserParam);
      }
      switch(CurrentState)
      {
      case StdTimerStates::STOP:
         stop = true;
         CurrentState = StdTimerStates::IDLE;
         break;
      default:
         ExitSignal = std::promise<void>();
         futureObj = ExitSignal.get_future();
         break;
      }
   }
}
