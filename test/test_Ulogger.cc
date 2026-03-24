#include "../src/Ulogger.hh"

#include <thread>

using namespace Utils;

void CallLog(int threadid) {
  for (int id = 0; id < 100; ++id) {
    Info("%d + %d = %d", threadid, id, threadid + id);
  }
}

int main() {

  Ulogger::GetInstance()->Open("/home/siva1onta/WORK/Ulogger/test.log");
  Ulogger::GetInstance()->SetLevel(Ulogger::DEBUG);

  std::thread thread1(CallLog, 1);
  std::thread thread2(CallLog, 2);
  std::thread thread3(CallLog, 3);
  std::thread thread4(CallLog, 4);

  thread1.join();
  thread2.join();
  thread3.join();
  thread4.join();

  return 0;
}
