/*
 * Copyright 2010-2017 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bx#license-bsd-2-clause
 */

#ifndef BX_STRING_H_HEADER_GUARD
#define BX_STRING_H_HEADER_GUARD

#include "bx.h"
#include <alloca.h>
#include <stdarg.h> // va_list
#include <wchar.h>  // wchar_t

#include <bx/allocator.h>
#include <bx/hash.h>

namespace bx
{
	/// Non-zero-terminated string view.
	class StringView
	{
	public:
		///
		StringView();

		///
		StringView(const StringView& _rhs);

		///
		StringView& operator=(const StringView& _rhs);

		///
		StringView(const char* _ptr, uint32_t _len = UINT16_MAX);

		///
		void set(const char* _ptr, uint32_t _len = UINT16_MAX);

		///
		void clear();

		///
		const char* getPtr() const;

		///
		const char* getTerm() const;

		///
		bool isEmpty() const;

		///
		uint32_t getLength() const;

	protected:
		const char* m_ptr;
		uint32_t    m_len;
	};

	/// ASCII string
	template<bx::AllocatorI** AllocatorT>
	class StringT : public StringView
	{
	public:
		///
		StringT();

		///
		StringT(const StringT<AllocatorT>& _rhs);

		///
		StringT<AllocatorT>& operator=(const StringT<AllocatorT>& _rhs);

		///
		StringT(const char* _ptr, uint32_t _len = UINT32_MAX);

		///
		StringT(const StringView& _rhs);

		///
		~StringT();

		///
		void set(const char* _ptr, uint32_t _len = UINT32_MAX);

		///
		void append(const char* _ptr, uint32_t _len = UINT32_MAX);

		///
		void clear();
	};

	///
	bool toBool(const char* _str);

	/// Case insensitive string compare.
	int32_t stricmp(const char* _a, const char* _b);

	///
	size_t strnlen(const char* _str, size_t _max);

	/// Copy _num characters from string _src to _dst buffer of maximum _dstSize capacity
	/// including zero terminator. Copy will be terminated with '\0'.
	size_t strlncpy(char* _dst, size_t _dstSize, const char* _src, size_t _num = -1);

	/// Find substring in string. Limit search to _size.
	const char* strnstr(const char* _str, const char* _find, size_t _size);

	/// Find substring in string. Case insensitive.
	const char* stristr(const char* _str, const char* _find);

	/// Find substring in string. Case insensitive. Limit search to _max.
	const char* stristr(const char* _str, const char* _find, size_t _max);

	/// Find new line. Returns pointer after new line terminator.
	const char* strnl(const char* _str);

	/// Find end of line. Retuns pointer to new line terminator.
	const char* streol(const char* _str);

	/// Skip whitespace.
	const char* strws(const char* _str);

	/// Skip non-whitespace.
	const char* strnws(const char* _str);

	/// Skip word.
	const char* strword(const char* _str);

	/// Find matching block.
	const char* strmb(const char* _str, char _open, char _close);

	// Normalize string to sane line endings.
	void eolLF(char* _out, size_t _size, const char* _str);

	// Finds identifier.
	const char* findIdentifierMatch(const char* _str, const char* _word);

	// Finds any identifier from NULL terminated array of identifiers.
	const char* findIdentifierMatch(const char* _str, const char* _words[]);

	/// Cross platform implementation of vsnprintf that returns number of
	/// characters which would have been written to the final string if
	/// enough space had been available.
	int32_t vsnprintf(char* _str, size_t _count, const char* _format, va_list _argList);

	/// Cross platform implementation of vsnwprintf that returns number of
	/// characters which would have been written to the final string if
	/// enough space had been available.
	int32_t vsnwprintf(wchar_t* _str, size_t _count, const wchar_t* _format, va_list _argList);

	///
	int32_t snprintf(char* _str, size_t _count, const char* _format, ...);

	///
	int32_t swnprintf(wchar_t* _out, size_t _count, const wchar_t* _format, ...);

	///
	template <typename Ty>
	void stringPrintfVargs(Ty& _out, const char* _format, va_list _argList);

	///
	template <typename Ty>
	void stringPrintf(Ty& _out, const char* _format, ...);

	/// Replace all instances of substring.
	template <typename Ty>
	Ty replaceAll(const Ty& _str, const char* _from, const char* _to);

	/// Extract base file name from file path.
	const char* baseName(const char* _filePath);

	/// Convert size in bytes to human readable string.
	void prettify(char* _out, size_t _count, uint64_t _size);

	/// Copy src to string dst of size siz.  At most siz-1 characters
	/// will be copied.  Always NUL terminates (unless siz == 0).
	/// Returns strlen(src); if retval >= siz, truncation occurred.
	size_t strlcpy(char* _dst, const char* _src, size_t _siz);

	/// Appends src to string dst of size siz (unlike strncat, siz is the
	/// full size of dst, not space left).  At most siz-1 characters
	/// will be copied.  Always NUL terminates (unless siz <= strlen(dst)).
	/// Returns strlen(src) + MIN(siz, strlen(initial dst)).
	/// If retval >= siz, truncation occurred.
	size_t strlcat(char* _dst, const char* _src, size_t _siz);

	///
	uint32_t hashMurmur2A(const StringView& _data);

	///
	uint32_t hashMurmur2A(const char* _data);

} // namespace bx

#include "string.inl"

#endif // BX_STRING_H_HEADER_GUARD
