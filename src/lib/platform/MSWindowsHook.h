/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2012 - 2016 Synergy App Ltd
 * SPDX-FileCopyrightText: (C) 2011 Chris Schoeneman
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#pragma once

#include <stdint.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define DESKFLOW_MSG_MARK WM_APP + 0x0011         // mark id; <unused>
#define DESKFLOW_MSG_KEY WM_APP + 0x0012          // vk code; key data
#define DESKFLOW_MSG_MOUSE_BUTTON WM_APP + 0x0013 // button msg; <unused>
#define DESKFLOW_MSG_MOUSE_WHEEL WM_APP + 0x0014  // delta; <unused>
#define DESKFLOW_MSG_MOUSE_MOVE WM_APP + 0x0015   // x; y
#define DESKFLOW_MSG_POST_WARP WM_APP + 0x0016    // <unused>; <unused>
#define DESKFLOW_MSG_PRE_WARP WM_APP + 0x0017     // x; y
#define DESKFLOW_MSG_SCREEN_SAVER WM_APP + 0x0018 // activated; <unused>
#define DESKFLOW_MSG_DEBUG WM_APP + 0x0019        // data, data
#define DESKFLOW_MSG_INPUT_FIRST DESKFLOW_MSG_KEY
#define DESKFLOW_MSG_INPUT_LAST DESKFLOW_MSG_PRE_WARP
#define DESKFLOW_HOOK_LAST_MSG DESKFLOW_MSG_DEBUG

#define DESKFLOW_HOOK_FAKE_INPUT_VIRTUAL_KEY VK_CANCEL
#define DESKFLOW_HOOK_FAKE_INPUT_SCANCODE 0

// HOZO 2026-11-01: 给 Deskflow 自己注入的键鼠事件打标记。
// 低级钩子用 dwExtraInfo 区分两种注入：
//   1) Deskflow 自己的注入（本标记）—— 保持原有行为，可继续转发给对端；
//   2) 其它程序注入的键鼠（AI 自动化、RustDesk/向日葵等远程控制、输入法工具等）
//      —— 一律放行，只在本地生效，不再转发到对端（Mac），避免串键。
static const ULONG_PTR kHozoDeskflowInputTag = static_cast<ULONG_PTR>(0x484F5A4F); // 'HOZO'

enum EHookResult
{
  kHOOK_FAILED,
  kHOOK_OKAY,
  kHOOK_OKAY_LL
};

enum EHookMode
{
  kHOOK_DISABLE,
  kHOOK_WATCH_JUMP_ZONE,
  kHOOK_RELAY_EVENTS
};

//! Loads and provides functions for the Windows hook
class MSWindowsHook
{
public:
  MSWindowsHook() = default;
  ~MSWindowsHook();

  void loadLibrary();

  int init(DWORD threadID);

  int cleanup();

  void setSides(uint32_t sides);
  uint32_t getSides();

  void setZone(int32_t x, int32_t y, int32_t w, int32_t h, int32_t jumpZoneSize);

  void setMode(EHookMode mode);

  //! Copy the hook's physical key state; false until it has observed an event.
  static bool getPhysicalKeyState(BYTE keys[256]);

  static EHookResult install();

  static int uninstall();

  static int installScreenSaver();

  static int uninstallScreenSaver();
};
