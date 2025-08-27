#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <utility>
#include <iostream>
#include "HLX_Logger.h"

using namespace std;

class HLX_Lockfile {
  public:
    explicit HLX_Lockfile(string filename) : fFD(-1), fFilename(std::move(filename)) {}
    ~HLX_Lockfile() {
      unlock();
    }

  private:
    int    fFD{-1};
    int    fPID{-1};
    string fFilename;

  public:
    bool lock() {
      fFD = open(fFilename.c_str(), O_RDWR | O_CREAT, 0666);
      if (fFD == -1) {
        hlog::error("HLX_Lockfile => Unable to open the file at {}", fFilename);
        return false;
      }

      struct flock fl{};
      fl.l_type   = F_WRLCK;
      fl.l_whence = SEEK_SET;
      fl.l_start  = 0;
      fl.l_len    = 0;
      fl.l_pid    = getpid();

      if (fcntl(fFD, F_SETLK, &fl) == -1) {
        hlog::error("HLX_Lockfile => Unable to lock the file at {}!", fFilename);

        // New code: read the PID from the file
        char pidStr[1024];
        if (read(fFD, pidStr, sizeof(pidStr)) == -1) {
          //hlog::error("HLX_Lockfile => Failed to read PID from the file at {}", fFilename);
        } else {
          hlog::error("HLX_Lockfile => The lock seems to be held by a process with PID {}", pidStr);
        }

        close(fFD);
        return false;
      }

      // For much fun, save the process PID to the file
      fPID = getpid();
      string pidStr = to_string(fPID);
      if (write(fFD, pidStr.c_str(), pidStr.size()) != pidStr.size()) {
        hlog::error("Failed to write PID to the file at {}", fFilename);
        close(fFD);
        return false;
      }

      return true;
    }

    void unlock() {
      if (fFD != -1) {
        struct flock fl{};
        fl.l_type = F_UNLCK;
        if (fcntl(fFD, F_SETLK, &fl) == -1) {
          hlog::error("HLX_Lockfile => Unable to unlock the file at {}", fFilename);
        }
        close(fFD);
        fFD = -1;
        fPID = -1;
      }
      // Remove the file after unlocking
      if (remove(fFilename.c_str()) != 0) {
        hlog::error("HLX_Lockfile => Unable to remove the file at {}", fFilename);
      }
    }

};
