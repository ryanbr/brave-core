/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVELEDGER_MEDIA_VIMEO_H_
#define BRAVELEDGER_MEDIA_VIMEO_H_

#include <map>
#include <string>

#include "base/gtest_prod_util.h"
#include "bat/ledger/ledger.h"
#include "bat/ledger/internal/media/helper.h"

namespace bat_ledger {
class LedgerImpl;
}

namespace braveledger_media {

class MediaVimeo : public ledger::LedgerCallbackHandler {
 public:
  explicit MediaVimeo(bat_ledger::LedgerImpl* ledger);

  ~MediaVimeo() override;

  void ProcessMedia(const std::map<std::string, std::string>& parts,
                    const ledger::VisitData& visit_data);

  static std::string GetLinkType(const std::string& url);

  void ProcessActivityFromUrl(uint64_t window_id,
                              const ledger::VisitData& visit_data);

 private:

  bat_ledger::LedgerImpl* ledger_;  // NOT OWNED

  // For testing purposes
  friend class MediaVimeoTest;
};

}  // namespace braveledger_media

#endif  // BRAVELEDGER_MEDIA_VIMEO_H_
