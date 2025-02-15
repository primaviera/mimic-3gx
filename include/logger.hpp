#pragma once

namespace CTRPluginFramework
{

namespace logger
{

void clear();

void write(std::string fmt);

int init(const std::string& dir);

void deinit();

} // namespace logger

} // namespace CTRPluginFramework
