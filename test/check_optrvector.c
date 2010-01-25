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
#include "../src/optrvector.h"
#include "../src/ostring.h"

static OPtrVector *ptrvectors;
static OPtrVector *ptrvectori;

void setup(void)
{
  ptrvectors = optrvector_new(0, (OPtrVectorElementCallback)ostring_release);
  ptrvectori = optrvector_new(0, 0);
}

void teardown(void)
{
  optrvector_release(ptrvectors);
  optrvector_release(ptrvectori);
}

START_TEST(test_optrvector_create)
{
  oint32 i;
  fail_unless(optrvector_length(ptrvectors) == 0, "mytestvec should be empty");
  for (i = 0; i < 99999; ++i)
    fail_unless(optrvector_getelementptrat(ptrvectors, i) == 0, "Should be null");
}
END_TEST

START_TEST(test_optrvector_pushbackremovefind)
{
  int i, index;
  OString *str, *ptr;
  ochar buffer[128];
  fail_unless(ptrvectors != 0, "ptrvectors should be initialized, not null");
  index = optrvector_find(ptrvectors, "nottobefound", (OVectorElementCompareCallback)ostring_comparestr);
  fail_unless(index < 0, "string must not be found");

  for (i = 0; i < 10000; i++)
  {
    snprintf(buffer, 128, "this is a string %d", i);
    str = ostring_newstr(buffer);
    optrvector_pushback(ptrvectors, str);
    fail_unless(optrvector_length(ptrvectors) == (i+1), "wrong size");
    fail_unless(memcmp((OString*)optrvector_getelementptrat(ptrvectors, i), str, sizeof(OString*)) == 0, "Should be str");
    fail_unless(memcmp((OString*)optrvector_peekback(ptrvectors), str, sizeof(OString*)) == 0, "Should be str");
    fail_unless(optrvector_getelementptrat(ptrvectors, i + 1) == 0, "Should be null too");
    fail_unless(optrvector_getelementptrat(ptrvectors, -1) == 0, "Should be null too");
  }

  for (i = 0; i < 10000; i++)
  {
    ptr = (OString*)optrvector_getelementptrat(ptrvectors, i);
    fail_unless(ptr != 0, "must not be 0");
    snprintf(buffer, 128, "this is a string %d", i);
    fail_unless(strcmp(ostring_tocstr(ptr), buffer) == 0, "should be \"%s\"", buffer);
  }

  for (i = 0; i < 100; i++)
  {
    index = optrvector_find(ptrvectors, "nottobefound", (OVectorElementCompareCallback)ostring_comparestr);
    fail_unless(index < 0, "string must not be found");
    snprintf(buffer, 128, "this is a string %d", i);
    index = optrvector_find(ptrvectors, buffer, (OVectorElementCompareCallback)ostring_comparestr);
    fail_unless(index >= 0, "string must be found");
    ptr = (OString*)optrvector_getelementptrat(ptrvectors, index);
    fail_unless(ptr != 0, "must not be 0");
  }

  for (i = 0; i < 5000; i++)
  {
    ptr = (OString*)optrvector_getelementptrat(ptrvectors, i);
    optrvector_removeelementat(ptrvectors, i);
    fail_unless(optrvector_length(ptrvectors) == (10000-(i+1)), "wrong size wrong size %d!=%d", optrvector_length(ptrvectors), (10000-(i+1)));
  }
  for (i = optrvector_length(ptrvectors)-1; i >= 0; i--)
  {
    ptr = (OString*)optrvector_getelementptrat(ptrvectors, i);
    optrvector_removeelementat(ptrvectors, i);
    fail_unless(optrvector_length(ptrvectors) == i, "wrong size %d!=%d", optrvector_length(ptrvectors), i);
  }
  fail_unless(optrvector_length(ptrvectors) == 0, "wrong size, should be empty");
  for (i = 0; i < 5000; i++)
  {
    ptr = (OString*)optrvector_getelementptrat(ptrvectors, i);
    optrvector_removeelementat(ptrvectors, i);
  }
  for (i = optrvector_length(ptrvectors)-1; i >= 0; i--)
  {
    ptr = (OString*)optrvector_getelementptrat(ptrvectors, i);
    optrvector_removeelementat(ptrvectors, i);
  }
}
END_TEST

static oint32 test_optrvector_compareoint32(const oint32 *a, const oint32 *b)
{
  return *a - *b;
}

START_TEST(test_optrvector_sort)
{
  int i, tmp, prev;
  fail_unless(ptrvectori != 0, "ptrvectori should be initialized, not null");
  for (i = 0; i < 100; i++)
  {
    tmp = rand();
    optrvector_pushback(ptrvectori, &tmp);
    fail_unless(optrvector_length(ptrvectori) == (i+1), "wrong size");
    fail_unless(memcmp((oint32*)optrvector_getelementptrat(ptrvectori, i), &tmp, sizeof(oint32)) == 0, "Should be equal");
    fail_unless(optrvector_getelementptrat(ptrvectori, i + 1) == 0, "Should be null too");
    fail_unless(optrvector_getelementptrat(ptrvectori, -1) == 0, "Should be null too");
  }

  optrvector_qsort(ptrvectori, (OPtrVectorElementCompareCallback)test_optrvector_compareoint32);

  for (i = 0; i < 100-1; i++)
  {
    prev = *(oint32*)optrvector_getelementptrat(ptrvectori, i);
    tmp  = *(oint32*)optrvector_getelementptrat(ptrvectori, i+1);
    fail_unless(prev <= tmp, "should be in order");
  }
}
END_TEST

Suite *optrvector_suite(void)
{
  Suite *s = suite_create("OPtrVector");
  TCase *tccore = tcase_create("Core");
  tcase_add_checked_fixture(tccore, setup, teardown);
  tcase_add_test(tccore, test_optrvector_create);
  tcase_add_test(tccore, test_optrvector_pushbackremovefind);
  tcase_add_test(tccore, test_optrvector_sort);
  suite_add_tcase(s, tccore);
  return s;
}

int main(void)
{
  int numfailed;
  Suite *s = optrvector_suite();
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  numfailed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (numfailed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
