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
#include "../src/ovector.h"
#include "../src/ostring.h"

static OVector *vectors;
static OVector *vectori;

void setup(void)
{
  vectors = ovector_new(sizeof(oint16),  0, 0);
  vectori = ovector_new(sizeof(oint32),  0, 0);
}

void teardown(void)
{
  ovector_release(vectors);
  ovector_release(vectori);
}

START_TEST(test_ovector_create)
{
  oint32 i;
  fail_unless(ovector_length(vectors) == 0, "testvecs should be empty");
  for (i = 0; i < 99999; ++i)
    fail_unless(ovector_getelementptrat(vectors, i) == 0, "Should be null");
}
END_TEST

static oint32 test_ovector_compareoint16(const oint16 *a, const oint16 *b)
{
  return *a - *b;
}

START_TEST(test_ovector_pushbackremovefind)
{
  int i, index;
  int value = -1023;
  fail_unless(vectors != 0, "vectors should be initialized, not null");
  index = ovector_find(vectors, &value, (OVectorElementCompareCallback)test_ovector_compareoint16);
  fail_unless(index < 0, "value must not be found");

  for (i = 0; i < 10000; i++)
  {
    ovector_pushback(vectors, &i);
    fail_unless(ovector_length(vectors) == (i+1), "wrong size");
    fail_unless(memcmp((oint16*)ovector_getelementptrat(vectors, i), &i, sizeof(oint16)) == 0, "Should be equal");
    fail_unless(memcmp((oint16*)ovector_peekback(vectors), &i, sizeof(oint16)) == 0, "Should be equal");
    fail_unless(ovector_getelementptrat(vectors, i + 1) == 0, "Should be null too");
    fail_unless(ovector_getelementptrat(vectors, -1) == 0, "Should be null too");
  }

  for (i = 0; i < 10000; i++)
  {
    oint16 *ptr = (oint16*)ovector_getelementptrat(vectors, i);
    fail_unless(ptr != 0, "must not be 0");
    fail_unless(*ptr == i, "should be \"%d\"", i);
  }

  for (i = 0; i < 100; i++)
  {
    index = ovector_find(vectors, &value, (OVectorElementCompareCallback)test_ovector_compareoint16);
    fail_unless(index < 0, "value must not be found");
    index = ovector_find(vectors, &i, (OVectorElementCompareCallback)test_ovector_compareoint16);
    fail_unless(index >= 0, "string must be found");
    oint16* ptr = (oint16*)ovector_getelementptrat(vectors, index);
    fail_unless(ptr != 0, "must not be 0");
    fail_unless(*ptr == i, "should be \"%d\"", i);
  }

  for (i = 0; i < 5000; i++)
  {
    ovector_removeelementat(vectors, i);
    fail_unless(ovector_length(vectors) == (10000-(i+1)), "wrong size %d!=%d", ovector_length(vectors), (10000-(i+1)));
  }
  for (i = ovector_length(vectors)-1; i >= 0; i--)
  {
    ovector_removeelementat(vectors, i);
    fail_unless(ovector_length(vectors) == i, "wrong size %d!=%d", ovector_length(vectors), i);
  }
  fail_unless(ovector_length(vectors) == 0, "wrong size, should be empty");
  for (i = 0; i < 5000; i++)
  {
    ovector_removeelementat(vectors, i);
  }
  for (i = ovector_length(vectors)-1; i >= 0; i--)
  {
    ovector_removeelementat(vectors, i);
  }
}
END_TEST

static oint32 test_ovector_compareoint32(const oint32 *a, const oint32 *b)
{
  return *a - *b;
}

START_TEST(test_ovector_sort)
{
  int i, tmp, prev;
  fail_unless(vectori != 0, "vectori should be initialized, not null");
  for (i = 0; i < 100; i++)
  {
    tmp = rand();
    ovector_pushback(vectori, &tmp);
    fail_unless(ovector_length(vectori) == (i+1), "wrong size");
    fail_unless(memcmp((oint32*)ovector_getelementptrat(vectori, i), &tmp, sizeof(oint32)) == 0, "Should be equal");
    fail_unless(ovector_getelementptrat(vectori, i + 1) == 0, "Should be null too");
    fail_unless(ovector_getelementptrat(vectori, -1) == 0, "Should be null too");
  }

  ovector_qsort(vectori, (OVectorElementCompareCallback)test_ovector_compareoint32);

  for (i = 0; i < 100-1; i++)
  {
    prev = *(oint32*)ovector_getelementptrat(vectori, i);
    tmp  = *(oint32*)ovector_getelementptrat(vectori, i+1);
    fail_unless(prev <= tmp, "should be in order");
  }
}
END_TEST

Suite *ovector_suite(void)
{
  Suite *s = suite_create("OVector");
  TCase *tccore = tcase_create("Core");
  tcase_add_checked_fixture (tccore, setup, teardown);
  tcase_add_test(tccore, test_ovector_create);
  tcase_add_test(tccore, test_ovector_pushbackremovefind);
  tcase_add_test(tccore, test_ovector_sort);
  suite_add_tcase(s, tccore);
  return s;
}

int main(void)
{
  int numfailed;
  Suite *s = ovector_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  numfailed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (numfailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
