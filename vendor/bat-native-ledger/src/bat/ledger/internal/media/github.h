
/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVELEDGER_MEDIA_GITHUB_H_
#define BRAVELEDGER_MEDIA_GITHUB_H_

#include <stdint.h>

#include <map>
#include <memory>
#include <string>

#include "base/gtest_prod_util.h"
#include "bat/ledger/ledger.h"
#include "bat/ledger/internal/media/helper.h"

namespace bat_ledger {
class LedgerImpl;
}

namespace braveledger_media {

class GitHub : public ledger::LedgerCallbackHandler {
 public:
  explicit GitHub(bat_ledger::LedgerImpl* ledger);

  void ProcessActivityFromUrl(uint64_t window_id,
                              const ledger::VisitData& visit_data);
  ~GitHub() override;

 private:
  static std::string GetUserNameFromUrl(const std::string& path);

  static std::string GetMediaKey(const std::string& screen_name);

  static std::string GetUserId(const std::string& data);  

  static std::string GetPublisherName(const std::string& data);

  static std::string GetProfileURL(const std::string& screen_name);

  static std::string GetProfileAPIURL(const std::string& screen_name);

  static std::string GetPublisherKey(const std::string& key);

  static std::string GetProfileImageURL(const std::string& key);

  static bool IsExcludedPath(const std::string& path);


  void OnMediaPublisherActivity(
      ledger::Result result,
      ledger::PublisherInfoPtr info,
      uint64_t window_id,
      const ledger::VisitData& visit_data,
      const std::string& media_key);

  void FetchDataFromUrl(
      const std::string& url,
      braveledger_media::FetchDataFromUrlCallback callback);

  void OnUserPage(
      uint64_t window_id,
      const ledger::VisitData& visit_data,
      int response_status_code,
      const std::string& response,
      const std::map<std::string, std::string>& headers);

  void OnSaveMediaVisit(
      ledger::Result result,
      ledger::PublisherInfoPtr info);
  
  void SavePublisherInfo(
      const uint64_t duration,
      const std::string& user_id,
      const std::string& screen_name,
      const std::string& publisher_name,
      const std::string& profile_picture,
      const uint64_t window_id,
      ledger::PublisherInfoCallback callback);

  void GetPublisherPanelInfo(
      uint64_t window_id,
      const ledger::VisitData& visit_data,
      const std::string& publisher_key);

  void OnPublisherPanelInfo(
      uint64_t window_id,
      const ledger::VisitData& visit_data,
      const std::string& publisher_key,
      ledger::Result result,
      ledger::PublisherInfoPtr info);

  void OnMediaActivityError(
      const ledger::VisitData& visit_data,
      uint64_t window_id);

  bat_ledger::LedgerImpl* ledger_;  // NOT OWNED
};
}
#endif