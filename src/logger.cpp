#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "logger.hpp"

namespace CTRPluginFramework {

namespace logger {

    File logFile;

    /* https://github.com/Anto726/CTRPluginFramework-RevolutionBase/blob/-/src/base/logger.cpp#L7 */
    std::string GetLogName()
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
        strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S.log", nowtm);

        return std::string(buf);
    }

    void Clear()
    {
        logFile.Clear();
    }

    void Write(std::string fmt)
    {
        logFile.Write(fmt.c_str(), fmt.size());
    }

    int Initialize(const std::string& path)
    {
        Directory logDir;
        std::string logName = std::string(path + GetLogName());
        std::vector<std::string> files;

        if (Directory::Open(logDir, path, true))
            return 1;
        logDir.ListFiles(files, ".log");
        if (!files.empty()) {
            /* Sort log files from oldest to newest depending on their names. */
            std::sort(files.begin(), files.end());
            while (files.size() > 2) {
                std::string pathToDelete = std::string(path + files.front());
                files.erase(files.begin());
                File::Remove(pathToDelete);
            }
        }
        if (File::Open(logFile, logName, (File::WRITE | File::CREATE | File::APPEND | File::SYNC)))
            return 1;

        return 0;
    }

    void Finalize()
    {
        logFile.Close();
    }

} // namespace logger

} // namespace CTRPluginFramework
