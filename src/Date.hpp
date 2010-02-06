
#ifndef _DATE_HPP_
#define _DATE_HPP_

#include <string>
#include <ostream>

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

    Date(int year = 1920, int month = 1, int day = 1);

    int                 getYear() const;
    int                 getMonth() const;
    int                 getDay() const;
    const std::string   getMonthAsString() const;

    bool                isEndOfYear() const;
    bool                isEndOfMonth() const;

    void                advance();

    unsigned int        getTimeDiff(Date t) const;
    unsigned int        getTimeDiff(Date t1, Date t2) const;
    
    static unsigned int daysToMonths(unsigned int days);
    static unsigned int daysToYears(unsigned int days);
    
    // For output in streams:
    friend std::ostream& operator<<(std::ostream& output, const Date& d);
};

#endif

