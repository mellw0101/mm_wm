#ifndef PTY_HPP
#define PTY_HPP

#include <pty.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include <iostream>
#include "Log.hpp"

using namespace std;

// class Pty
// {
//     private:
//         int masterFd;
//         pid_t childPid;

//     public:
//         Pty() : masterFd(-1), childPid(-1) {}

//         ~Pty()
//         {
//             if (masterFd != -1)
//             {
//                 close(masterFd);
//             }
//             if (childPid != -1)
//             {
//                 kill(childPid, SIGTERM);
//                 waitpid(childPid, NULL, 0);
//             }
//         }

//         bool
//         start()
//         {
//             int slaveFd;
//             char slaveName[1024];
//             if (openpty(&masterFd, &slaveFd, slaveName, NULL, NULL) == -1)
//             {
//                 loutE << "Failed to open PTY" << loutEND;
//                 return false;
//             }

//             childPid = fork();
//             if (childPid < 0)
//             {
//                 loutE << "Fork failed" << loutEND;
//                 return false;
//             }
//             else if (childPid == 0) // Child process
//             {
//                 close(masterFd);
//                 setsid();
//                 ioctl(slaveFd, TIOCSCTTY, 0);

//                 dup2(slaveFd, STDIN_FILENO);
//                 dup2(slaveFd, STDOUT_FILENO);
//                 dup2(slaveFd, STDERR_FILENO);

//                 close(slaveFd);
//                 execl("/usr/bin/bash", "/usr/bin/bash", (char *)NULL);

//                 // If execl returns, exit child immediately
//                 _exit(1);
//             }
//             else // Parent process
//             {
//                 close(slaveFd);
//                 return true;
//             }
//         }

//         ssize_t
//         writeToBash(const string &command)
//         {
//             if (masterFd == -1)
//             {
//                 loutE << "PTY not initialized" << loutEND;
//                 return -1;
//             }
//             return write(masterFd, command.c_str(), command.length());
//         }

//         string
//         readFromBash()
//         {
//             if (masterFd == -1)
//             {
//                 loutE << "PTY not initialized" << loutEND;
//                 return "";
//             }

//             vector<char> buffer(4096);
//             ssize_t bytes_read = read(masterFd, buffer.data(), buffer.size() - 1);
//             if (bytes_read > 0)
//             {
//                 buffer[bytes_read] = '\0';
//                 return string(buffer.data());
//             }
//             return "";
//         }
// };
// static Pty *pty(nullptr);

#endif/* PTY_HPP */