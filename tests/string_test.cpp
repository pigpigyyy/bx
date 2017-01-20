/*
 * Copyright 2010-2017 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bx#license-bsd-2-clause
 */

#include "test.h"
#include <bx/string.h>
#include <bx/crtimpl.h>
#include <bx/handlealloc.h>

bx::AllocatorI* g_allocator;

TEST_CASE("chars", "")
{
	for (char ch = 'A'; ch <= 'Z'; ++ch)
	{
		REQUIRE(!bx::isLower(ch) );
		REQUIRE(!bx::isNumeric(ch) );
		REQUIRE(bx::isUpper(ch) );
		REQUIRE(bx::isAlpha(ch) );
		REQUIRE(bx::isAlphaNum(ch) );
		REQUIRE(bx::isLower(bx::toLower(ch) ) );
	}
}

TEST_CASE("strnlen", "")
{
	const char* test = "test";

	REQUIRE(0 == bx::strnlen(test, 0) );
	REQUIRE(2 == bx::strnlen(test, 2) );
	REQUIRE(4 == bx::strnlen(test, UINT32_MAX) );
}

TEST_CASE("strlncpy", "")
{
	char dst[128];
	size_t num;

	num = bx::strlncpy(dst, 1, "blah");
	REQUIRE(num == 0);

	num = bx::strlncpy(dst, 3, "blah", 3);
	REQUIRE(0 == strcmp(dst, "bl") );
	REQUIRE(num == 2);

	num = bx::strlncpy(dst, sizeof(dst), "blah", 3);
	REQUIRE(0 == strcmp(dst, "bla") );
	REQUIRE(num == 3);

	num = bx::strlncpy(dst, sizeof(dst), "blah");
	REQUIRE(0 == strcmp(dst, "blah") );
	REQUIRE(num == 4);
}

TEST_CASE("strlncat", "")
{
	char dst[128] = { '\0' };

	REQUIRE(0 == bx::strlncat(dst, 1, "cat") );

	REQUIRE(4 == bx::strlncpy(dst, 5, "copy") );
	REQUIRE(3 == bx::strlncat(dst, 8, "cat") );
	REQUIRE(0 == bx::strncmp(dst, "copycat") );

	REQUIRE(1 == bx::strlncat(dst, BX_COUNTOF(dst), "------", 1) );
	REQUIRE(3 == bx::strlncat(dst, BX_COUNTOF(dst), "cat") );
	REQUIRE(0 == bx::strncmp(dst, "copycat-cat") );
}

TEST_CASE("strincmp", "")
{
	REQUIRE(0 == bx::strincmp("test", "test") );
	REQUIRE(0 == bx::strincmp("test", "testestes", 4) );
	REQUIRE(0 == bx::strincmp("testestes", "test", 4) );
	REQUIRE(0 != bx::strincmp("preprocess", "platform") );

	const char* abvgd = "abvgd";
	const char* abvgx = "abvgx";
	const char* empty = "";
	REQUIRE(0 == bx::strincmp(abvgd, abvgd) );
	REQUIRE(0 == bx::strincmp(abvgd, abvgx, 4) );

	REQUIRE(0 >  bx::strincmp(abvgd, abvgx) );
	REQUIRE(0 >  bx::strincmp(empty, abvgd) );

	REQUIRE(0 <  bx::strincmp(abvgx, abvgd) );
	REQUIRE(0 <  bx::strincmp(abvgd, empty) );
}

TEST_CASE("strnchr", "")
{
	const char* test = "test";
	REQUIRE(NULL == bx::strnchr(test, 's', 0) );
	REQUIRE(NULL == bx::strnchr(test, 's', 2) );
	REQUIRE(&test[2] == bx::strnchr(test, 's') );
}

TEST_CASE("strnrchr", "")
{
	const char* test = "test";
	REQUIRE(NULL == bx::strnrchr(test, 's', 0) );
	REQUIRE(NULL == bx::strnrchr(test, 's', 1) );
	REQUIRE(&test[2] == bx::strnrchr(test, 's') );
}

TEST_CASE("stristr", "")
{
	const char* test = "The Quick Brown Fox Jumps Over The Lazy Dog.";

	REQUIRE(NULL == bx::stristr(test, "quick", 8) );
	REQUIRE(NULL == bx::stristr(test, "quick1") );
	REQUIRE(&test[4] == bx::stristr(test, "quick", 9) );
	REQUIRE(&test[4] == bx::stristr(test, "quick") );
}

TEST_CASE("strnstr", "")
{
	const char* test = "The Quick Brown Fox Jumps Over The Lazy Dog.";

	REQUIRE(NULL == bx::strnstr(test, "quick", 8) );
	REQUIRE(NULL == bx::strnstr(test, "quick1") );
	REQUIRE(NULL == bx::strnstr(test, "quick", 9) );
	REQUIRE(NULL == bx::strnstr(test, "quick") );

	REQUIRE(NULL == bx::strnstr(test, "Quick", 8) );
	REQUIRE(NULL == bx::strnstr(test, "Quick1") );
	REQUIRE(&test[4] == bx::strnstr(test, "Quick", 9) );
	REQUIRE(&test[4] == bx::strnstr(test, "Quick") );
}

TEST_CASE("StringView", "")
{
	bx::StringView sv("test");
	REQUIRE(4 == sv.getLength() );

	bx::CrtAllocator crt;
	g_allocator = &crt;

	typedef bx::StringT<&g_allocator> String;

	String st(sv);
	REQUIRE(4 == st.getLength() );

	st.append("test");
	REQUIRE(8 == st.getLength() );

	st.append("test", 2);
	REQUIRE(10 == st.getLength() );

	REQUIRE(0 == strcmp(st.getPtr(), "testtestte") );

	st.clear();
	REQUIRE(0 == st.getLength() );
	REQUIRE(4 == sv.getLength() );

	st.append("test");
	REQUIRE(4 == st.getLength() );

	sv.clear();
	REQUIRE(0 == sv.getLength() );
}
