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
#include "../src/omap.h"
#include "../src/ostring.h"

static OMap* map;

void setup(void)
{
  map = omap_new(sizeof(oint32), 0);
}

void teardown(void)
{
  omap_release(map);
}

START_TEST(test_omap_all)
{
  oint32 iteration, i;
  const ochar *keys[] = {
    "",
    " ",
    "abc",
    "123",
    "abcdefghijklmnopqrstuvwxyz0123456789-_!#&|\".:,;^?@$<>ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    0
  };

  for (iteration = 0; iteration < 100; ++iteration)
  {
    for (i=0; keys[i]; ++i)
    {
      fail_unless(omap_getindex(map, keys[i]) == -1, "Nonexistent key found");
      fail_unless(omap_get(map, keys[i]) == 0, "Nonexistent key found");
    }

    for (i=0; keys[i]; ++i)
    {
      fail_unless(omap_put(map, keys[i], &i) != 0, "Put failed");
      fail_unless(omap_getindex(map, keys[i]) == i, "Wrong index");
      fail_unless(omap_get(map, keys[i]) != 0, "Key should have been found");
      fail_unless(*(oint32*)omap_get(map, keys[i]) == i, "Wrong value");
    }

    for (i=0; keys[i]; ++i)
    {
      fail_unless(omap_put(map, keys[i], &i) == 0, "Double add should fail");
      fail_unless(omap_getindex(map, keys[i]) == i, "Wrong index");
      fail_unless(omap_get(map, keys[i]) != 0, "Key should have been found");
      fail_unless(*(oint32*)omap_get(map, keys[i]) == i, "Wrong value");
    }

    for (i=0; keys[i]; ++i)
    {
      fail_unless(omap_getindex(map, keys[i]) != -1, "Key not found");
      fail_unless(omap_get(map, keys[i]) != 0, "Key not found");
      fail_unless(*(oint32*)omap_get(map, keys[i]) == i, "Wrong value");
    }

    for (i=0; keys[i]; ++i)
    {
      omap_remove(map, keys[i]);
      fail_unless(omap_getindex(map, keys[i]) == -1, "Nonexistent key found");
      fail_unless(omap_get(map, keys[i]) == 0, "Nonexistent key found");
    }

    for (i=0; keys[i]; ++i)
    {
      fail_unless(omap_getindex(map, keys[i]) == -1, "Nonexistent key found");
      fail_unless(omap_get(map, keys[i]) == 0, "Nonexistent key found");
    }

    for (i=0; keys[i]; ++i)
      omap_remove(map, keys[i]);
  }
}
END_TEST

Suite *omap_suite(void)
{
  Suite *s = suite_create("OMap");
  TCase *tccore = tcase_create("Core");
  tcase_add_checked_fixture(tccore, setup, teardown);
  tcase_add_test(tccore, test_omap_all);
  suite_add_tcase(s, tccore);
  return s;
}

int main(void)
{
  int numfailed;
  Suite *s = omap_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  numfailed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (numfailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
