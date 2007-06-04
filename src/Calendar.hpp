
#ifndef _CALENDAR_HPP_
#define _CALENDAR_HPP_

#include <string>

/*
#include <libintl.h>
#include <gettext.h>
#define _(string) gettext (string)
*/
#define _(string) string

class Calendar {

  private:
    unsigned int year;
    unsigned int month;
    unsigned int day;

  public:

    Calendar() {
      year  = 1920;
      month = 11;
      day   = 1;
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
};

#endif

