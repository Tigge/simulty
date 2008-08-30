
#ifndef _BUDGET_HPP_
#define _BUDGET_HPP_


class Budget {

  public:

  static const unsigned char ITEM_INCOME_TAXES_RESIDENTAL = 0;
  static const unsigned char ITEM_INCOME_TAXES_COMMERSIAL = 1;
  static const unsigned char ITEM_INCOME_TAXES_INDUSTRIAL = 2;

  static const unsigned char ITEM_EXPENSE_TRAFFIC         = 3;

  static const unsigned char ITEM_EXPENSE_CRIME           = 4;
  static const unsigned char ITEM_EXPENSE_FIRE            = 5;
  static const unsigned char ITEM_EXPENSE_HEALTH          = 6;

  static const unsigned char ITEM_EXPENSE_ELECTRICITY     = 7;
  static const unsigned char ITEM_EXPENSE_WATER           = 8;
  static const unsigned char ITEM_EXPENSE_ENVIRONMENT     = 9;
  static const unsigned char ITEM_EXPENSE_CONNECTIVITY    = 10;
  
  static const unsigned char ITEM_EXPENSE_BUILD           = 11;

  static const unsigned char ITEM_COUNT                   = 12;

  private:
  
  class BudgetMonth {
    
    public:
  
    double items[Budget::ITEM_COUNT];
    BudgetMonth();
  };

  BudgetMonth months[12];
  
  int         balance;
  double      tax;
  
  public: 
  
  Budget();
  Budget(int startBalance);
  ~Budget();
  
  int   getBudgetItem(unsigned char type) const;
  int   getMonthlyBudgetItem(unsigned char type, unsigned char month) const;
  
  float getTax();
  void  setTax(float tax);
  
  int   getBalance();
  
  void  updateBudgetItem(unsigned char type, unsigned char month, int balance);
};

#endif

