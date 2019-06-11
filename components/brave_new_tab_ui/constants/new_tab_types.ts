/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

export const enum types {
  BOOKMARK_ADDED = '@@newtab/BOOKMARK_ADDED',
  BOOKMARK_REMOVED = '@@newtab/BOOKMARK_REMOVED',
  NEW_TAB_TOP_SITES_DATA_UPDATED = '@@newtab/NEW_TAB_TOP_SITES_DATA_UPDATED',
  NEW_TAB_SITE_PINNED = '@@newtab/NEW_TAB_SITE_PINNED',
  NEW_TAB_SITE_UNPINNED = '@@newtab/NEW_TAB_SITE_UNPINNED',
  NEW_TAB_SITE_IGNORED = '@@newtab/NEW_TAB_SITE_IGNORED',
  NEW_TAB_UNDO_SITE_IGNORED = '@@newtab/NEW_TAB_UNDO_SITE_IGNORED',
  NEW_TAB_UNDO_ALL_SITE_IGNORED = '@@newtab/NEW_TAB_UNDO_ALL_SITE_IGNORED',
  NEW_TAB_SITE_DRAGGED = '@@newtab/NEW_TAB_SITE_DRAGGED',
  NEW_TAB_SITE_DRAG_END = '@@newtab/NEW_TAB_SITE_DRAG_END',
  NEW_TAB_HIDE_SITE_REMOVAL_NOTIFICATION = '@@newtab/NEW_TAB_HIDE_SITE_REMOVAL_NOTIFICATION',
  NEW_TAB_BOOKMARK_INFO_AVAILABLE = '@@newtab/NEW_TAB_BOOKMARK_INFO_AVAILABLE',
  NEW_TAB_GRID_SITES_UPDATED = '@@newtab/NEW_TAB_GRID_SITES_UPDATED',
  NEW_TAB_STATS_UPDATED = '@@newtab/NEW_TAB_STATS_UPDATED',
  NEW_TAB_USE_ALTERNATIVE_PRIVATE_SEARCH_ENGINE = '@@newtab/NEW_TAB_USE_ALTERNATIVE_PRIVATE_SEARCH_ENGINE',
  NEW_TAB_SHOW_SETTINGS_MENU = '@@newtab/NEW_TAB_STATS_UPDATED',
  NEW_TAB_CLOSE_SETTINGS_MENU = '@@newtab/NEW_TAB_CLOSE_SETTINGS_MENU',
  NEW_TAB_TOGGLE_SHOW_BACKGROUND_IMAGE = '@@newtab/NEW_TAB_TOGGLE_SHOW_BACKGROUND_IMAGE'
}
