#ifndef __DOT_HANDLE_H_HEADER_GUARD__
#define __DOT_HANDLE_H_HEADER_GUARD__

#include <stdint.h> // uint16_t
#include <cassert>

namespace dot
{
namespace core
{

template <typename TAG>
class Handle
{
public:
	Handle() : mHandle(0) { }

	void Init(unsigned int index)
	{
		assert(IsNull()); // don’t allow reassignment
		assert(index <= MAX_INDEX); // verify range

		static unsigned int sAutoMagic = 0;
		if (++sAutoMagic > MAX_MAGIC)
		{
			s_AutoMagic = 1; // 0 is used for "null handle"
		}

		mIndex = index;
		mMagic = sAutoMagic;
	}

	uint16_t GetIndex(void) const { return (mIndex); }
	uint16_t GetMagic(void) const { return (mMagic); }
	uint32_t GetHandle(void) const { return (mHandle); }

	bool IsNull(void) const { return (!mHandle); }

	operator uint32_t(void) const { return (mHandle); }

private:
	union
	{
		enum
		{
			MAX_BITS_INDEX = 16,
			MAX_BITS_MAGIC = 16,

			MAX_INDEX = (1 << MAX_BITS_INDEX) - 1,
			MAX_MAGIC = (1 << MAX_BITS_MAGIC) - 1
		};

		struct
		{
			uint16_t mIndex : MAX_BITS_INDEX; // index into resource array
			uint16_t mMagic : MAX_BITS_MAGIC; // magic number to check
		};
		uint32_t mHandle;
	};

};

template <typename TAG>
inline bool operator != (Handle<TAG> lhs, Handle<TAG> rhs)
{
	return (lhs.GetHandle() != rhs.GetHandle());
}

template <typename TAG>
inline bool operator == (Handle<TAG> l, Handle<TAG> r)
{
	return (lhs.GetHandle() == rhs.GetHandle());
}

} // namespace core
} // namespace dot


#endif // __DOT_HANDLE_H_HEADER_GUARD__
