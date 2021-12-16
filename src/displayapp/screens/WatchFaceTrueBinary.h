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

        lv_obj_t* label_day;
        lv_obj_t* backgroundLabel;

        // Binary Watch
        // Time
#define CIRCLE_OUTLINE_WIDTH 4
        static constexpr uint8_t LED_SIZE1 = 37;
        static constexpr uint8_t LED_RING1 = 2;
        static constexpr uint8_t LED_SPACE_H1 = (LED_SIZE1 / 12);
        static constexpr uint8_t LED_SPACE_V1 = (LED_SIZE1 / 4);
#define LED_COL_MIN_ON   lv_color_hex(0xDD0000)
#define LED_COL_MIN_OFF  lv_color_hex(0x220000)
#define LED_COL_HOUR_ON  lv_color_hex(0x00DD00)
#define LED_COL_HOUR_OFF lv_color_hex(0x002200)
#define LED_COL_RING     lv_color_hex(0x999999)
        lv_obj_t* minTens1;
        lv_obj_t* minTens2;
        lv_obj_t* minTens4;
        lv_obj_t* minOnes1;
        lv_obj_t* minOnes2;
        lv_obj_t* minOnes4;
        lv_obj_t* minOnes8;

        lv_obj_t* hour1;
        lv_obj_t* hour2;
        lv_obj_t* hour4;
        lv_obj_t* hour8;

        lv_obj_t* amPm;

        // Date
        static constexpr uint8_t LED_SIZE2 = 18;
        static constexpr uint8_t LED_RING2 = 2;
        static constexpr uint8_t LED_SPACE_H2 = (LED_SIZE2 / 6);
        static constexpr uint8_t LED_SPACE_V2 = (LED_SIZE2 / 4);
#define LED_COL_DAY_ON  lv_color_hex(0xCCCCCC)
#define LED_COL_DAY_OFF lv_color_hex(0x222222)
#define LED_COL_MON_ON  lv_color_hex(0xCCCCCC)
#define LED_COL_MON_OFF lv_color_hex(0x222222)
#define LED_COL_RING    lv_color_hex(0x999999)
        lv_obj_t* dayOfWeek1;
        lv_obj_t* dayOfWeek2;
        lv_obj_t* dayOfWeek4;

        lv_obj_t* dayOfMonthTens1;
        lv_obj_t* dayOfMonthTens2;
        lv_obj_t* dayOfMonthOnes1;
        lv_obj_t* dayOfMonthOnes2;
        lv_obj_t* dayOfMonthOnes4;
        lv_obj_t* dayOfMonthOnes8;

        lv_obj_t* monthOfYear1;
        lv_obj_t* monthOfYear2;
        lv_obj_t* monthOfYear4;
        lv_obj_t* monthOfYear8;

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
