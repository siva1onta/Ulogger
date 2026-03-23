#include "../src/Ulogger.hh"

#include <iostream>

using namespace Utils;

int main() {

  Ulogger::GetInstance()->Open("/home/siva1onta/WORK/Ulogger/test.log");
  Ulogger::GetInstance()->SetLevel(Ulogger::DEBUG);

  Debug("hewew");

  return 0;
}
