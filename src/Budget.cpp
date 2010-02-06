#include "Budget.hpp"

Budget::BudgetMonth::BudgetMonth() {

  for(int i = 0; i < Budget::ITEM_COUNT; i++) {
    items[i] = 0;
  }
}

Budget::Budget(int startBalance) {

  balance = startBalance;
}

float Budget::getTax() {
  return this->tax;
}
void Budget::setTax(float tax) {
  this->tax = tax;
}

int Budget::getBalance() {
  return this->balance;
}

int Budget::getBudgetItem(unsigned char type) const {
  int itemBalance = 0;
  for(int month = 1; month <= 12; month++) {
    itemBalance += getMonthlyBudgetItem(type, month);
  }
  return itemBalance;
}

int Budget::getMonthlyBudgetItem(unsigned char type, unsigned char month) const {
  return months[month - 1].items[type];
}

void Budget::updateBudgetItem(unsigned char type, unsigned char month, int balance) {
  months[month - 1].items[type] += balance;
  this->balance                 += balance;
}
