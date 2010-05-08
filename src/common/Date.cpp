#include "Date.hpp"

Date::Date(int year, int month, int day) {
  this->year  = year;
  this->month = month;
  this->day   = day;
}

int Date::getYear() const {
  return year;
}

int Date::getMonth() const {
  return month;
}

const std::string Date::getMonthAsString() const {
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

int Date::getDay() const {
  return day;
}

bool Date::isEndOfYear() const {
  return (day == 30 && month == 12);
}
bool Date::isEndOfMonth() const {
  return (day == 30);
}

void Date::advance() {
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

unsigned int Date::getTimeDiff(Date t) const {
  return getTimeDiff(*this, t);
}

unsigned int Date::getTimeDiff(Date t1, Date t2) const {

  long int day_diff = (t1.getYear()*12*30 + t1.getMonth()*30 + t1.getDay())
            -(t2.getYear()*12*30 + t2.getMonth()*30 + t2.getDay());
  if(day_diff < 0) day_diff *= -1;

  return (unsigned int)day_diff;
}

unsigned int Date::daysToMonths(unsigned int days) {
  return (unsigned int)(days/30);
}

unsigned int Date::daysToYears(unsigned int days) {
  return (unsigned int)(days/30/12);
}

std::ostream& operator<<(std::ostream& output, const Date& d) {
  output << "(" <<  d.getYear() << "-" << d.getMonth() 
      << "-" << d.getDay() << ")";
  return output;
}
