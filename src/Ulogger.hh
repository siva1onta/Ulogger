#pragma once

#include <cstdio>

namespace Utils {

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

    static Ulogger *GetInstance();
    void SetLevel(Level level);
    void Log(Level level, const char *file, const int line, const char *format, ...);
    void Open(const char *path);
    void Close();

   private:
    Ulogger();
    ~Ulogger();
    Ulogger(const Ulogger &sample) = delete;
    Ulogger &operator=(const Ulogger &sample) = delete;

    static Ulogger *m_instance;
    Level m_level;
    FILE *m_file;
  };

#define Debug(format, ...) Ulogger::GetInstance()->Log(Ulogger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Info(format, ...) Ulogger::GetInstance()->Log(Ulogger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Warn(format, ...) Ulogger::GetInstance()->Log(Ulogger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Error(format, ...) Ulogger::GetInstance()->Log(Ulogger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define Fatal(format, ...) Ulogger::GetInstance()->Log(Ulogger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

}
