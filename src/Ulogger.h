#ifndef ULOGGER_H_
#define ULOGGER_H_

#include <climits>
#include <cstdio>
#include <mutex>

class Ulogger {

 public:
  enum Level {
    DEBUG = 0,
    INFO,
    WARN,
    ERROR,
    FATAL,
    LEVEL_COUNT
  };

  Ulogger(const Ulogger &sample) = delete;
  Ulogger &operator=(const Ulogger &sample) = delete;

  static Ulogger *GetInstance();
  void SetLevel(Level level);
  void SetMaxSize(long maxsize);
  void Log(Level level, const char *file, const int line, const char *format, ...);
  void Open(const char *path);
  void Close();

 private:
  Ulogger();
  ~Ulogger();

  static Ulogger *m_instance;
  std::mutex m_mutex;
  Level m_level;
  char m_filepath[PATH_MAX];
  FILE *m_file;
  long m_maxsize;
};

#define Debug(format, ...) Ulogger::GetInstance()->Log(Ulogger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Info(format, ...) Ulogger::GetInstance()->Log(Ulogger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Warn(format, ...) Ulogger::GetInstance()->Log(Ulogger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Error(format, ...) Ulogger::GetInstance()->Log(Ulogger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Fatal(format, ...) Ulogger::GetInstance()->Log(Ulogger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

#endif
