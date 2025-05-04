#pragma once

namespace CTRPluginFramework {

namespace logger {

    void Clear();

    void Write(std::string fmt);

    int Initialize(const std::string& path);

    void Finalize();

} // namespace logger

} // namespace CTRPluginFramework
