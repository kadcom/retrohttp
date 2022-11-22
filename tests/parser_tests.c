#include <check.h>
#include <unistd.h>

#include <mhttp.h>

START_TEST (parser_tests)
{
  
}
END_TEST

Suite *buffer_suite(void) {
  Suite *s;
  TCase *tc_buffer;

  s = suite_create("buffers");
  tc_buffer = tcase_create("allocation");
  tcase_add_test(tc_buffer, parser_tests);
  suite_add_tcase(s, tc_buffer);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = buffer_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
