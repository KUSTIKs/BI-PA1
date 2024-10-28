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

bool isLeap(unsigned year) {
  return ((year % 4 == 0) && (year % 100 != 0)) ||
         ((year % 400 == 0) && (year % 4000 != 0));
}

bool isValidDate(struct TDate date) {
  if (date.m_Month == FEBRUARY) {
    unsigned maxDay = isLeap(date.m_Year) ? 29 : 28;

    return date.m_Day <= maxDay;
  } else if (date.m_Month % 2 == 1) {
    return date.m_Day <= 30;
  }

  return date.m_Day <= 31;
}

unsigned dateToDays(struct TDate date) {
  unsigned fullYearsFromBase = date.m_Year - BASE_YEAR;

  unsigned daysInYear = fullYearsFromBase * DAYS_IN_YEAR;

  daysInYear += fullYearsFromBase / 4;
  daysInYear -= fullYearsFromBase / 100;
  daysInYear += fullYearsFromBase / 400;
  daysInYear -= fullYearsFromBase / 4000;

  unsigned monthCount = date.m_Month - 1;
  unsigned daysInMonth = monthCount * 31;

  daysInMonth -= monthCount / 2;

  if (monthCount >= FEBRUARY) {
    if (isLeap(fullYearsFromBase)) {
      daysInMonth -= 1;
    } else {
      daysInMonth -= 2;
    }
  }

  return daysInYear + daysInMonth + date.m_Day - 1;
}

unsigned getDaysBetween(struct TDate startDate, struct TDate endDate) {
  return dateToDays(endDate) - dateToDays(startDate) + 1;
}

unsigned getWeekday(struct TDate date) {
  // +5 is -2 (mod 7). base date (0.0.2000) is saturday

  return (dateToDays(date) + 5) % DAYS_IN_WEEK;
}

long long getConnectionsForWeekday(unsigned weekday, unsigned perWorkDay) {
  if (weekday == SATURDAY) {
    return ceil(perWorkDay / 2.0);
  } else if (weekday == SUNDAY) {
    return ceil(perWorkDay / 3.0);
  }

  return perWorkDay;
}

long long countConnections(
    struct TDate from, struct TDate to, unsigned perWorkDay, unsigned dowMask
) {
  if (!isValidDate(from) || !isValidDate(to)) return -1;

  long long count = 0;

  unsigned fromWeekday = getWeekday(from);
  unsigned toWeekday = getWeekday(to);
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

    unsigned firstTargetIndex =
        (weekday - fromWeekday + DAYS_IN_WEEK) % DAYS_IN_WEEK;

    if (daysBetween < firstTargetIndex) continue;

    unsigned matchWeeksCount =
        ceil((double)(daysBetween - firstTargetIndex) / DAYS_IN_WEEK);
    unsigned weekdayConnections = getConnectionsForWeekday(weekday, perWorkDay);

    // printf("  firstTargetIndex: %u\n", firstTargetIndex);
    // printf("  matchWeeksCount: %u\n", matchWeeksCount);
    // printf("  weekdayConnections: %u\n", weekdayConnections);
    // printf("\n");

    count += matchWeeksCount * weekdayConnections;
  }

  //   printf("  count: %llu\n", count);

  return count;
}

// struct TDate endDate(
//     struct TDate from, long long connections, unsigned perWorkDay,
//     unsigned dowMask
// ) {
//   // todo
// }

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
  //   d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_ALL);
  //   assert(d.m_Year == 2025 && d.m_Month == 1 && d.m_Day == 8);
  //   d = endDate(makeDate(2024, 10, 1), 100, 6, DOW_ALL);
  //   assert(d.m_Year == 2024 && d.m_Month == 10 && d.m_Day == 20);
  //   d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_WORKDAYS);
  //   assert(d.m_Year == 2025 && d.m_Month == 2 && d.m_Day == 17);
  //   d = endDate(makeDate(2024, 10, 1), 100, 4, DOW_WORKDAYS);
  //   assert(d.m_Year == 2024 && d.m_Month == 11 && d.m_Day == 4);
  //   d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_THU);
  //   assert(d.m_Year == 2026 && d.m_Month == 9 && d.m_Day == 2);
  //   d = endDate(makeDate(2024, 10, 1), 100, 2, DOW_THU);
  //   assert(d.m_Year == 2025 && d.m_Month == 9 && d.m_Day == 17);
  //   d = endDate(makeDate(2024, 10, 1), 100, 0, DOW_THU);
  //   assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);
  //   d = endDate(makeDate(2024, 10, 1), 100, 1, 0);
  //   assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);
  //   return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
