#include <doctest/doctest.h>
#include <nicols/nicols.h>
#include <nicols/version.h>

#include <string>

/*
TEST_CASE("NiCoLS") {
  using namespace nicols;

}
*/

TEST_CASE("NiCoLS version") {
  static_assert(std::string_view(NICOLS_VERSION) == std::string_view("1.0"));
  CHECK(std::string(NICOLS_VERSION) == std::string("1.0"));
}
