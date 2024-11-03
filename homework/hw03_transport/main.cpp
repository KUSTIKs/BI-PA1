#ifndef __PROGTEST__
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

constexpr unsigned DOW_MON = 0b0000'0001;
constexpr unsigned DOW_TUE = 0b0000'0010;
constexpr unsigned DOW_WED = 0b0000'0100;
constexpr unsigned DOW_THU = 0b0000'1000;
constexpr unsigned DOW_FRI = 0b0001'0000;
constexpr unsigned DOW_SAT = 0b0010'0000;
constexpr unsigned DOW_SUN = 0b0100'0000;
constexpr unsigned DOW_WORKDAYS =
    DOW_MON | DOW_TUE | DOW_WED | DOW_THU | DOW_FRI;
constexpr unsigned DOW_WEEKEND = DOW_SAT | DOW_SUN;
constexpr unsigned DOW_ALL = DOW_WORKDAYS | DOW_WEEKEND;

struct TDate {
  unsigned m_Year;
  unsigned m_Month;
  unsigned m_Day;
};

struct TDate makeDate(unsigned y, unsigned m, unsigned d) {
  struct TDate res = {y, m, d};
  return res;
}
#endif /* __PROGTEST__ */

#define BASE_YEAR 2000
#define DAYS_IN_WEEK 7
#define DAYS_IN_YEAR 365

#define FEBRUARY 2

typedef long long int int64;

const unsigned weekdayMasks[DAYS_IN_WEEK] = {
    DOW_MON, DOW_TUE, DOW_WED, DOW_THU, DOW_FRI, DOW_SAT, DOW_SUN,
};

enum Weekday {
  MONDAY = 0,
  TUESDAY = 1,
  WEDNESDAY = 2,
  THURSDAY = 3,
  FRIDAY = 4,
  SATURDAY = 5,
  SUNDAY = 6,
};

int mod(int a, int b) {
  return (a % b + b) % b;
}

bool isLeap(unsigned year) {
  bool leap4 = year % 4 == 0;
  bool leap100 = year % 100 == 0;
  bool leap400 = year % 400 == 0;
  bool leap4000 = year % 4000 == 0;

  return (leap4 && !leap100) || (leap400 && !leap4000);
}

unsigned countLeapYears(unsigned year) {
  unsigned leapYearsCount = 0;

  leapYearsCount += year / 4;
  leapYearsCount -= year / 100;
  leapYearsCount += year / 400;
  leapYearsCount -= year / 4000;

  return leapYearsCount;
}

unsigned daysInMonth(unsigned month, bool isLeap) {
  switch (month) {
    case FEBRUARY:
      return isLeap ? 29 : 28;

    case 4:
    case 6:
    case 9:
    case 11:
      return 30;

    default:
      return 31;
  }
}

bool isValidDate(struct TDate date) {
  if (date.m_Year < 2000) return false;
  if (date.m_Month < 1 || date.m_Month > 12) return false;

  unsigned maxDays = daysInMonth(date.m_Month, isLeap(date.m_Year));
  if (date.m_Day < 1 || date.m_Day > maxDays) return false;

  return true;
}

int64 dateToDays(struct TDate date) {
  int64 fullYearsFromBase = date.m_Year - BASE_YEAR;

  unsigned dateLeapYears = countLeapYears(date.m_Year - 1);
  unsigned baseLeapYears = countLeapYears(BASE_YEAR - 1);
  unsigned leapYears = dateLeapYears - baseLeapYears;

  int64 daysInYear = fullYearsFromBase * DAYS_IN_YEAR + leapYears;

  unsigned daysInMonths = 0;
  for (unsigned month = 1; month < date.m_Month; month += 1) {
    daysInMonths += daysInMonth(month, isLeap(date.m_Year));
  }

  return daysInYear + daysInMonths + date.m_Day - 1;
}

struct TDate daysToDate(int64 days) {
  struct TDate date = {BASE_YEAR, 1, 1};

  while (days >= DAYS_IN_YEAR + isLeap(date.m_Year)) {
    days -= DAYS_IN_YEAR + isLeap(date.m_Year);
    date.m_Year += 1;
  }

  while (days >= daysInMonth(date.m_Month, isLeap(date.m_Year))) {
    days -= daysInMonth(date.m_Month, isLeap(date.m_Year));
    date.m_Month += 1;
  }

  date.m_Day += days;

  return date;
}

int64 getDaysBetween(struct TDate startDate, struct TDate endDate) {
  return dateToDays(endDate) - dateToDays(startDate) + 1;
}

unsigned getWeekday(struct TDate date) {
  return (dateToDays(date) + 5) % DAYS_IN_WEEK;
}

int64 getConnectionsForWeekday(unsigned weekday, unsigned perWorkDay) {
  if (weekday == SATURDAY) {
    return ceil(perWorkDay / 2.0);
  } else if (weekday == SUNDAY) {
    return ceil(perWorkDay / 3.0);
  }

  return perWorkDay;
}

int64 countConnections(
    struct TDate from, struct TDate to, unsigned perWorkDay, unsigned dowMask
) {
  if (!isValidDate(from) || !isValidDate(to)) return -1;

  int64 count = 0;

  unsigned fromWeekday = getWeekday(from);
  unsigned daysBetween = getDaysBetween(from, to);

  //   printf("fromWeekday is %d\n", fromWeekday);
  //   printf("daysBetween is %d\n", daysBetween);

  if (daysBetween <= 0) return -1;

  for (unsigned weekday = 0; weekday < DAYS_IN_WEEK; weekday += 1) {
    // printf("Weekday is %d\n", weekday);
    // printf(
    //     "  dowMask & weekdayMasks[weekday]: %d\n",
    //     (dowMask & weekdayMasks[weekday]) == 0
    // );

    if ((dowMask & weekdayMasks[weekday]) == 0) continue;

    unsigned firstTargetDay = mod(weekday - fromWeekday, DAYS_IN_WEEK);

    if (daysBetween < firstTargetDay) continue;

    unsigned shiftedDaysBetween = daysBetween - firstTargetDay;
    unsigned matchWeeksCount = ceil(shiftedDaysBetween / (double)DAYS_IN_WEEK);

    unsigned weekdayConnections = getConnectionsForWeekday(weekday, perWorkDay);

    // printf("  firstTargetDay: %u\n", firstTargetDay);
    // printf("  matchWeeksCount: %u\n", matchWeeksCount);
    // printf("  weekdayConnections: %u\n", weekdayConnections);
    // printf("\n");

    count += matchWeeksCount * weekdayConnections;
  }

  //   printf("  count: %llu\n", count);

  return count;
}

struct TDate endDate(
    struct TDate from, int64 connections, unsigned perWorkDay, unsigned dowMask
) {
  if (!isValidDate(from) || dowMask == 0 || perWorkDay == 0) {
    return {0, 0, 0};
  };
  // printf("from: %02d.%02d.%d\n", from.m_Day, from.m_Month, from.m_Year);

  unsigned fromWeekday = getWeekday(from);
  int64 fromDays = dateToDays(from);

  int64 daysBetween = 0;
  unsigned targetWeekdaysCount = 0;
  unsigned connectionsPerWeek = 0;
  int firstTargetDay = -1;

  for (unsigned shift = 0; shift < DAYS_IN_WEEK; shift += 1) {
    unsigned weekday = mod(fromWeekday + shift, DAYS_IN_WEEK);

    if ((dowMask & weekdayMasks[weekday]) == 0) continue;

    if (firstTargetDay == -1) {
      firstTargetDay = weekday;
    }

    connectionsPerWeek += getConnectionsForWeekday(weekday, perWorkDay);
    targetWeekdaysCount += 1;
  }

  daysBetween += mod(firstTargetDay - 1, DAYS_IN_WEEK);

  // printf("firstTargetDay: %d\n", firstTargetDay);
  // printf("connectionsPerWeek: %d\n", connectionsPerWeek);
  // printf("daysBetween: %lld\n", daysBetween);

  unsigned fullWeeks = connections / connectionsPerWeek;
  int connectionsLeft = connections % connectionsPerWeek;

  daysBetween += fullWeeks * DAYS_IN_WEEK;
  // printf("fullWeeks: %d\n", fullWeeks);
  // printf("connectionsLeft: %d\n", connectionsLeft);
  // printf("daysBetween: %lld\n", daysBetween);

  for (unsigned shift = 0; shift < DAYS_IN_WEEK; shift += 1) {
    unsigned weekday = mod(firstTargetDay + shift, DAYS_IN_WEEK);
    // struct TDate date = daysToDate(fromDays + daysBetween - 1);
    // printf("date: %d.%d.%d\n", date.m_Day, date.m_Month, date.m_Year);
    // printf("weekday: %d\n", weekday);

    if ((dowMask & weekdayMasks[weekday]) == 0) continue;

    connectionsLeft -= getConnectionsForWeekday(weekday, perWorkDay);
    // printf("connectionsLeft: %d\n", connectionsLeft);

    if (connectionsLeft < 0) break;

    daysBetween += 1;
  }

  // printf("daysBetween: %lld\n", daysBetween);

  struct TDate to = daysToDate(fromDays + daysBetween - 1);

  return to;
}

#ifndef __PROGTEST__
int main() {
  struct TDate d;

  assert(
      countConnections(
          makeDate(2024, 10, 1), makeDate(2024, 10, 31), 1, DOW_ALL
      ) == 31
  );
  assert(
      countConnections(
          makeDate(2024, 10, 1), makeDate(2024, 10, 31), 10, DOW_ALL
      ) == 266
  );
  assert(
      countConnections(
          makeDate(2024, 10, 1), makeDate(2024, 10, 31), 1, DOW_WED
      ) == 5
  );
  assert(
      countConnections(
          makeDate(2024, 10, 2), makeDate(2024, 10, 30), 1, DOW_WED
      ) == 5
  );
  assert(
      countConnections(
          makeDate(2024, 10, 1), makeDate(2024, 10, 1), 10, DOW_TUE
      ) == 10
  );
  assert(
      countConnections(
          makeDate(2024, 10, 1), makeDate(2024, 10, 1), 10, DOW_WED
      ) == 0
  );
  assert(
      countConnections(
          makeDate(2024, 1, 1), makeDate(2034, 12, 31), 5,
          DOW_MON | DOW_FRI | DOW_SAT
      ) == 7462
  );
  assert(
      countConnections(
          makeDate(2024, 1, 1), makeDate(2034, 12, 31), 0,
          DOW_MON | DOW_FRI | DOW_SAT
      ) == 0
  );
  assert(
      countConnections(makeDate(2024, 1, 1), makeDate(2034, 12, 31), 100, 0) ==
      0
  );
  assert(
      countConnections(
          makeDate(2024, 10, 10), makeDate(2024, 10, 9), 1, DOW_MON
      ) == -1
  );
  assert(
      countConnections(
          makeDate(2024, 2, 29), makeDate(2024, 2, 29), 1, DOW_ALL
      ) == 1
  );
  assert(
      countConnections(
          makeDate(2023, 2, 29), makeDate(2023, 2, 29), 1, DOW_ALL
      ) == -1
  );
  assert(
      countConnections(
          makeDate(2100, 2, 29), makeDate(2100, 2, 29), 1, DOW_ALL
      ) == -1
  );
  assert(
      countConnections(
          makeDate(2400, 2, 29), makeDate(2400, 2, 29), 1, DOW_ALL
      ) == 1
  );
  assert(
      countConnections(
          makeDate(4000, 2, 29), makeDate(4000, 2, 29), 1, DOW_ALL
      ) == -1
  );
  d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_ALL);
  assert(d.m_Year == 2025 && d.m_Month == 1 && d.m_Day == 8);
  d = endDate(makeDate(2024, 10, 1), 100, 6, DOW_ALL);
  assert(d.m_Year == 2024 && d.m_Month == 10 && d.m_Day == 20);
  d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_WORKDAYS);
  assert(d.m_Year == 2025 && d.m_Month == 2 && d.m_Day == 17);
  d = endDate(makeDate(2024, 10, 1), 100, 4, DOW_WORKDAYS);
  assert(d.m_Year == 2024 && d.m_Month == 11 && d.m_Day == 4);
  d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_THU);
  assert(d.m_Year == 2026 && d.m_Month == 9 && d.m_Day == 2);
  d = endDate(makeDate(2024, 10, 1), 100, 2, DOW_THU);
  assert(d.m_Year == 2025 && d.m_Month == 9 && d.m_Day == 17);
  d = endDate(makeDate(2024, 10, 1), 100, 0, DOW_THU);
  assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);
  d = endDate(makeDate(2024, 10, 1), 100, 1, 0);
  assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
