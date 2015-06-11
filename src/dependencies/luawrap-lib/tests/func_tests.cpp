#include <cstdlib>

#include <UnitTest++.h>

#include <luawrap/testutils.h>
#include <luawrap/functions.h>

template<typename T>
static T idagent1(T a) {
	return a;
}
template<typename T>
static T idagent2(const T& a) {
	return a;
}

template<typename T, typename V>
static void test_num_func(T func, V num) {
	TestLuaState L;

	luawrap::push_function(L, "testfunc", func);
	CHECK_EQUAL(1, lua_gettop(L));

	lua_pushnumber(L, num);
	lua_call(L, 1, 1);

	CHECK(lua_isnumber(L, -1));
	CHECK(num == lua_tonumber(L, -1));

	lua_pop(L, 1);
	L.finish_check();
}

#define TEMPLATE_TEST(func, temparg, n) \
	TEST(func##temparg) { test_num_func(func<temparg>, (temparg)n); }

SUITE (luawrap_func_tests) {
	TEMPLATE_TEST(idagent1, int, 2);

	//If idagent2 works once, it should work the other times
	TEMPLATE_TEST(idagent2, int, 2);

	TEMPLATE_TEST(idagent1, char, 255);
	TEMPLATE_TEST(idagent1, short, 3);
	TEMPLATE_TEST(idagent1, long, 4);
	TEMPLATE_TEST(idagent1, float, 1.1);
	TEMPLATE_TEST(idagent1, double, 1.1);
}
