#include "Ulogger.hh"

#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <cstring>

#include <unistd.h>

namespace Utils {

  Ulogger *Ulogger::m_instance = nullptr;

  Ulogger::Ulogger() {
    m_level = WARN;
    m_maxsize = 16 * 1024 * 1024;
    m_file = nullptr;
  }

  Ulogger::~Ulogger() {
    Close();
  }

  Ulogger *Ulogger::GetInstance() {
    if (m_instance == nullptr) {
      m_instance = new Ulogger();
    }
    return m_instance;
  }

  void Ulogger::SetLevel(Level level) {
    m_level = level;
  }

  void Ulogger::SetMaxSize(long maxsize) {
    m_maxsize = maxsize;
  }

  void Ulogger::Log(Level level, const char *file, const int line, const char *format, ...) {
    if (level < m_level) {
      return;
    }

    char time_buf[32] {0};
    time_t now = time(nullptr);
    tm local_time;
    localtime_r(&now, &local_time);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", &local_time);

    char file_buf[PATH_MAX] {0};
    if (file[0] == '/') {
      strcat(file_buf, file);
    } else {
      char cwd[PATH_MAX] {0};
      getcwd(cwd, sizeof(cwd));
      strcat(file_buf, cwd);
      strcat(file_buf, "/");
      strcat(file_buf, file);
    }

    char format_buf[512] {0};
    va_list args;
    va_start(args, format);
    vsnprintf(format_buf, 512, format, args);
    format_buf[512 - 1] = 0;
    va_end(args);

    std::lock_guard lckgd(m_mutex);

    if (fprintf(m_file, "[%s] [%s:%d] %s\n", time_buf, file_buf, line, format_buf) < 0) {
      perror("Failed to write to log file");
    }
    fflush(m_file);

    fseek(m_file, 0, SEEK_END);
    long sz = ftell(m_file);
    if (sz < m_maxsize) {
      return;
    }

    if (fclose(m_file) != 0) {
      perror("Failed to close log file");
    }
    char filepath_buf[PATH_MAX] {0};
    memset(time_buf, 0, sizeof(time_buf));
    strftime(time_buf, sizeof(time_buf), "%Y%m%d%H%M%S", &local_time);
    strcat(filepath_buf, m_filepath);
    strcat(filepath_buf, ".");
    strcat(filepath_buf, time_buf);
    rename(m_filepath, filepath_buf);
    m_file = fopen(m_filepath, "a");
    if (m_file == nullptr) {
      perror("Failed to open log file");
    }
  }

  void Ulogger::Open(const char *path) {
    strcpy(m_filepath, path);
    std::lock_guard lckgd(m_mutex);
    if (m_file != nullptr) {
      Close();
    }
    m_file = fopen(path, "a");
    if (m_file == nullptr) {
      perror("Failed to open log file");
    }
  }

  void Ulogger::Close() {
    std::lock_guard lckgd(m_mutex);
    if (m_file == nullptr) {
      return;
    }
    if (fclose(m_file) != 0) {
      perror("Failed to close log file");
    }
  }

}
