#include <stdio.h>

struct base_class {
  int x;
};

struct derived_class {
  struct base_class base;
  int y;
};

struct derived_class2 {
  struct base_class base;
  int z;
};
void test() {
  struct derived_class d;
  struct derived_class2 d2;

  d.base.x = 10;
  d2.y = 20;

  printf("x=%i, y=%i\n", d.base.x, d.y);
}

int	main(void)
{
	test();
}
