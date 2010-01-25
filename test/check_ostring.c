/*
  Crates ‐ extensible 3D puzzle game.
  Copyright (C) 2008‐2010  Octaspire (www.octaspire.com)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include <check.h>
#include "../src/ostring.h"

void setup(void)
{
}

void teardown(void)
{
}

START_TEST(test_ostring_compare)
{
  OString *str1, *str2, *str3, *str4;
  const ochar* buffer1 = "abcdefghijklmnopqrstuvwxyz0123456789!#@&\\\"=|_-*'^'";
  const ochar* buffer2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#@&\\\"=|_-*'^'";
  str1 = ostring_newstr(buffer1);
  str2 = ostring_new(str1);
  str3 = ostring_new(str2);
  str4 = ostring_newstr(buffer2);

  fail_unless(ostring_comparestr(str1, buffer1) == 0, "should be equal");
  fail_unless(ostring_comparestr(str1, buffer2) != 0, "should be inequal");

  fail_unless(ostring_compare(str1, str2) == 0, "should be equal");
  fail_unless(ostring_compare(str1, str3) == 0, "should be equal");
  fail_unless(ostring_compare(str2, str3) == 0, "should be equal");
  fail_unless(ostring_compare(str1, str4) != 0, "should be inequal");
  fail_unless(ostring_compare(str2, str4) != 0, "should be inequal");

  fail_unless(ostring_casecomparestr(str1, buffer2) == 0, "should be equal");
  fail_unless(ostring_casecompare(str1, str4) == 0, "should be equal");

  ostring_release(str1);
  ostring_release(str2);
  ostring_release(str3);
  ostring_release(str4);
}
END_TEST

Suite *ostring_suite(void)
{
  Suite *s = suite_create ("OString");
  TCase *tccore = tcase_create("Core");
  tcase_add_checked_fixture(tccore, setup, teardown);
  tcase_add_test(tccore, test_ostring_compare);
  suite_add_tcase(s, tccore);
  return s;
}

int main(void)
{
  int numfailed;
  Suite *s = ostring_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  numfailed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (numfailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
