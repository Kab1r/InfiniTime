#include "WatchFaceTrueBinary.h"

#include <date/date.h>
#include <lvgl/lvgl.h>
#include <displayapp/Colors.h>
#include <cstdio>
#include "components/settings/Settings.h"
#include "../DisplayApp.h"

using namespace Pinetime::Applications::Screens::Binary;

BinaryDot::BinaryDot() {
  obj = lv_obj_create(lv_scr_act(), nullptr);
  lv_obj_set_size(obj, SIZE, SIZE);
  lv_obj_set_style_local_radius(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_RADIUS_CIRCLE);
  lv_obj_set_style_local_bg_color(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_bg_opa(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_style_local_border_width(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, BORDER_WIDTH);
  lv_obj_set_style_local_border_color(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_obj_set_style_local_border_opa(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_COVER);
  lv_obj_set_style_local_border_side(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_BORDER_SIDE_FULL);
  bottomRight(); // to find unexpected binary dots
  set(false);
}
void BinaryDot::set(bool value) {
  lv_obj_set_style_local_bg_opa(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, value ? LV_OPA_COVER : LV_OPA_TRANSP);
}
void BinaryDot::setColors(lv_color_t borderColor, lv_color_t dotColor) {
  lv_obj_set_style_local_bg_color(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, dotColor);
  lv_obj_set_style_local_border_color(obj, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, borderColor);
}

void BinaryDot::topRight() {
  lv_obj_align(obj, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, -SPACE, SPACE);
}
void BinaryDot::topLeft() {
  lv_obj_align(obj, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, SPACE, SPACE);
}
void BinaryDot::bottomRight() {
  lv_obj_align(obj, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, -SPACE, -SPACE);
}
void BinaryDot::bottomLeft() {
  lv_obj_align(obj, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, SPACE, -SPACE);
}
void BinaryDot::below(BinaryDot& ref) {
  below(ref.obj);
}
void BinaryDot::above(BinaryDot& ref) {
  above(ref.obj);
}
void BinaryDot::leftOf(BinaryDot& ref) {
  leftOf(ref.obj);
}
void BinaryDot::rightOf(BinaryDot& ref) {
  rightOf(ref.obj);
}
void BinaryDot::below(lv_obj_t* ref) {
  lv_obj_align(obj, ref, LV_ALIGN_OUT_BOTTOM_MID, 0, SPACE);
}
void BinaryDot::above(lv_obj_t* ref) {
  lv_obj_align(obj, ref, LV_ALIGN_OUT_TOP_MID, 0, -SPACE);
}
void BinaryDot::leftOf(lv_obj_t* ref) {
  lv_obj_align(obj, ref, LV_ALIGN_OUT_LEFT_MID, -SPACE, 0);
}
void BinaryDot::rightOf(lv_obj_t* ref) {
  lv_obj_align(obj, ref, LV_ALIGN_OUT_RIGHT_MID, SPACE, 0);
}

using namespace Pinetime::Applications::Screens;

WatchFaceTrueBinary::WatchFaceTrueBinary(DisplayApp* app,
                                         Controllers::DateTime& dateTimeController,
                                         Controllers::Settings& settingsController)
  : Screen(app), currentDateTime {{}}, dateTimeController {dateTimeController}, settingsController {settingsController} {

  settingsController.SetClockFace(3);

  // Minute
  for (auto i = MINUTE_ONES - 1; i >= 0; i--) {
    minuteOnes[i].setColors(Convert(settingsController.GetPTSColorTime()), Convert(settingsController.GetPTSColorBar()));
    if (i == MINUTE_ONES - 1) {
      minuteOnes[i].topRight();
    } else {
      minuteOnes[i].below(minuteOnes[i + 1]);
    }
  }
  for (auto i = 0; i < MINUTE_TENS; i++) {
    minuteTens[i].setColors(Convert(settingsController.GetPTSColorTime()), Convert(settingsController.GetPTSColorBar()));
    if (i == 0) {
      minuteTens[i].leftOf(minuteOnes[i]);
    } else {
      minuteTens[i].above(minuteTens[i - 1]);
    }
  }
  // Hours
  for (auto i = 0; i < HOURS; i++) {
    hours[i].setColors(Convert(settingsController.GetPTSColorTime()), Convert(settingsController.GetPTSColorBar()));
    if (i == 0) {
      hours[i].leftOf(minuteTens[i]);
    } else if (i == HOURS - 1) {
      // AM / PM / 24h bit
      hours[i].leftOf(hours[i - 1]);
    } else {
      hours[i].above(hours[i - 1]);
    }
  }
  // Day of Week
  for (auto i = 0; i < DAYS_IN_WEEK; i++) {
    dayOfWeek[i].setColors(Convert(settingsController.GetPTSColorTime()), Convert(settingsController.GetPTSColorBar()));
    if (i == 0) {
      dayOfWeek[i].bottomLeft();
    } else {
      dayOfWeek[i].above(dayOfWeek[i - 1]);
    }
  }
  // Day of Month
  for (auto i = 0; i < DAYS_IN_MONTH_TENS; i++) {
    dayOfMonthTens[i].setColors(Convert(settingsController.GetPTSColorTime()), Convert(settingsController.GetPTSColorBar()));
    if (i == 0) {
      dayOfMonthTens[i].rightOf(dayOfWeek[i]);
    } else {
      dayOfMonthTens[i].above(dayOfMonthTens[i - 1]);
    }
  }
  for (auto i = 0; i < DAYS_IN_MONTH_ONES; i++) {
    dayOfMonthOnes[i].setColors(Convert(settingsController.GetPTSColorTime()), Convert(settingsController.GetPTSColorBar()));
    if (i == 0) {
      dayOfMonthOnes[i].rightOf(dayOfMonthTens[i]);
    } else {
      dayOfMonthOnes[i].above(dayOfMonthOnes[i - 1]);
    }
  }
  // Month of Year
  for (auto i = 0; i < MONTHS_IN_YEAR; i++) {
    monthOfYear[i].setColors(Convert(settingsController.GetPTSColorTime()), Convert(settingsController.GetPTSColorBar()));
    if (i == 0) {
      monthOfYear[i].rightOf(dayOfMonthOnes[i]);
    } else {
      monthOfYear[i].above(monthOfYear[i - 1]);
    }
  }

  taskRefresh = lv_task_create(RefreshTaskCallback, LV_DISP_DEF_REFR_PERIOD, LV_TASK_PRIO_MID, this);
  Refresh();
}

WatchFaceTrueBinary::~WatchFaceTrueBinary() {
  lv_task_del(taskRefresh);
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
  // Minutes
  if (currentMinute != newMinute) {
    uint8_t ones = (currentMinute = newMinute) % 10;
    uint8_t tens = currentMinute / 10;
    for (auto i = MINUTE_ONES - 1; i >= 0; i--) {
      minuteOnes[i].set(hasBin(ones, i));
    }
    for (auto i = MINUTE_TENS - 1; i >= 0; i--) {
      minuteTens[i].set(hasBin(tens, i));
    }
  }
  // Hours
  if (currentHour != newHour) {
    bool isPM = newHour >= 12 && settingsController.GetClockType() == Controllers::Settings::ClockType::H12;
    currentHour = newHour % 12 ? newHour - (isPM ? 12 : 0) : 12;
    for (auto i = HOURS - 1; i >= 0; i--) {
      hours[i].set(hasBin(currentHour, i) || (i == HOURS - 1 && isPM));
    }
    currentHour = newHour;
  }
  // Day of Week
  if (currentDayOfWeek != newDayOfWeek) {
    currentDayOfWeek = newDayOfWeek;
    for (auto i = DAYS_IN_WEEK - 1; i >= 0; i--) {
      dayOfWeek[i].set(hasBin(currentDayOfWeek, i));
    }
  }
  // Day of Month
  if (currentDay != newDay) {
    uint8_t ones = (currentDay = newDay) % 10;
    uint8_t tens = currentDay / 10;
    for (auto i = DAYS_IN_MONTH_ONES - 1; i >= 0; i--) {
      dayOfMonthOnes[i].set(hasBin(ones, i));
    }
    for (auto i = DAYS_IN_MONTH_TENS - 1; i >= 0; i--) {
      dayOfMonthTens[i].set(hasBin(tens, i));
    }
  }
  // Month of Year
  if (currentMonth != newMonth) {
    currentMonth = newMonth;
    for (auto i = MONTHS_IN_YEAR - 1; i >= 0; i--) {
      monthOfYear[i].set(hasBin(currentMonth, i));
    }
  }
}
