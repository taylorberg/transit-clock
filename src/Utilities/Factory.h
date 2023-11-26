/*
 * TransitClock
 * Copyright (C) 2023-2024 Taylor Berg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRANSIT_CLOCK_FACTORY_H
#define TRANSIT_CLOCK_FACTORY_H

#include <string>
#include <map>
#include <stdexcept>
#include <mutex>
#include <memory>
#include <source_location>
#include <iostream>

template<typename BaseType>
class Factory
{
public:
    class RegistrationBase
    {
    protected:
        explicit RegistrationBase(const std::string & type_name) :
                mName(type_name)
        {
            Factory<BaseType>::instance()->register_type(this);
        }

    public:
        virtual ~RegistrationBase()
        {
            Factory<BaseType>::instance()->unregister_type(this);
        }

        virtual BaseType * construct() const = 0;

        const std::string & name() const { return mName; }

    private:
        const std::string mName;
    };

    template<typename SpecializedType>
    class Registration : public RegistrationBase
    {
    public:
        explicit Registration(const std::string & type_name) :
                RegistrationBase(type_name) { }

        virtual ~Registration() = default;

        virtual BaseType * construct() const
        {
            return new SpecializedType();
        }
    };

private:
    Factory() = default;

public:
    virtual ~Factory() = default;

    static Factory<BaseType> * instance()
    {
        if (mInstance == nullptr)
        {
            mInstance = new Factory<BaseType>();
        }

        return mInstance;
    }

    void register_type(const RegistrationBase * registration)
    {
        if (registration == nullptr)
        {
            throw std::domain_error("registration was null!");
        }

        mRegistrations.emplace(registration->name(), registration);
    }

    void unregister_type(const RegistrationBase * registration)
    {
        if (registration == nullptr)
        {
            throw std::domain_error("registration was null!");
        }

        mRegistrations.erase(registration->name());
    }

    BaseType * construct(const std::string & object_name)
    {
        if (!mRegistrations.contains(object_name))
        {
            throw std::runtime_error(std::string(std::source_location::current().function_name())
                + ": Attempted to construct an invalid object: " + object_name);
        }
        return mRegistrations.at(object_name)->construct();
    }

private:
    std::map<std::string, const RegistrationBase *> mRegistrations;
    static Factory<BaseType> * mInstance;
};

template<typename T>
Factory<T> * Factory<T>::mInstance = nullptr;


#endif //TRANSIT_CLOCK_FACTORY_H
