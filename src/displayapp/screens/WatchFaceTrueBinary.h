#pragma once

#include <lvgl/src/lv_core/lv_obj.h>
#include <chrono>
#include <cstdint>
#include <memory>
#include "Screen.h"
#include "ScreenList.h"
#include "components/datetime/DateTimeController.h"

namespace Pinetime {
  namespace Controllers {
    class Settings;
    class Battery;
    class Ble;
    class NotificationManager;
    class MotionController;
  }

  namespace Applications {
    namespace Screens {
      namespace Binary {
        inline bool hasBin(uint8_t x, uint8_t bin) {
          return (x & 0b1 << bin) >> bin;
        }

        class BinaryDot {
        private:
          lv_obj_t* obj;
          static constexpr uint8_t SIZE = 26;
          static constexpr uint8_t SPACE = SIZE / 4;
          static constexpr uint8_t BORDER_WIDTH = SIZE / 7;

        public:
          BinaryDot();
          void set(bool value);

          void topRight();
          void topLeft();
          void bottomRight();
          void bottomLeft();

          void below(BinaryDot& ref);
          void above(BinaryDot& ref);
          void leftOf(BinaryDot& ref);
          void rightOf(BinaryDot& ref);
          void below(lv_obj_t* ref);
          void above(lv_obj_t* ref);
          void leftOf(lv_obj_t* ref);
          void rightOf(lv_obj_t* ref);
        };
      }

      class WatchFaceTrueBinary : public Screen {
      public:
        WatchFaceTrueBinary(DisplayApp* app,
                            Controllers::DateTime& dateTimeController,
                            Controllers::Battery& batteryController,
                            Controllers::Ble& bleController,
                            Controllers::NotificationManager& notificationManager,
                            Controllers::Settings& settingsController,
                            Controllers::MotionController& motionController);
        ~WatchFaceTrueBinary() override;

        void Refresh() override;

        void OnObjectEvent(lv_obj_t* pObj, lv_event_t i);

      private:
        uint8_t currentMonth = 0;
        uint8_t currentDay = 0;
        uint8_t currentDayOfWeek = 0;
        uint8_t currentHour = 0;
        uint8_t currentMinute = 0;

        DirtyValue<std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>> currentDateTime {};

        lv_task_t* taskRefresh;

        // Binary Watch
        // Time

        static constexpr uint8_t MINUTE_ONES = 4;
        static constexpr uint8_t MINUTE_TENS = 3;
        Binary::BinaryDot minuteOnes[MINUTE_ONES];
        Binary::BinaryDot minuteTens[MINUTE_TENS];

        static constexpr uint8_t HOURS = 5;
        Binary::BinaryDot hours[HOURS];

        // Date
        static constexpr uint8_t DAYS_IN_WEEK = 3;
        Binary::BinaryDot dayOfWeek[DAYS_IN_WEEK];

        static constexpr uint8_t DAYS_IN_MONTH_ONES = 4;
        static constexpr uint8_t DAYS_IN_MONTH_TENS = 2;
        Binary::BinaryDot dayOfMonthOnes[DAYS_IN_MONTH_ONES];
        Binary::BinaryDot dayOfMonthTens[DAYS_IN_MONTH_TENS];

        static constexpr uint8_t MONTHS_IN_YEAR = 4;
        Binary::BinaryDot monthOfYear[MONTHS_IN_YEAR];

        // Controllers
        Controllers::DateTime& dateTimeController;
        Controllers::Battery& batteryController;
        Controllers::Ble& bleController;
        Controllers::NotificationManager& notificationManager;
        Controllers::Settings& settingsController;
        Controllers::MotionController& motionController;
      };
    }
  }
}
