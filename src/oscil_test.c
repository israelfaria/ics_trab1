#include <stdio.h>
#include <stdlib.h>

#include "CUnit/CUnit.h"
#include "oscil.h"

static oscil * oscillator = NULL;
static int init_suite(void);
static int clean_suite(void);

static void test_start_oscil(void);


static int init_suite(void){	
	oscillator = start_oscil(1024,44120,32767);
	if (oscillator == NULL){
		return -1;	
	}	
	return 0;
}

static int clean_suite(void){
	free(oscillator);
	return 0;	
}

static void test_start_oscil(void){
	CU_ASSERT_EQUAL(oscillator->table_length, 1024);
	CU_ASSERT_EQUAL(oscillator->sample_rate, 44120);
	CU_ASSERT_EQUAL(oscillator->amplitude, 32767);
	CU_ASSERT_PTR_NOT_NULL(oscillator->wavetable);	
}

void add_oscil_tests(void){
	CU_pSuite suite;
	
	suite = CU_add_suite("Testes do Oscilador",init_suite,clean_suite);
	CU_ADD_TEST(suite,test_start_oscil);
}
