#pragma once

#include <chrono>
#include <functional>

std::chrono::milliseconds benchWrap(std::function <void()> func);

#define BENCH(X) std::cout << benchWrap([&]{X}).count() << " ms\n"
