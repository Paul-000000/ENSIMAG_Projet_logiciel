#include "unity.h"
#include "zz_quant.h"
#include "dct.h"
#include "qtables.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test2(void){
	
    TEST_ASSERT(1 > 0);
	TEST_ASSERT_FALSE(2+2==5);
    // TEST_FAIL_MESSAGE("Parce que la vie est injuste!");
}

void test_position(void) {
    int16_t identite[64] = {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 15,
        16, 17, 18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29, 30, 31,
        32, 33, 34, 35, 36, 37, 38, 39,
        40, 41, 42, 43, 44, 45, 46, 47,
        48, 49, 50, 51, 52, 53, 54, 55,
        56, 57, 58, 59, 60, 61, 62, 63
    };
    
    int16_t zzi[64] = {
        0, 1, 8, 16, 9, 2, 3, 10, 
        17, 24, 32, 25, 18, 11, 4, 5, 
        12, 19, 26, 33, 40, 48, 41, 34, 
        27, 20, 13, 6, 7, 14, 21, 28, 
        35, 42, 49, 56, 57, 50, 43, 36, 
        29, 22, 15, 23, 30, 37, 44, 51, 
        58, 59, 52, 45, 38, 31, 39, 46, 
        53, 60, 61, 54, 47, 55, 62, 63 
    };

    zigzag(identite);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(zzi,identite,64);
}

void test_quantification ( void ){

    int16_t image_test[64] = {
        0x007b, 0x0000, 0xfffb, 0xfeb3, 0x0000, 0xfee4, 0x0000, 0xfedb,
        0x0000, 0xff37, 0x007f, 0x0000, 0xff8c, 0x0000, 0x0000, 0x0000,
        0x006a, 0x0000, 0xffa1, 0x0000, 0xffa7, 0xff76, 0x0000, 0x011c,
        0x0000, 0x0045, 0x0000, 0xffec, 0x0000, 0xff7e, 0x0000, 0x0018,
        0x0000, 0x0013, 0x0000, 0x00dc, 0x0000, 0x001a, 0x0000, 0x00ff,
        0x0000, 0x0099, 0x0000, 0x0000, 0x0003, 0x0000, 0x007d, 0x0000,
        0xffa9, 0x0000, 0xff5a, 0x0000, 0x0014, 0x0000, 0x0000, 0xfe1f,
        0x0000, 0xffba, 0x0000, 0x00f4, 0x0000, 0x0000, 0xff3d, 0x0000
    };

    int16_t image_attendue[64] = {
        0x0018, 0x0000, 0xffff, 0xffbe, 0x0000, 0xffe9, 0x0000, 0xfff0,
        0x0000, 0xffce, 0x001f, 0x0000, 0xfff2, 0x0000, 0x0000, 0x0000,
        0x001a, 0x0000, 0xffed, 0x0000, 0xfff9, 0xfff8, 0x0000, 0x0010,
        0x0000, 0x000d, 0x0000, 0xfffe, 0x0000, 0xfffb, 0x0000, 0x0001,
        0x0000, 0x0002, 0x0000, 0x000c, 0x0000, 0x0000, 0x0000, 0x000b,
        0x0000, 0x000d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0003, 0x0000,
        0xfffb, 0x0000, 0xfff9, 0x0000, 0x0000, 0x0000, 0x0000, 0xfff0,
        0x0000, 0xfffe, 0x0000, 0x0008, 0x0000, 0x0000, 0xfffa, 0x0000 
    };

    quantification(image_test, Y);

    TEST_ASSERT_UINT8_ARRAY_WITHIN(1, image_attendue, image_test, 64);
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_position);
    RUN_TEST(test_quantification);
    RUN_TEST(test2);
    return UNITY_END();
}
