#include <stdio.h>
#include <assert.h>

#include <smallengine/se_maths.h>

void TST_DoubleEqual()
{
        double x = 1.0;
        double y = 0.99999;
        double z = 0.5;

        assert(double_equal(x, y) == 1);
        assert(double_equal(x, z) == 0);
        assert(double_equal(x, 1.000001) == 1);

        printf("[Double Equal] Complete, all tests pass!\n");
        return;
}

void TST_DoubleCompare()
{
        double x = 1.0;
        double y = 0.99999;
        double z = 0.5;

        assert(double_compare(x, y) == 0);
        assert(double_compare(x, z) == 1);
        assert(double_compare(z, y) == -1);

        printf("[Double Compare] Complete, all tests pass!\n");
        return;
}

int main()
{
        TST_DoubleEqual();
        TST_DoubleCompare();

        printf("[TESTING] Maths Tests Complete\n");

        return 0;
}
