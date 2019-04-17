/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ledger/internal/bat_helper.h"
#include "bat/ledger/internal/ledger_impl.h"
#include "bat/ledger/internal/media/vimeo.h"

using std::placeholders::_1;
using std::placeholders::_2;

namespace braveledger_media {

MediaVimeo::MediaVimeo(bat_ledger::LedgerImpl* ledger):
  ledger_(ledger) {
}

MediaVimeo::~MediaVimeo() {
}

// static
std::string MediaVimeo::GetLinkType(const std::string& url) {
  std::string type = VIMEO_MEDIA_TYPE;
  return type;
}

void MediaVimeo::ProcessMedia(const std::map<std::string, std::string>& parts,
                              const ledger::VisitData& visit_data) {

}

void MediaVimeo::ProcessActivityFromUrl(uint64_t window_id,
                                        const ledger::VisitData& visit_data) {

}

}  // namespace braveledger_media
