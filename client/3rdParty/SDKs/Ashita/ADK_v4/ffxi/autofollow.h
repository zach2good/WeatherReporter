/**
 * Ashita SDK - Copyright (c) 2023 Ashita Development Team
 * Contact: https://www.ashitaxi.com/
 * Contact: https://discord.gg/Ashita
 *
 * This file is part of Ashita.
 *
 * Ashita is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ashita is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Ashita.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef ASHITA_SDK_FFXI_AUTOFOLLOW_H_INCLUDED
#define ASHITA_SDK_FFXI_AUTOFOLLOW_H_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

// clang-format off

#include <cinttypes>

namespace Ashita::FFXI
{
    struct autofollow_t
    {
        // CXiAttachment: XiControlActor::user_control_target
        uintptr_t       VTablePointer;          // The base VTable pointer.
        uint32_t        TargetIndex;            // The last targeted entities target index.
        uint32_t        TargetServerId;         // The last targeted entities server id.
        float           FollowDeltaX;           // The auto-run/auto-follow X delta.
        float           FollowDeltaZ;           // The auto-run/auto-follow Z delta.
        float           FollowDeltaY;           // The auto-run/auto-follow Y delta.
        float           FollowDeltaW;           // The auto-run/auto-follow W delta. [Generally set to 1.]

        // CXiAttachment: XiControlActor::follow_actor
        uintptr_t       VTablePointer2;         // The base VTable pointer.
        uint32_t        FollowTargetIndex;      // The auto-follow follow target index.
        uint32_t        FollowTargetServerId;   // The auto-follow follow target server id.
        uint8_t         IsFirstPersonCamera;    // Flag if the camrea is in first person mode or node.
        uint8_t         IsAutoRunning;          // Flag if the player is auto-running / auto-following. (Or auto-running in a direction.)
        uint16_t        Padding0000;            // Padding
        uint32_t        Unknown0000;            // Unknown [Set to 0 when auto-following and the delta x and z becomes 0.]
        uint8_t         IsCameraLocked;         // Set to 1 when the camera is locked forward. (When holding shift.)
        uint8_t         IsCameraLockedOn;       // Set to 1 when the camera is locked on to a target and is in third-person mode.
        uint16_t        Padding0001;            // Padding
    };

} // namespace Ashita::FFXI

#endif // ASHITA_SDK_FFXI_AUTOFOLLOW_H_INCLUDED