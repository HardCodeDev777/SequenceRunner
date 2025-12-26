#pragma once

// Who give a hell about std::print fr
#define LOG(v) std::cout << v << std::endl;

constexpr bool is_debug =
#ifdef NDEBUG
false;
#else
true;
#endif