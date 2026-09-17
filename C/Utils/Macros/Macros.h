#pragma once

#define CONCAT2(a, b) a##b
#define CONCAT(a, b) CONCAT2(a, b)

#define STRINGIFY2(a) #a
#define STRINGIFY(a) STRINGIFY2(a)