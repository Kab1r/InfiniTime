#include "WatchFaceTrueBinary.h"

#include <date/date.h>
#include <lvgl/lvgl.h>
#include <cstdio>
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "NotificationIcon.h"
#include "Symbols.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/ble/NotificationManager.h"
#include "components/motion/MotionController.h"
#include "components/settings/Settings.h"
#include "../DisplayApp.h"

using namespace Pinetime::Applications::Screens;

WatchFaceTrueBinary::WatchFaceTrueBinary(DisplayApp* app,
                                         Controllers::DateTime& dateTimeController,
                                         Controllers::Battery& batteryController,
                                         Controllers::Ble& bleController,
                                         Controllers::NotificationManager& notificationManager,
                                         Controllers::Settings& settingsController,
                                         Controllers::MotionController& motionController)
  : Screen(app),
    currentDateTime {{}},
    dateTimeController {dateTimeController},
    batteryController {batteryController},
    bleController {bleController},
    notificationManager {notificationManager},
    settingsController {settingsController},
    motionController {motionController} {

  settingsController.SetClockFace(3);

  // minutes
  minOnes8 = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(minOnes8, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_bg_opa(minOnes8, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_style_local_radius(minOnes8, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
  lv_obj_set_style_local_outline_width(minOnes8, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, CIRCLE_OUTLINE_WIDTH);
  lv_obj_set_style_local_outline_color(minOnes8, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_outline_pad(minOnes8, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_size(minOnes8, LED_SIZE1, LED_SIZE1);
  lv_obj_align(minOnes8, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, -LED_SIZE1, -LED_SIZE1);

  minOnes4 = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(minOnes4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_bg_opa(minOnes4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_style_local_radius(minOnes4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
  lv_obj_set_style_local_outline_width(minOnes4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, CIRCLE_OUTLINE_WIDTH);
  lv_obj_set_style_local_outline_color(minOnes4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_outline_pad(minOnes4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_size(minOnes4, LED_SIZE1, LED_SIZE1);
  lv_obj_align(minOnes4, minOnes8, LV_ALIGN_OUT_BOTTOM_MID, 0, -LED_SPACE_H1);

  minOnes2 = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(minOnes2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_bg_opa(minOnes2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_style_local_radius(minOnes2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
  lv_obj_set_style_local_outline_width(minOnes2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, CIRCLE_OUTLINE_WIDTH);
  lv_obj_set_style_local_outline_color(minOnes2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_outline_pad(minOnes2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_size(minOnes2, LED_SIZE1, LED_SIZE1);
  lv_obj_align(minOnes2, minOnes4, LV_ALIGN_OUT_BOTTOM_MID, 0, -LED_SPACE_H1);

  minOnes1 = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_bg_color(minOnes1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_bg_opa(minOnes1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_style_local_radius(minOnes1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
  lv_obj_set_style_local_outline_width(minOnes1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, CIRCLE_OUTLINE_WIDTH);
  lv_obj_set_style_local_outline_color(minOnes1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_outline_pad(minOnes1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_size(minOnes1, LED_SIZE1, LED_SIZE1);
  lv_obj_align(minOnes1, minOnes2, LV_ALIGN_OUT_BOTTOM_MID, 0, -LED_SPACE_H1);
}

WatchFaceTrueBinary::~WatchFaceTrueBinary() {
  lv_obj_clean(lv_scr_act());
}

void WatchFaceTrueBinary::Refresh() {
  currentDateTime = dateTimeController.CurrentDateTime();
  if (!currentDateTime.IsUpdated()) {
    return;
  }

  auto newDateTime = currentDateTime.Get();

  auto dp = date::floor<date::days>(newDateTime);
  auto time = date::make_time(newDateTime - dp);
  auto yearMonthDay = date::year_month_day(dp);

  uint8_t newMonth = (unsigned) yearMonthDay.month();
  uint8_t newDay = (unsigned) yearMonthDay.day();
  uint8_t newDayOfWeek = date::weekday(yearMonthDay).iso_encoding();
  uint8_t newHour = time.hours().count();
  uint8_t newMinute = time.minutes().count();

  if (newMinute != currentMinute) {
    currentMinute = newMinute;
    if ((currentMinute & 0x0001) >> 0) {
      lv_obj_set_style_local_bg_opa(minOnes1, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    }
    if ((currentMinute & 0x0010) >> 1) {
      lv_obj_set_style_local_bg_opa(minOnes2, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    }
    if ((currentMinute & 0x0100) >> 2) {
      lv_obj_set_style_local_bg_opa(minOnes4, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    }
    if ((currentMinute & 0x1000) >> 3) {
      lv_obj_set_style_local_bg_opa(minOnes8, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
    }
  }
  if (newHour != currentHour) {
    currentHour = newHour;
  }
  if (newDayOfWeek != currentDayOfWeek) {
    currentDayOfWeek = newDayOfWeek;
  }
  if (newDay != currentDay) {
    currentDay = newDay;
  }
  if (newMonth != currentMonth) {
    currentMonth = newMonth;
  }
}
