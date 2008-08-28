
#ifndef _DATE_HPP_
#define _DATE_HPP_

#include <string>

/*
#include <libintl.h>
#include <gettext.h>
#define _(string) gettext (string)
*/
#define _(string) string

class Date {

  private:
    unsigned int year;
    unsigned int month;
    unsigned int day;

  public:

    Date(int year = 1920, int month = 1, int day = 1) {
      this->year  = year;
      this->month = month;
      this->day   = day;
    }

    int getYear() {
      return year;
    }

    int getMonth() {
      return month;
    }
    const std::string getMonthAsString() {
      switch(getMonth()) {
        case  1: return std::string(_("January"));
        case  2: return std::string(_("February"));
        case  3: return std::string(_("Mars"));
        case  4: return std::string(_("April"));
        case  5: return std::string(_("May"));
        case  6: return std::string(_("June"));
        case  7: return std::string(_("July"));
        case  8: return std::string(_("August"));
        case  9: return std::string(_("September"));
        case 10: return std::string(_("October"));
        case 11: return std::string(_("November"));
        case 12: return std::string(_("December"));
      }

      // TODO
      return std::string("ERROR");
    }

    int getDay() {
      return day;
    }

    bool isEndOfYear() {
      return (day == 30 && month == 12);
    }
    bool isEndOfMonth() {
      return (day == 30);
    }

    void advance() {
      if(day == 30) {
        day = 1;
        if(month == 12) {
          month = 1;
          year++;
        } else {
          month++;
        }
      } else {
        day++;
      }
    }

    unsigned int getTimeDiff(Date t) {
      return getTimeDiff(*this, t);
    }
    unsigned int getTimeDiff(Date t1, Date t2) {

      long int day_diff = (t1.getYear()*12*30 + t1.getMonth()*30 + t1.getDay())
                -(t2.getYear()*12*30 + t2.getMonth()*30 + t2.getDay());
      if(day_diff < 0) day_diff *= -1;

      return (unsigned int)day_diff;
    }
    static unsigned int daysToMonths(unsigned int days) {
      return (unsigned int)(days/30);
    }
    static unsigned int daysToYears(unsigned int days) {
      return (unsigned int)(days/30/12);
    }
};

#endif

