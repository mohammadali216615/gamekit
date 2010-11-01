#include "StdAfx.h"

#define TEST_CASE_NAME testUtHashTable

TEST(TEST_CASE_NAME, testSize)
{
	utHashTable<utIntHashKey, int> table1;

	const int count = 1000;
	for (int i = 0; i < count; i++)
		table1.insert(i, i);

	EXPECT_EQ(table1.size(), count);
}

TEST(TEST_CASE_NAME, testNestedType)
{
	typedef utHashTable<utHashedString, utString> tEntry;
	typedef utHashTable<utHashedString, tEntry> tSection;
	typedef utHashTable<utHashedString, tSection> tGroup;

	tEntry entry;
	tSection section;
	tGroup group;

	entry.insert("key1", "value1");
	section.insert("key2", entry);
	group.insert("key3", section);

	EXPECT_TRUE(entry.find("key1") != UT_NPOS);
	EXPECT_TRUE(section.find("key2") != UT_NPOS);
	EXPECT_TRUE(group.find("key3") != UT_NPOS);

	//EXPECT_STREQ(group.get("key3")->get("key2")->get("key1")->c_str(), "value1");
}
