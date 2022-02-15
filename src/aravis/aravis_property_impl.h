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

#include "../PropertyInterfaces.h"
#include "../compiler_defines.h"

#include <arv.h>
#include <tcamprop1.0_base/tcamprop_property_info.h>

VISIBILITY_INTERNAL

namespace tcam::aravis
{
class AravisPropertyBackend;
}

namespace tcam::aravis
{
using namespace tcam::property;

struct aravis_backend_guard;

class prop_base_impl
{
public:
    prop_base_impl(const std::shared_ptr<AravisPropertyBackend>& cam,
                   ArvGcFeatureNode* feature_node);

protected:
    PropertyFlags get_flags_impl() const;

    aravis_backend_guard acquire_backend_guard() const noexcept;

    tcamprop1::prop_static_info_str build_static_info(
        std::string_view category,
        std::string_view name_override) const noexcept;

    auto get_access_mode() const noexcept
    {
        return access_mode_;
    }
private:
    std::weak_ptr<AravisPropertyBackend> backend_;
    ArvGcFeatureNode* feature_node_ = nullptr;

    tcamprop1::Access_t access_mode_ = tcamprop1::Access_t::RW;
};

class AravisPropertyIntegerImpl : public prop_base_impl, public IPropertyInteger
{
public:
    AravisPropertyIntegerImpl(std::string_view name,
                              std::string_view category,
                              ArvGcNode* node,
                              const std::shared_ptr<AravisPropertyBackend>&);

    tcamprop1::prop_static_info get_static_info() const final
    {
        return static_info_.to_prop_static_info();
    }
    PropertyFlags get_flags() const final
    {
        return get_flags_impl();
    }

    std::string_view get_unit() const final
    {
        return unit_;
    }
    tcamprop1::IntRepresentation_t get_representation() const final
    {
        return int_rep_;
    }

    tcamprop1::prop_range_integer get_range() const final;

    int64_t get_default() const final
    {
        return m_default;
    }
    outcome::result<int64_t> get_value() const final;

    outcome::result<void> set_value(int64_t new_value) final;

private:
    ArvGcInteger* arv_gc_node_ = nullptr;

    tcamprop1::prop_static_info_str static_info_;
    std::string unit_;
    tcamprop1::IntRepresentation_t int_rep_ = tcamprop1::IntRepresentation_t::Linear;

    int64_t m_default = 0;
};

class AravisPropertyDoubleImpl : public prop_base_impl, public IPropertyFloat
{

public:
    AravisPropertyDoubleImpl(std::string_view name,
                             std::string_view category,
                             ArvGcNode* node,
                             const std::shared_ptr<AravisPropertyBackend>&);

    tcamprop1::prop_static_info get_static_info() const final
    {
        return static_info_.to_prop_static_info();
    }
    PropertyFlags get_flags() const final
    {
        return get_flags_impl();
    }
    std::string_view get_unit() const final
    {
        return unit_;
    }
    tcamprop1::FloatRepresentation_t get_representation() const final
    {
        return float_rep_;
    }
    tcamprop1::prop_range_float get_range() const final;
    double get_default() const final
    {
        return m_default;
    }
    outcome::result<double> get_value() const final;
    outcome::result<void> set_value(double new_value) final;

private:
    ArvGcFloat* arv_gc_node_ = nullptr;

    tcamprop1::prop_static_info_str static_info_;
    std::string unit_;
    tcamprop1::FloatRepresentation_t float_rep_ = tcamprop1::FloatRepresentation_t::Linear;

    double m_default = 0;
};


class AravisPropertyBoolImpl : public prop_base_impl, public IPropertyBool
{
public:
    AravisPropertyBoolImpl(std::string_view name,
                           std::string_view category,
                           ArvGcNode* node,
                           const std::shared_ptr<AravisPropertyBackend>& backend);

    tcamprop1::prop_static_info get_static_info() const final
    {
        return static_info_.to_prop_static_info();
    }

    PropertyFlags get_flags() const final
    {
        return get_flags_impl();
    }

    bool get_default() const final
    {
        return m_default;
    }
    outcome::result<bool> get_value() const final;

    outcome::result<void> set_value(bool new_value) final;

private:
    ArvGcBoolean* arv_gc_node_ = nullptr;
    tcamprop1::prop_static_info_str static_info_;

    bool m_default = false;
};


class AravisPropertyCommandImpl : public prop_base_impl, public IPropertyCommand
{
public:
    AravisPropertyCommandImpl(std::string_view name,
                              std::string_view category,
                              ArvGcNode* node,
                              const std::shared_ptr<AravisPropertyBackend>& backend);

    tcamprop1::prop_static_info get_static_info() const final
    {
        return static_info_.to_prop_static_info();
    }

    PropertyFlags get_flags() const final
    {
        return get_flags_impl();
    }

    outcome::result<void> execute() final;

private:
    tcamprop1::prop_static_info_str static_info_;
    ArvGcCommand* arv_gc_node_ = nullptr;
};


class AravisPropertyEnumImpl : public prop_base_impl, public IPropertyEnum
{
public:
    AravisPropertyEnumImpl(std::string_view name,
                           std::string_view category,
                           ArvGcNode* node,
                           const std::shared_ptr<AravisPropertyBackend>& backend);

    tcamprop1::prop_static_info get_static_info() const final
    {
        return static_info_.to_prop_static_info();
    }

    PropertyFlags get_flags() const final
    {
        return get_flags_impl();
    }

    outcome::result<void> set_value_str(const std::string_view& new_value) final;
    outcome::result<std::string_view> get_value() const final;

    std::string get_default() const final
    {
        return m_default;
    }

    std::vector<std::string> get_entries() const final
    {
        std::vector<std::string> rval;
        for (auto& e : entries_) { rval.push_back(e.display_name); }
        return rval;
    }

private:
    tcamprop1::prop_static_info_str static_info_;
    ArvGcEnumeration* arv_gc_node_ = nullptr;

    struct enum_entry
    {
        std::string display_name;
        int64_t value;
    };

    std::vector<enum_entry> entries_;

    std::string m_default;
};

class balance_ratio_raw_to_wb_channel : public IPropertyFloat2
{
public:
    balance_ratio_raw_to_wb_channel(const std::shared_ptr<IPropertyEnum>& sel,
                                    const std::shared_ptr<IPropertyInteger>& val,
                                    const std::string& sel_entry,
                                    const tcamprop1::prop_static_info_float* info_entry)
        : selector_(sel), value_(val), selector_entry_(sel_entry), prop_entry_(info_entry)
    {
    }

    tcamprop1::prop_static_info_float get_static_info_ext() const final
    {
        return *prop_entry_;
    }

    PropertyFlags get_flags() const final
    {
        return value_->get_flags();
    }

    tcamprop1::prop_range_float get_range() const final
    {
        return { 0., 4., 1. / 64. };
    }
    double get_default() const final
    {
        return 1.0;
    }
    outcome::result<double> get_value() const final
    {
        if (auto sel_res = selector_->set_value_str(selector_entry_); !sel_res)
        {
            return sel_res.error();
        }
        auto res = value_->get_value();
        if (!res)
        {
            return res.error();
        }
        return res.value() / 64.0;
    }
    outcome::result<void> set_value(double new_value) final
    {
        if (auto sel_res = selector_->set_value_str(selector_entry_); !sel_res)
        {
            return sel_res.error();
        }
        return value_->set_value(new_value * 64.);
    }

private:
    std::shared_ptr<IPropertyEnum> selector_;
    std::shared_ptr<IPropertyInteger> value_;
    std::string selector_entry_;

    const tcamprop1::prop_static_info_float* prop_entry_ = nullptr;
};


class balance_ratio_to_wb_channel : public IPropertyFloat2
{
public:
    balance_ratio_to_wb_channel(const std::shared_ptr<IPropertyEnum>& sel,
                                const std::shared_ptr<IPropertyFloat>& val,
                                const std::string& sel_entry,
                                const tcamprop1::prop_static_info_float* info_entry)
        : selector_(sel), value_(val), selector_entry_(sel_entry), prop_entry_(info_entry)
    {
    }

    tcamprop1::prop_static_info_float get_static_info_ext() const final
    {
        return *prop_entry_;
    }

    PropertyFlags get_flags() const final
    {
        if (auto sel_res = selector_->set_value_str(selector_entry_); !sel_res)
        {
            // do nothing
            //return sel_res.error();
        }
        return value_->get_flags();
    }

    tcamprop1::prop_range_float get_range() const final
    {
        if (auto sel_res = selector_->set_value_str(selector_entry_); !sel_res)
        {
            // do nothing
            //return sel_res.error();
        }
        return value_->get_range();
    }
    double get_default() const final
    {
        if (auto sel_res = selector_->set_value_str(selector_entry_); !sel_res)
        {
            // do nothing
            //return sel_res.error();
        }
        return value_->get_default();
    }
    outcome::result<double> get_value() const final
    {
        if (auto sel_res = selector_->set_value_str(selector_entry_); !sel_res)
        {
            return sel_res.error();
        }
        auto res = value_->get_value();
        if (!res)
        {
            return res.error();
        }
        return res.value();
    }
    outcome::result<void> set_value(double new_value) final
    {
        if (auto sel_res = selector_->set_value_str(selector_entry_); !sel_res)
        {
            return sel_res.error();
        }
        return value_->set_value(new_value);
    }

private:
    std::shared_ptr<IPropertyEnum> selector_;
    std::shared_ptr<IPropertyFloat> value_;
    std::string selector_entry_;

    const tcamprop1::prop_static_info_float* prop_entry_ = nullptr;
};

} // namespace tcam::aravis

VISIBILITY_POP
