/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/serving/notification_ad_serving_feature.h"

#include "base/test/scoped_feature_list.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

TEST(BraveAdsNotificationAdServingFeatureTest, IsEnabled) {
  // Arrange

  // Act

  // Assert
  EXPECT_TRUE(base::FeatureList::IsEnabled(kNotificationAdServingFeature));
}

TEST(BraveAdsNotificationAdServingFeatureTest, IsDisabled) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndDisableFeature(kNotificationAdServingFeature);

  // Act

  // Assert
  EXPECT_FALSE(base::FeatureList::IsEnabled(kNotificationAdServingFeature));
}

TEST(BraveAdsNotificationAdServingFeatureTest, ServingVersion) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kNotificationAdServingFeature, {{"version", "0"}});

  // Act

  // Assert
  EXPECT_EQ(0, kNotificationAdServingVersion.Get());
}

TEST(BraveAdsNotificationAdServingFeatureTest, DefaultServingVersion) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(2, kNotificationAdServingVersion.Get());
}

TEST(BraveAdsNotificationAdServingFeatureTest,
     DefaultServingVersionWhenDisabled) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndDisableFeature(kNotificationAdServingFeature);

  // Act

  // Assert
  EXPECT_EQ(2, kNotificationAdServingVersion.Get());
}

TEST(BraveAdsNotificationAdServingFeatureTest, ServeFirstAdAfter) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kNotificationAdServingFeature, {{"serve_first_ad_after", "10m"}});

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(10), kServeFirstNotificationAdAfter.Get());
}

TEST(BraveAdsNotificationAdServingFeatureTest, DefaultServeFirstAdAfter) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(2), kServeFirstNotificationAdAfter.Get());
}

TEST(BraveAdsNotificationAdServingFeatureTest,
     DefaultServeFirstAdAfterWhenDisabled) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndDisableFeature(kNotificationAdServingFeature);

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(2), kServeFirstNotificationAdAfter.Get());
}

TEST(BraveAdsNotificationAdServingFeatureTest, MinimumDelayBeforeServingAd) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kNotificationAdServingFeature,
      {{"minimum_delay_before_serving_an_ad", "10m"}});

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(10), kMinimumDelayBeforeServingNotificationAd.Get());
}

TEST(BraveAdsNotificationAdServingFeatureTest,
     DefaultMinimumDelayBeforeServingAd) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(1), kMinimumDelayBeforeServingNotificationAd.Get());
}

TEST(BraveAdsNotificationAdServingFeatureTest,
     DefaultMinimumDelayBeforeServingAdWhenDisabled) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndDisableFeature(kNotificationAdServingFeature);

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(1), kMinimumDelayBeforeServingNotificationAd.Get());
}

TEST(BraveAdsNotificationAdServingFeatureTest, RetryServingAdAfter) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kNotificationAdServingFeature, {{"retry_serving_ad_after", "10m"}});

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(10), kRetryServingNotificationAdAfter.Get());
}

TEST(BraveAdsNotificationAdServingFeatureTest, DefaultRetryServingAdAfter) {
  // Arrange

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(2), kRetryServingNotificationAdAfter.Get());
}

TEST(BraveAdsNotificationAdServingFeatureTest,
     DefaultRetryServingAdAfterWhenDisabled) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndDisableFeature(kNotificationAdServingFeature);

  // Act

  // Assert
  EXPECT_EQ(base::Minutes(2), kRetryServingNotificationAdAfter.Get());
}

}  // namespace brave_ads
