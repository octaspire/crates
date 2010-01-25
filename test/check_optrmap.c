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
#include "../src/optrmap.h"
#include "../src/ostring.h"

static OPtrMap* ptrmap;

void setup(void)
{
  ptrmap = optrmap_new(0);
}

void teardown(void)
{
  optrmap_release(ptrmap);
}

START_TEST(test_optrmap_all)
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
      fail_unless(optrmap_getindex(ptrmap, keys[i]) == -1, "Nonexistent key found");
      fail_unless(optrmap_get(ptrmap, keys[i]) == 0, "Nonexistent key found");
    }

    for (i=0; keys[i]; ++i)
    {
      fail_unless(optrmap_put(ptrmap, keys[i], &i) != 0, "Put failed");
      fail_unless(optrmap_getindex(ptrmap, keys[i]) == i, "Wrong index");
      fail_unless(optrmap_get(ptrmap, keys[i]) != 0, "Key should have been found");
      fail_unless(*(oint32*)optrmap_get(ptrmap, keys[i]) == i, "Wrong value");
    }

    for (i=0; keys[i]; ++i)
    {
      fail_unless(optrmap_put(ptrmap, keys[i], &i) == 0, "Double add should fail");
      fail_unless(optrmap_getindex(ptrmap, keys[i]) == i, "Wrong index");
      fail_unless(optrmap_get(ptrmap, keys[i]) != 0, "Key should have been found");
      fail_unless(*(oint32*)optrmap_get(ptrmap, keys[i]) == i, "Wrong value");
    }

    for (i=0; keys[i]; ++i)
    {
      fail_unless(optrmap_getindex(ptrmap, keys[i]) != -1, "Key not found");
      fail_unless(optrmap_get(ptrmap, keys[i]) != 0, "Key not found");
      fail_unless(*(oint32*)optrmap_get(ptrmap, keys[i]) == i, "Wrong value");
    }

    for (i=0; keys[i]; ++i)
    {
      optrmap_remove(ptrmap, keys[i]);
      fail_unless(optrmap_getindex(ptrmap, keys[i]) == -1, "Nonexistent key found");
      fail_unless(optrmap_get(ptrmap, keys[i]) == 0, "Nonexistent key found");
    }

    for (i=0; keys[i]; ++i)
    {
      fail_unless(optrmap_getindex(ptrmap, keys[i]) == -1, "Nonexistent key found");
      fail_unless(optrmap_get(ptrmap, keys[i]) == 0, "Nonexistent key found");
    }

    for (i=0; keys[i]; ++i)
      optrmap_remove(ptrmap, keys[i]);
  }
}
END_TEST

Suite *optrmap_suite(void)
{
  Suite *s = suite_create("OPtrMap");
  TCase *tccore = tcase_create("Core");
  tcase_add_checked_fixture(tccore, setup, teardown);
  tcase_add_test(tccore, test_optrmap_all);
  suite_add_tcase(s, tccore);
  return s;
}

int main(void)
{
  int numfailed;
  Suite *s = optrmap_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  numfailed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (numfailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
