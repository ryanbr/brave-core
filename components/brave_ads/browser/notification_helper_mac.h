/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BROWSER_BRAVE_ADS_NOTIFICATION_HELPER_MAC_H_
#define BRAVE_COMPONENTS_BROWSER_BRAVE_ADS_NOTIFICATION_HELPER_MAC_H_

#include "brave/components/brave_ads/browser/notification_helper.h"

namespace brave_ads {

class NotificationHelperMac : public NotificationHelper {
 public:
  NotificationHelperMac();
  ~NotificationHelperMac() override;

  static NotificationHelperMac* GetInstance();

 private:
  friend struct base::DefaultSingletonTraits<NotificationHelperMac>;

  // NotificationHelper impl
  bool IsNotificationsAvailable() const override;

  DISALLOW_COPY_AND_ASSIGN(NotificationHelperMac);
};

}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BROWSER_BRAVE_ADS_NOTIFICATION_HELPER_MAC_H_
