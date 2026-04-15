#include "../src/Ulogger.h"

#include <thread>
#include <unistd.h>

void CallLog(int threadid) {
  for (int id = 0; id < 100; ++id) {
    Info("%d + %d = %d", threadid, id, threadid + id);
    usleep(100000);
  }
}

int main() {

  Ulogger::GetInstance()->Open("./test.log");
  Ulogger::GetInstance()->SetLevel(Ulogger::DEBUG);
  Ulogger::GetInstance()->SetMaxSize(1024);

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
