// Copyright (C) 2017 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#define LOG_TAG "statsd_test"

#include "frameworks/base/cmds/statsd/src/statsd_config.pb.h"
#include "../src/matchers/LogEntryMatcherManager.h"
#include "../src/parse_util.h"
#include <log/logprint.h>
#include <log/log_read.h>
#include <log/log_event_list.h>
#include <gtest/gtest.h>

#include <stdio.h>

using namespace android::os::statsd;
using std::unordered_map;

#ifdef __ANDROID__
TEST(LogEntryMatcherTest, TestSimpleMatcher) {
    // Set up the matcher
    LogEntryMatcher matcher;
    auto simpleMatcher = matcher.mutable_simple_log_entry_matcher();
    simpleMatcher->add_tag(TagId::WAKELOCK);

    unordered_map<int, long> intMap;
    unordered_map<int, string> strMap;
    unordered_map<int, float> floatMap;
    unordered_map<int, bool> boolMap;

    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap, boolMap));
}

TEST(LogEntryMatcherTest, TestBoolMatcher) {
    // Set up the matcher
    LogEntryMatcher matcher;
    auto simpleMatcher = matcher.mutable_simple_log_entry_matcher();
    simpleMatcher->add_tag(TagId::WAKELOCK);
    auto keyValue = simpleMatcher->add_key_value_matcher();
    keyValue->mutable_key_matcher()->set_key(KeyId::STATE);


    unordered_map<int, long> intMap;
    unordered_map<int, string> strMap;
    unordered_map<int, float> floatMap;
    unordered_map<int, bool> boolMap;

    keyValue->set_eq_bool(true);
    boolMap[KeyId::STATE] = true;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));

    keyValue->set_eq_bool(false);
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));

    boolMap[TagId::WAKELOCK] = false;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
}

TEST(LogEntryMatcherTest, TestStringMatcher) {
    // Set up the matcher
    LogEntryMatcher matcher;
    auto simpleMatcher = matcher.mutable_simple_log_entry_matcher();
    simpleMatcher->add_tag(TagId::WAKELOCK);
    auto keyValue = simpleMatcher->add_key_value_matcher();
    keyValue->mutable_key_matcher()->set_key(KeyId::STATE);
    keyValue->set_eq_string("wakelock_name");

    unordered_map<int, long> intMap;
    unordered_map<int, string> strMap;
    unordered_map<int, float> floatMap;
    unordered_map<int, bool> boolMap;

    strMap[KeyId::STATE] = "wakelock_name";

    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap, boolMap));
}

TEST(LogEntryMatcherTest, TestIntComparisonMatcher) {
    // Set up the matcher
    LogEntryMatcher matcher;
    auto simpleMatcher = matcher.mutable_simple_log_entry_matcher();
    simpleMatcher->add_tag(TagId::WAKELOCK);
    auto keyValue = simpleMatcher->add_key_value_matcher();
    keyValue->mutable_key_matcher()->set_key(KeyId::STATE);

    unordered_map<int, long> intMap;
    unordered_map<int, string> strMap;
    unordered_map<int, float> floatMap;
    unordered_map<int, bool> boolMap;

    keyValue->set_lt_int(10);
    intMap[KeyId::STATE] = 11;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));
    intMap[KeyId::STATE] = 10;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));
    intMap[KeyId::STATE] = 9;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));

    keyValue->set_gt_int(10);
    intMap[KeyId::STATE] = 11;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));
    intMap[KeyId::STATE] = 10;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));
    intMap[KeyId::STATE] = 9;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));
}

TEST(LogEntryMatcherTest, TestIntWithEqualityComparisonMatcher) {
    // Set up the matcher
    LogEntryMatcher matcher;
    auto simpleMatcher = matcher.mutable_simple_log_entry_matcher();
    simpleMatcher->add_tag(TagId::WAKELOCK);
    auto keyValue = simpleMatcher->add_key_value_matcher();
    keyValue->mutable_key_matcher()->set_key(KeyId::STATE);

    unordered_map<int, long> intMap;
    unordered_map<int, string> strMap;
    unordered_map<int, float> floatMap;
    unordered_map<int, bool> boolMap;

    keyValue->set_lte_int(10);
    intMap[KeyId::STATE] = 11;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));
    intMap[KeyId::STATE] = 10;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));
    intMap[KeyId::STATE] = 9;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));

    keyValue->set_gte_int(10);
    intMap[KeyId::STATE] = 11;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));
    intMap[KeyId::STATE] = 10;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));
    intMap[KeyId::STATE] = 9;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));
}

TEST(LogEntryMatcherTest, TestFloatComparisonMatcher) {
    // Set up the matcher
    LogEntryMatcher matcher;
    auto simpleMatcher = matcher.mutable_simple_log_entry_matcher();
    simpleMatcher->add_tag(TagId::WAKELOCK);
    auto keyValue = simpleMatcher->add_key_value_matcher();
    keyValue->mutable_key_matcher()->set_key(KeyId::STATE);

    unordered_map<int, long> intMap;
    unordered_map<int, string> strMap;
    unordered_map<int, float> floatMap;
    unordered_map<int, bool> boolMap;

    keyValue->set_lt_float(10.0);
    floatMap[KeyId::STATE] = 10.1;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));
    floatMap[KeyId::STATE] = 9.9;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));

    keyValue->set_gt_float(10.0);
    floatMap[KeyId::STATE] = 10.1;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));
    floatMap[KeyId::STATE] = 9.9;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap,
        floatMap, boolMap));
}

// Helper for the composite matchers.
void addSimpleMatcher(SimpleLogEntryMatcher* simpleMatcher, TagId tag, KeyId key, int val) {
    simpleMatcher->add_tag(tag);
    auto keyValue = simpleMatcher->add_key_value_matcher();
    keyValue->mutable_key_matcher()->set_key(key);
    keyValue->set_eq_int(val);
}

TEST(LogEntryMatcherTest, TestAndMatcher) {
    // Set up the matcher
    LogEntryMatcher matcher;
    auto combination = matcher.mutable_combination();
    combination->set_operation(LogicalOperation::AND);

    addSimpleMatcher(combination->add_matcher()->mutable_simple_log_entry_matcher(), TagId::WAKELOCK, KeyId::STATE, 3);
    addSimpleMatcher(combination->add_matcher()->mutable_simple_log_entry_matcher(), TagId::WAKELOCK, KeyId::PACKAGE_VERSION, 4);

    unordered_map<int, long> intMap;
    unordered_map<int, string> strMap;
    unordered_map<int, float> floatMap;
    unordered_map<int, bool> boolMap;

    intMap[1003] = 4;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap, boolMap));
    intMap.clear();
    intMap[1] = 3;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap, boolMap));
    intMap.clear();
    intMap[1] = 3;
    intMap[1003] = 4;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap, boolMap));
}

TEST(LogEntryMatcherTest, TestOrMatcher) {
    // Set up the matcher
    LogEntryMatcher matcher;
    auto combination = matcher.mutable_combination();
    combination->set_operation(LogicalOperation::OR);

    addSimpleMatcher(combination->add_matcher()->mutable_simple_log_entry_matcher(),
        TagId::WAKELOCK, KeyId::STATE, 3);
    addSimpleMatcher(combination->add_matcher()->mutable_simple_log_entry_matcher(),
        TagId::WAKELOCK, KeyId::PACKAGE_VERSION, 4);

    unordered_map<int, long> intMap;
    unordered_map<int, string> strMap;
    unordered_map<int, float> floatMap;
    unordered_map<int, bool> boolMap;

    // Don't set any key-value pairs.
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
    intMap[1003] = 4;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
    intMap.clear();
    intMap[1] = 3;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
    intMap.clear();
    intMap[1] = 3;
    intMap[1003] = 4;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
}

TEST(LogEntryMatcherTest, TestNotMatcher) {
    // Set up the matcher
    LogEntryMatcher matcher;
    auto combination = matcher.mutable_combination();
    combination->set_operation(LogicalOperation::NOT);

    // Define first simpleMatcher
    addSimpleMatcher(combination->add_matcher()->mutable_simple_log_entry_matcher(),
        TagId::WAKELOCK, KeyId::STATE, 3);

    unordered_map<int, long> intMap;
    unordered_map<int, string> strMap;
    unordered_map<int, float> floatMap;
    unordered_map<int, bool> boolMap;

    // Don't set any key-value pairs.
    intMap[KeyId::STATE] = 3;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
}

TEST(LogEntryMatcherTest, TestNANDMatcher) {
    // Set up the matcher
    LogEntryMatcher matcher;
    auto combination = matcher.mutable_combination();
    combination->set_operation(LogicalOperation::NAND);

    addSimpleMatcher(combination->add_matcher()->mutable_simple_log_entry_matcher(),
        TagId::WAKELOCK, KeyId::STATE, 3);
    addSimpleMatcher(combination->add_matcher()->mutable_simple_log_entry_matcher(),
        TagId::WAKELOCK, KeyId::PACKAGE_VERSION, 4);

    unordered_map<int, long> intMap;
    unordered_map<int, string> strMap;
    unordered_map<int, float> floatMap;
    unordered_map<int, bool> boolMap;

    // Don't set any key-value pairs.
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
    intMap[KeyId::STATE] = 3;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
    intMap[KeyId::PACKAGE_VERSION] = 4;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
}

TEST(LogEntryMatcherTest, TestNORMatcher) {
    // Set up the matcher
    LogEntryMatcher matcher;
    auto combination = matcher.mutable_combination();
    combination->set_operation(LogicalOperation::NOR);

    addSimpleMatcher(combination->add_matcher()->mutable_simple_log_entry_matcher(),
        TagId::WAKELOCK, KeyId::STATE, 3);
    addSimpleMatcher(combination->add_matcher()->mutable_simple_log_entry_matcher(),
        TagId::WAKELOCK, KeyId::PACKAGE_VERSION, 4);

    unordered_map<int, long> intMap;
    unordered_map<int, string> strMap;
    unordered_map<int, float> floatMap;
    unordered_map<int, bool> boolMap;

    // Don't set any key-value pairs.
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
    intMap[KeyId::STATE] = 3;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
    intMap[KeyId::PACKAGE_VERSION] = 4;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
}

// Tests that a NOT on top of AND is the same as NAND
TEST(LogEntryMatcherTest, TestMultipleLayerMatcher) {
    LogEntryMatcher matcher;
    auto not_combination = matcher.mutable_combination();
    not_combination->set_operation(LogicalOperation::NOT);

    // Now add the AND
    auto combination = not_combination->add_matcher()->mutable_combination();
    combination->set_operation(LogicalOperation::AND);
    addSimpleMatcher(combination->add_matcher()->mutable_simple_log_entry_matcher(),
        TagId::WAKELOCK, KeyId::STATE, 3);
    addSimpleMatcher(combination->add_matcher()->mutable_simple_log_entry_matcher(),
        TagId::WAKELOCK, KeyId::PACKAGE_VERSION, 4);

    unordered_map<int, long> intMap;
    unordered_map<int, string> strMap;
    unordered_map<int, float> floatMap;
    unordered_map<int, bool> boolMap;

    // Don't set any key-value pairs.
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
    intMap[KeyId::STATE] = 3;
    EXPECT_TRUE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
    intMap[KeyId::PACKAGE_VERSION] = 4;
    EXPECT_FALSE(LogEntryMatcherManager::matches(matcher, TagId::WAKELOCK, intMap, strMap, floatMap,
        boolMap));
}
#else
            GTEST_LOG_(INFO) << "This test does nothing.\n";
#endif
