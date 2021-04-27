/*
 * Copyright 2021 The Imaging Source Europe GmbH
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

namespace tcam::property
{

enum class PropertyFlags
{
    None = 0x0,
    Implemented = 1,
    Available = 2,
    Locked = 4,
    // additional flags like 'External' to indicate library and not camera internal properties?
};


inline PropertyFlags operator|(PropertyFlags a, PropertyFlags b)
{
    return static_cast<PropertyFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline PropertyFlags operator~(PropertyFlags a)
{
    return static_cast<PropertyFlags>(~static_cast<int>(a));
}

// inline PropertyFlags operator&(PropertyFlags a, PropertyFlags b)
// {
//     return static_cast<PropertyFlags>(static_cast<int>(a) & static_cast<int>(b));
// }

inline bool operator&(PropertyFlags a, PropertyFlags b)
{
    return static_cast<bool>(static_cast<int>(a) & static_cast<int>(b));
}

inline PropertyFlags operator^(PropertyFlags a, PropertyFlags b)
{
    return static_cast<PropertyFlags>(static_cast<int>(a) ^ static_cast<int>(b));
}

inline PropertyFlags& operator|=(PropertyFlags& a, PropertyFlags b)
{
    return (PropertyFlags&)((int&)a |= (int)b);
}

inline PropertyFlags& operator&=(PropertyFlags& a, PropertyFlags b)
{
    return (PropertyFlags&)((int&)a &= ~(int)b);
}

inline PropertyFlags& operator^=(PropertyFlags& a, PropertyFlags b)
{
    return (PropertyFlags&)((int&)a ^= (int)b);
}

inline bool is_implemented(PropertyFlags flags) noexcept
{
    if (flags & PropertyFlags::Implemented)
    {
        return true;
    }
    return false;
}

inline bool is_locked(PropertyFlags flags) noexcept
{
    if (flags & PropertyFlags::Locked)
    {
        return true;
    }
    return false;
}

} // namespace tcam::property
