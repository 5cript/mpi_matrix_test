#pragma once

#include <chrono>
#include <functional>

std::chrono::milliseconds benchWrap(std::function <void()> func);
