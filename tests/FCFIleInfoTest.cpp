#include "../src/FCFileInfo.hpp"
#include "gtest/gtest.h"

const std::string testFilePath = "/tmp/testfile.txt";
const std::string testFileAcls = "rwx";
const std::string testFileCaps = "cap_sys_admin";
const uint64_t testFileSize = 1234;
const uint64_t testFileCrc = 123;
const uint32_t testFilePerms = 12;
const FCFileType testFileType = FCFileType::REG_FILE;
const uint32_t testFileOwner = 1001;
const uint32_t testFileOwnerGroup = 20;

class FCFileInfoTest : public ::testing::Test
{
protected:
    FCFileInfoTest()
    {
        // set-up work for each test here.
    }

    ~FCFileInfoTest() override
    {
        // clean-up work that doesn't throw exceptions here.
    }
    void SetUp() override
    {
        std::cout << "setup" << std::endl;
    }

    void TearDown() override
    {
        std::cout << "teardown" << std::endl;
    }
};

TEST_F(FCFileInfoTest, check_factory_not_from_fs)
{

    FCFileInfo fileEntry = FCFileInfo::FCFileInfoFactory::constructFCFileInfo(testFilePath,
                                                                              testFileAcls,
                                                                              testFileCaps,
                                                                              testFileSize,
                                                                              testFileCrc,
                                                                              testFilePerms,
                                                                              testFileType,
                                                                              testFileOwner,
                                                                              testFileOwnerGroup);
    ASSERT_EQ(fileEntry.getFilePath(), testFilePath);
    ASSERT_EQ(fileEntry.getFileAcls(), testFileAcls);
    ASSERT_EQ(fileEntry.getFileCaps(), testFileCaps);
    ASSERT_EQ(fileEntry.getFileSize(), testFileSize);
    ASSERT_EQ(fileEntry.getFileCrc(), testFileCrc);
    ASSERT_EQ(fileEntry.getFilePerms(), testFilePerms);
    ASSERT_EQ(fileEntry.getFileType(), testFileType);
    ASSERT_EQ(fileEntry.getFileOwner(), testFileOwner);
    ASSERT_EQ(fileEntry.getFileOwnerGroup(), testFileOwnerGroup);
}