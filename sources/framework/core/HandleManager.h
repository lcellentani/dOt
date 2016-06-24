#ifndef __DOT_HANDLEMANAGER_H_HEADER_GUARD__
#define __DOT_HANDLEMANAGER_H_HEADER_GUARD__

#include "core/Handle.h"

namespace dot
{
namespace core
{

template <typename DATA, typename HANDLE>
class HandleManager
{
public:
	HandleManager() { }
	~HandleManager() { }

	// acquisition
	DATA* Acquire(HANDLE& handle)
	{
		uint16_t index;
		if (mFreeSlots.empty())
		{
			index = mMagicNumbers.size();
			handle.Init(index);
			mUserData.push_back(DATA());
			mMagicNumbers.push_back(handle.GetMagic());
		}
		else
		{
			index = mFreeSlots.back();
			handle.Init(index);
			mFreeSlots.pop_back();
			mMagicNumbers[index] = handle.GetMagic();
		}
		return (mUserData.begin() + index);
	}
	void Release(HANDLE handle)
	{
		unsigned int index = handle.GetIndex();

		//assert(index < m_UserData.size());
		//
		// ok remove it – tag as unused and add to free list
		mMagicNumbers[index] = 0;
		mFreeSlots.push_back(index);
	}

	DATA* Dereference(HANDLE handle)
	{
		if (handle.IsNull()) return (0);

		// check handle validity – $ this check can be removed for speed
		// if you can assume all handle references are always valid.
		uint16_t index = handle.GetIndex();
		if ((index >= mUserData.size()) || (mMagicNumbers[index] != handle.GetMagic()))
		{
			// no good! invalid handle == client programming error
			return (0);
		}

		return (mUserData.begin() + index);
	}
	const DATA* Dereference(HANDLE handle) const
	{
		// this lazy cast is ok – non-const version does not modify anything
		typedef HandleMgr <DATA, HANDLE> ThisType;
		return (const_cast <ThisType *> (this)->Dereference(handle));
	}

	// other query
	unsigned int GetUsedHandleCount(void) const
	{
		return (m_MagicNumbers.size() – m_FreeSlots.size());
	}
	bool HasUsedHandles(void) const
	{
		return (!!GetUsedHandleCount());
	}

private:
	std::vector <DATA> mUserData; // data we’re going to get to
	std::vector<uint16_t> mMagicNumbers; // corresponding magic numbers
	std::vector<uint16_t> mFreeSlots; // keeps track of free slots in the db
};

} // namespace core
} // namespace dot

#endif // __DOT_HANDLEMANAGER_H_HEADER_GUARD__
