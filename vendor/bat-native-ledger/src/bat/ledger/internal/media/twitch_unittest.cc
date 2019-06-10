/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <utility>

#include "bat/ledger/internal/media/twitch.h"
#include "bat/ledger/ledger.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- brave_unit_tests --filter=MediaTwitchTest.*

namespace braveledger_media {

class MediaTwitchTest : public testing::Test {
};

const char profile_html[] =
    "<div class=\"channel-header__banner-toggle channel-header__user "
    "channel-header__user--selected tw-align-items-center tw-flex "
    "tw-flex-nowrap tw-flex-shrink-0 tw-pd-r-2 tw-pd-y-05\" "
    "data-target=\"channel-header__channel-link\" "
    "data-a-target=\"user-channel-header-item\">"
    "<div class=\"tw-align-items-center tw-flex tw-flex-nowrap "
    "tw-flex-shrink-0\"><div><div class=\"channel-header__user-avatar "
    "channel-header__user-avatar--active tw-align-items-stretch "
    "tw-flex tw-flex-shrink-0 tw-mg-r-1\"><div "
    "class=\"channel-header__avatar-dropdown tw-relative\"><figure "
    "class=\"tw-avatar tw-avatar--size-36\"><div "
    "class=\"tw-border-radius-rounded tw-overflow-hidden\"><img "
    "class=\"tw-avatar__img tw-image\" alt=\"bravesoftware\" "
    "src=\"https://static-cdn.jtvnw.net/user-default-pictures/"
    "0ecbb6c3-fecb-4016-8115-aa467b7c36ed-profile_image-70x70.jpg\">"
    "</div></figure></div></div></div><h5 class=\"\">bravesoftware</h5>"
    "</div></div><a class=\"channel-header__item tw-align-items-center "
    "tw-flex-shrink-0 tw-interactive tw-link tw-link--hover-underline-none\" "
    "data-target=\"channel-header-item\" data-a-target=\"videos-channel-header"
    "-item\" data-test-selector=\"videos-channel-header-item\" "
    "href=\"/bravesoftware/videos\"><div class=\"tw-flex tw-pd-x-2 "
    "tw-pd-y-05\"><span class=\"tw-font-size-5\">Videos</span><div "
    "class=\"channel-header__item-count tw-flex tw-mg-l-05\"><span "
    "class=\"tw-font-size-5\">0</span></div></div></a></div>";

TEST(MediaTwitchTest, GetMediaIdFromParts) {
  std::string media_id;
  std::string user_id;
  // empty
  std::pair<std::string, std::string> result =
      MediaTwitch::GetMediaIdFromParts({});
  media_id = result.first;
  user_id = result.second;
  EXPECT_TRUE(user_id.empty());
  ASSERT_TRUE(media_id.empty());

  // event is not on the list
  result = MediaTwitch::GetMediaIdFromParts({
    {"event", "test"},
    {"properties", ""}
  });
  media_id = result.first;
  user_id = result.second;
  EXPECT_TRUE(user_id.empty());
  ASSERT_TRUE(media_id.empty());

  // properties are missing
  result = MediaTwitch::GetMediaIdFromParts({
    {"event", "minute-watched"}
  });
  media_id = result.first;
  user_id = result.second;
  EXPECT_TRUE(user_id.empty());
  ASSERT_TRUE(media_id.empty());

  // channel is missing
  result = MediaTwitch::GetMediaIdFromParts({
    {"event", "minute-watched"},
    {"properties", ""}
  });
  media_id = result.first;
  user_id = result.second;
  EXPECT_TRUE(user_id.empty());
  ASSERT_TRUE(media_id.empty());

  // channel is provided
  result = MediaTwitch::GetMediaIdFromParts({
    {"event", "minute-watched"},
    {"properties", ""},
    {"channel", "bravesoftware"}
  });
  media_id = result.first;
  user_id = result.second;
  EXPECT_EQ(user_id, "bravesoftware");
  ASSERT_EQ(media_id, "bravesoftware");

  // vod is missing leading v
  result = MediaTwitch::GetMediaIdFromParts({
    {"event", "minute-watched"},
    {"properties", ""},
    {"channel", "bravesoftware"},
    {"vod", "123312312"}
  });
  media_id = result.first;
  user_id = result.second;
  EXPECT_EQ(user_id, "bravesoftware");
  ASSERT_EQ(media_id, "bravesoftware");

  // vod is provided
  result = MediaTwitch::GetMediaIdFromParts({
    {"event", "minute-watched"},
    {"properties", ""},
    {"channel", "bravesoftware"},
    {"vod", "v123312312"}
  });
  media_id = result.first;
  user_id = result.second;
  EXPECT_EQ(user_id, "bravesoftware");
  ASSERT_EQ(media_id, "bravesoftware_vod_123312312");

  // live stream username has '_'
  result = MediaTwitch::GetMediaIdFromParts({
    {"event", "minute-watched"},
    {"properties", ""},
    {"channel", "anatomyz_2"}
  });
  media_id = result.first;
  user_id = result.second;
  EXPECT_EQ(user_id, "anatomyz_2");
  ASSERT_EQ(media_id, "anatomyz_2");

  // vod has '_'
  result = MediaTwitch::GetMediaIdFromParts({
    {"event", "minute-watched"},
    {"properties", ""},
    {"channel", "anatomyz_2"},
    {"vod", "v123312312"}
  });
  media_id = result.first;
  user_id = result.second;
  EXPECT_EQ(user_id, "anatomyz_2");
  ASSERT_EQ(media_id, "anatomyz_2_vod_123312312");
}

TEST(MediaTwitchTest, GetMediaURL) {
  // empty
  std::string result = MediaTwitch::GetMediaURL("");
  ASSERT_EQ(result, "");

  // all ok
  result = MediaTwitch::GetMediaURL("bravesoftware");
  ASSERT_EQ(result, "https://www.twitch.tv/bravesoftware");
}

TEST(MediaTwitchTest, GetTwitchStatus) {
  // empty
  ledger::MediaEventInfo old_event;
  ledger::MediaEventInfo new_event;
  std::string result = MediaTwitch::GetTwitchStatus(old_event, new_event);
  ASSERT_EQ(result, "playing");

  // user paused the video
  old_event.event_ = "video_pause";
  old_event.status_ = "playing";
  new_event.event_ = "video_pause";
  result = MediaTwitch::GetTwitchStatus(old_event, new_event);
  ASSERT_EQ(result, "paused");

  // user seeked while video was paused
  old_event.status_ = "paused";
  new_event.event_ = "player_click_vod_seek";
  result = MediaTwitch::GetTwitchStatus(old_event, new_event);
  ASSERT_EQ(result, "paused");

  // user skipped the video that was playing
  old_event.status_ = "playing";
  old_event.event_ = "video_pause";
  new_event.event_ = "player_click_vod_seek";
  result = MediaTwitch::GetTwitchStatus(old_event, new_event);
  ASSERT_EQ(result, "playing");

  // user pauses a video, then seeks it and plays it again
  old_event.status_ = "paused";
  old_event.event_ = "player_click_vod_seek";
  new_event.event_ = "video_pause";
  result = MediaTwitch::GetTwitchStatus(old_event, new_event);
  ASSERT_EQ(result, "playing");
}

TEST(MediaTwitchTest, GetMediaIdFromUrl) {
  // for live stream
  std::string result = MediaTwitch::GetMediaIdFromUrl(
      "https://www.twitch.tv/bravesoftware", profile_html);

  ASSERT_EQ(result, "bravesoftware");

  // longer url
  result = MediaTwitch::GetMediaIdFromUrl(
      "https://www.twitch.tv/bravesoftware/clips", profile_html);

  ASSERT_EQ(result, "bravesoftware");

  // video
  result = MediaTwitch::GetMediaIdFromUrl(
      "https://www.twitch.tv/videos/11111", profile_html);

  ASSERT_EQ(result, "bravesoftware");
}

TEST(MediaTwitchTest, GetLinkType) {
  const std::string url("https://k8923479-sub.cdn.ttvnw.net/v1/segment/");

  // url is not correct
  std::string result = MediaTwitch::GetLinkType("https://brave.com",
                                                "https://www.twitch.tv",
                                                "");
  ASSERT_EQ(result, "");

  // first party is off
  result = MediaTwitch::GetLinkType(url, "https://www.brave.com", "");
  ASSERT_EQ(result, "");

  // regular page
  result = MediaTwitch::GetLinkType(url, "https://www.twitch.tv/", "");
  ASSERT_EQ(result, "twitch");

  // mobile page
  result = MediaTwitch::GetLinkType(url, "https://m.twitch.tv/", "");
  ASSERT_EQ(result, "twitch");

  // player page
  result = MediaTwitch::GetLinkType(url,
                                    "https://brave.com/",
                                    "https://player.twitch.tv/");
  ASSERT_EQ(result, "twitch");
}

TEST(MediaTwitchTest, GetMediaKeyFromUrl) {
  // id is empty
  std::string result = MediaTwitch::GetMediaKeyFromUrl("", "");
  ASSERT_EQ(result, "");

  // id is twitch
  result = MediaTwitch::GetMediaKeyFromUrl("twitch", "");
  ASSERT_EQ(result, "");

  // get vod id
  result = MediaTwitch::GetMediaKeyFromUrl(
      "bravesoftware",
      "https://www.twitch.tv/videos/411403500");
  ASSERT_EQ(result, "twitch_bravesoftware_vod_411403500");

  // regular id
  result = MediaTwitch::GetMediaKeyFromUrl("bravesoftware", "");
  ASSERT_EQ(result, "twitch_bravesoftware");
}

TEST(MediaTwitchTest, GetPublisherKey) {
  // empty
  std::string result = MediaTwitch::GetPublisherKey("");
  ASSERT_EQ(result, "");

  // all ok
  result = MediaTwitch::GetPublisherKey("key");
  ASSERT_EQ(result, "twitch#author:key");
}

TEST(MediaTwitchTest, GetPublisherName) {
  // blob is not correct
  std::string result = MediaTwitch::GetPublisherName("dfsfsdfsdfds");
  ASSERT_EQ(result, "");

  // all ok
  result = MediaTwitch::GetPublisherName(profile_html);
  ASSERT_EQ(result, "bravesoftware");
}

TEST(MediaTwitchTest, GetFaviconUrl) {
  // handler is empty
  std::string result = MediaTwitch::GetFaviconUrl(profile_html, "");
  ASSERT_EQ(result, "");

  // blob is not correct
  result = MediaTwitch::GetFaviconUrl("dfsfsdfsdfds", "bravesoftware");
  ASSERT_EQ(result, "");

  // all ok
  result = MediaTwitch::GetFaviconUrl(profile_html, "bravesoftware");
  ASSERT_EQ(result,
      "https://static-cdn.jtvnw.net/user-default-pictures/"
      "0ecbb6c3-fecb-4016-8115-aa467b7c36ed-profile_image-70x70.jpg");
}

TEST(MediaTwitchTest, UpdatePublisherData) {
  // blob is not correct
  std::string name;
  std::string favicon_url;
  MediaTwitch::UpdatePublisherData(
      &name,
      &favicon_url,
      "dfsfsdfsdfds");

  ASSERT_EQ(name, "");
  ASSERT_EQ(favicon_url, "");

  // all ok
  MediaTwitch::UpdatePublisherData(
      &name,
      &favicon_url,
      profile_html);

  ASSERT_EQ(name, "bravesoftware");
  ASSERT_EQ(favicon_url,
      "https://static-cdn.jtvnw.net/user-default-pictures/"
      "0ecbb6c3-fecb-4016-8115-aa467b7c36ed-profile_image-70x70.jpg");
}

}  // namespace braveledger_media
