#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "logger.hpp"

namespace CTRPluginFramework
{

namespace logger
{

File log_file;

// https://github.com/Anto726/CTRPluginFramework-RevolutionBase/blob/-/src/base/logger.cpp#L7
std::string get_log_name()
{
    timeval t;
    time_t nowtime;
    tm* nowtm;
    char buf[64];

    // https://gitlab.com/thepixellizeross/ctrpluginframework/-/blob/develop/Library/source/ctrulibExtension/system/syscalls.c#L49
    s64 now = osGetTime() - 2208988800000ULL;
    t.tv_sec = now / 1000;
    t.tv_usec = (now - 1000 * t.tv_sec) * 1000;

    // https://stackoverflow.com/a/2409054
    nowtime = t.tv_sec;
    nowtm = localtime(&nowtime);
    strftime(buf, sizeof(buf), "log_%Y-%m-%d_%H-%M-%S.txt", nowtm);

    return std::string(buf);
}

void clear()
{
    log_file.Clear();
}

void write(std::string fmt)
{
    log_file.Write(fmt.c_str(), fmt.size());
}

int init(const std::string& dir)
{
    Directory log_dir;
    std::string log_name = std::string(dir + get_log_name());

    if (Directory::Open(log_dir, dir, true)) return 1;
    if (File::Open(log_file, log_name, (File::WRITE | File::CREATE | File::APPEND | File::SYNC)))
        return 1;

    return 0;
}

void deinit()
{
    log_file.Close();
}

} // namespace logger

} // namespace CTRPluginFramework
