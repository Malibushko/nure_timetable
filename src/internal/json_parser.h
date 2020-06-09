#pragma once
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <QtDebug>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <boost/pfr.hpp>

#include "traits.h"

namespace timetable::internal {

using jsonAllocator = rapidjson::Document::AllocatorType;
using jsonArray = decltype(std::declval<rapidjson::Document>().GetArray());
/**
 * @brief Class for parsing json
 * Main idea of json parsing is described in telegram_bot.h
 * There will only be described implementation details
 */
class JsonParser {
    JsonParser() noexcept {
    }
public:
    static JsonParser& i() {
        static JsonParser instance;
        return instance;
    }
    JsonParser(JsonParser&&) = delete;
    JsonParser(const JsonParser&) = delete;
    /**
     * @brief Serialize value to JSON format
     * \return std::string containing valid JSON
     * @param item - Structure or class containing 'declare_struct' macro
     */
    template <class T, typename = std::enable_if_t<traits::is_parsable_v<T>>>
    std::string toJson(const T &item) const {
        static_assert(boost::pfr::tuple_size_v<std::decay_t<T>>> 0,
                "The structure has no fields.");

        rapidjson::Value object(rapidjson::kObjectType);
        rapidjson::Document document;
        document.SetObject();

        launchDeserialize(item, document,
                          std::make_index_sequence<boost::pfr::tuple_size_v<T>>{});
        return rapidObjectToJson(document);
    }

    /**
     * @brief Deserialize value from JSON format
     * \return object of class T
     * @param data - valid JSON string containing data named as T fields
     */
    template <class T, typename = std::enable_if_t<traits::is_container_v<T> ||
                                                   traits::is_parsable_v<T>>>
    T fromJson(const std::string &data) const  {
        rapidjson::Document doc;
        rapidjson::Value val;
        rapidjson::ParseResult ok = doc.Parse(data.data());
        if (!ok) {
            //   utility::Logger::warn("Invalid json;");
            return T{};
        }
        T item{};
        // if value is array
        if constexpr (traits::is_container_v<T> && !traits::is_string_type<T>) {
            auto temp_arr = doc.GetArray();
            arrayToJson(item, temp_arr, doc.GetAllocator());
        } else if constexpr (traits::is_parsable_v<T>) {
            static_assert(boost::pfr::tuple_size_v<T>> 0, "The struct has no fields");
            launchParser(item, doc,
                         std::make_index_sequence<boost::pfr::tuple_size_v<T>>{});
        }
        return item;
    }
    /**
     * @brief Serialize rapidjson value to JSON format and write it to object
     * @param object of type T that must be filled from json
     * @param val - rapidjson Value containing data
     * @paragraph allocator - rapidjson Allocator
     */
    template <class T>
    void valueToJson(const T &value, rapidjson::Value &val,
                     jsonAllocator &allocator) const {
        using field_type = typename std::decay_t<decltype(value)>;
        using type = traits::optional_or_value<field_type>; // get real value type

        // optional case
        if constexpr (traits::is_optional_v<field_type>) {
            if (value) {
                // call recursive with value
                valueToJson(value.value(), val, allocator);
                return;
            } else {
                // return empty value
                val = rapidjson::Value(rapidjson::kNullType);
                val.SetNull();
                return;
            }
        }
        else if constexpr (traits::is_variant_v<type>) {
            std::visit(
                        [&](auto &&inner_val) {
                // get real value from std::variant
                valueToJson(inner_val, val, allocator);
            },
            value);
        }
        // string case
        else if constexpr (std::is_same_v<QString, type>) {
            val = rapidjson::Value(rapidjson::kStringType);
            std::string std_value = value.toStdString();
            val.SetString(std_value.data(),std_value.size(),allocator);
        }
        else if constexpr (traits::is_string_type<type>) {
            val = rapidjson::Value(rapidjson::kStringType);
            val.SetString(value.data(), value.size(), allocator);
        }
        // bool case
        else if constexpr (std::is_same_v<type, bool>) {
            std::optional<type> underlying_value;
            if constexpr (traits::is_optional_v<field_type>) {
                underlying_value = value.value_or(type{});
            } else {
                underlying_value = value;
            }
            if (underlying_value.value())
                val = rapidjson::Value(rapidjson::kTrueType);
            else
                val = rapidjson::Value(rapidjson::kFalseType);

            val.SetBool(underlying_value.value());
        }
        // float case
        else if constexpr (std::is_floating_point_v<type>) {
            val = rapidjson::Value(rapidjson::kNumberType);
            val.SetFloat(value);
        }
        // integer case
        else if constexpr (std::is_arithmetic_v<type>) {
            val = rapidjson::Value(rapidjson::kNumberType);
            val.SetInt64(value);
        }
        // value is a struct
        else if constexpr (traits::is_parsable_v<type>) {
            val = rapidjson::Value(rapidjson::kObjectType);
            val.SetObject();
            std::string parsedField = toJson(value);
            // parse all fields using rapidjson and write them to our struct
            rapidjson::Document subdoc(&allocator);
            subdoc.Parse(parsedField.data());

            auto docObj = subdoc.GetObject();
            for (auto it = docObj.begin(); it != docObj.end(); ++it) {
                val.AddMember(it->name, it->value, subdoc.GetAllocator());
            }
        }
        else if constexpr (traits::is_container_v<type>) {
            arrayToJson(value, val, allocator);
        }
    }

    /**
     * @brief Serialize rapidjson value to JSON string
     * @param val - rapidjson Value containing data
     * \return std::string containing valid JSON
     */
    std::string rapidObjectToJson(const rapidjson::Value& val) const {
        rapidjson::StringBuffer buff;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buff);
        val.Accept(writer);
        return buff.GetString();
    }
    /**
     * @brief Serialize rapidjson array to JSON string
     * @param val - rapidjson Value containing Array object
     * \return std::string containing valid JSON
     */
    std::string rapidArrayToJson(rapidjson::Value val) const {
        rapidjson::StringBuffer buff;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buff);
        rapidjson::Document doc;
        doc.SetArray();

        for (auto it = val.Begin();it != val.End();++it) {
            doc.PushBack(*it,doc.GetAllocator());
        }
        doc.Accept(writer);
        return buff.GetString();
    }
    /**
     * @brief Serialize rapidjson Document to JSON string
     * @param val - rapidjson Document containing data
     * \return std::string containing valid JSON
     */
    std::string rapidDocumentToString(const rapidjson::Document& doc) const {
        rapidjson::StringBuffer buff;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buff);
        doc.Accept(writer);
        return buff.GetString();
    }
private:
    // helper functions

    // ------------------------- SERIALIZE ----------------------------
    template <class T>
    void arrayToJson(const T &arr, rapidjson::Value &val,
                     jsonAllocator &allocator) const {
        using type = traits::optional_or_value<std::decay_t<T>>;
        if (val.IsNull()) // check if value has not been set before
            val = rapidjson::Value(rapidjson::kArrayType);
        // string case
        if constexpr (traits::is_string_type<typename type::value_type>) {
            for (auto &&it : arr) {
                rapidjson::Value strVal(rapidjson::kStringType);
                strVal.SetString(it.data(), it.size(), allocator);
                val.PushBack(strVal, allocator);
            }
        }
        // float case
        else if constexpr (std::is_floating_point_v<typename type::value_type>) {
            for (auto &&it : arr) {
                rapidjson::Value numVal(rapidjson::kNumberType);
                numVal.SetFloat(it);
                val.PushBack(numVal, allocator);
            }
        }
        // boolean case
        else if constexpr (std::is_same_v<typename type::value_type, bool>) {
            for (auto &&it : arr) {
                rapidjson::Value numVal(it ? rapidjson::kTrueType
                                           : rapidjson::kFalseType);
                numVal.SetBool(it);
                val.PushBack(numVal, allocator);
            }
        }
        // integer case
        else if constexpr (std::is_arithmetic_v<typename type::value_type>) {
            for (auto &&it : arr) {
                rapidjson::Value numVal(rapidjson::kNumberType);
                numVal.SetInt64(it);
                val.PushBack(numVal, allocator);
            }
        }
        // struct contain some other struct
        else if constexpr (traits::is_parsable_v<typename type::value_type>) {
            for (auto &&it : arr) {
                const auto &json = toJson(it);
                rapidjson::Document subDoc(&allocator);
                subDoc.Parse(json.data());
                val.PushBack(subDoc, allocator);
            }
        }
        // struct contains array of values
        else if constexpr (traits::is_container_v<typename type::value_type>) {
            for (auto &&it : arr) {
                rapidjson::Value buffArray(rapidjson::kArrayType);
                // recursive call
                arrayToJson(it, buffArray, allocator);
                val.PushBack(buffArray.Move(), allocator);
            }
        }
    }
    /**
     * function builds JSON document
     */
    template <size_t N, class MetaStruct, typename = std::enable_if_t<traits::is_parsable_v<MetaStruct>>>
    void appendToDocument(rapidjson::Document &doc, jsonAllocator &allocator,
                          const MetaStruct &str) const {
        // set init value to Null
        rapidjson::Value val(rapidjson::kNullType);
        // check if value is unique_ptr (support for other smart pointer will be added
        // later)
        if constexpr (traits::is_unique_ptr_v<traits::remove_cvref_t<decltype(
                    boost::pfr::get<N>(str))>>) {
            if (boost::pfr::get<N>(str))
                valueToJson(*boost::pfr::get<N>(str), val, allocator);
            else
                return;
        } else {
            valueToJson(boost::pfr::get<N>(str), val, allocator);
        }
        // null values are not being written to doc
        if (val.GetType() != rapidjson::kNullType) {
            rapidjson::Value name(rapidjson::kStringType);
            name.SetString(
                        std::decay_t<MetaStruct>::template field_info<N>::name.data(),
                        std::decay_t<MetaStruct>::template field_info<N>::name.length(),
                        allocator);

            doc.AddMember(name.Move(), val, allocator);
        }
    }
    template <class T, size_t... Indexes>
    void launchDeserialize(const T &item, rapidjson::Document &doc,
                           const std::index_sequence<Indexes...> &) const {
        (appendToDocument<Indexes>(doc, doc.GetAllocator(), item), ...);
    }

    // ---------------------- DESERIALIZE ----------------------------


    template <class T>
    void arrayToJson(T &array, const jsonArray &arr, jsonAllocator &allocator) const {
        using type =
        traits::optional_or_value<std::remove_reference_t<std::remove_cv_t<T>>>;
        if constexpr (traits::is_optional_v<T>)
                array = type{}; // if value is optional set it to default

        // bool case
        if constexpr (std::is_same_v<bool, typename type::value_type>) {
            for (auto &&it : arr) {
                if constexpr (traits::is_optional_v<T>)
                        array.value().push_back(it.GetBool());
                else
                array.push_back(it.GetBool());
            }
            // float case
        } else if constexpr (std::is_floating_point<
                typename type::value_type>::value) {
            for (auto &&it : arr) {
                if constexpr (traits::is_optional_v<T>)
                        array.value().push_back(it.GetFloat());
                else
                array.push_back(it.GetFloat());
            }
            // any integer case (except for boolean and float)
        } else if constexpr (std::is_arithmetic<typename type::value_type>::value) {
            for (auto &&it : arr) {
                if constexpr (traits::is_optional_v<T>)
                        array.value().push_back(it.GetInt64());
                else
                array.push_back(it.GetInt64());
            }
            // string type
        } else if constexpr (traits::is_string_type<typename type::value_type>) {
            for (auto &&it : arr) {
                if constexpr (traits::is_optional_v<T>)
                        array.value().push_back(it.GetString());
                else
                array.push_back(it.GetString());
            }
            // another structure
        } else if constexpr (traits::is_parsable_v<typename type::value_type>) {
            for (auto &&it : arr) {
                rapidjson::Document doc(&allocator);
                doc.SetObject();
                const auto &obj = it.GetObject();

                for (auto &&fieldIter : obj) {
                    doc.AddMember(fieldIter.name.Move(), fieldIter.value.Move(),
                                  allocator);
                }
                if constexpr (traits::is_optional_v<T>) {
                    array.value().push_back(
                                fromJson<typename type::value_type>(JsonParser::i().rapidObjectToJson(doc)));
                } else {
                    array.push_back(
                                fromJson<typename type::value_type>(JsonParser::i().rapidObjectToJson(doc)));
                }
            }
            // recursive case
        } else if constexpr (traits::is_container_v<typename type::value_type>) {
            using val_type = typename type::value_type;
            val_type nestedArray;
            for (auto it = arr.begin(); it != arr.end(); ++it) {
                if (it->IsArray()) {
                    auto val = it->GetArray();
                    arrayToJson(nestedArray, val, allocator);
                }
            }
            if constexpr (traits::is_optional_v<T>)
                    array.value().push_back(std::move(nestedArray));
            else
            array.push_back(std::move(nestedArray));
        }
    }

    template <class T>
    void parseFieldImpl(const char *field_name, T &field,
                        rapidjson::Document &doc) const {
        using field_type = traits::optional_or_value<std::decay_t<decltype(field)>>;

        if (!doc.HasMember(field_name)) {

            // if not member in document return default construct
            if constexpr (traits::is_string_type<field_type>) {
                field = "";
            }
            else
            field = {};
            return;
        }
        auto &val = doc[field_name];

        // algo the same as 'to_json' and 'from_json'
        if constexpr (traits::is_string_type<field_type>)
                field = val.Move().GetString();
        else if constexpr (std::is_same_v<bool, field_type>)
                field = val.GetBool();
        else if constexpr (std::is_floating_point_v<field_type>)
                field = val.GetFloat();
        else if constexpr (std::is_integral_v<field_type>)
                field = val.GetInt64();
        else if constexpr (traits::is_parsable_v<field_type>) {
            rapidjson::Document subdoc(&doc.GetAllocator());
            subdoc.SetObject();
            for (auto it = val.GetObject().begin(); it != val.GetObject().end(); ++it) {
                subdoc.AddMember(it->name.Move(), it->value.Move(),
                                 subdoc.GetAllocator());
            }
            field = fromJson<field_type>(JsonParser::i().rapidObjectToJson(subdoc));
        } else if constexpr (traits::is_container_v<field_type>) {
            auto arr = val.GetArray();
            arrayToJson(field, arr, doc.GetAllocator());
        }
    }
    template <typename T, size_t N> void parseField(T &s, rapidjson::Document &doc) const {
        if (doc.HasMember(T::template field_info<N>::name.data())) {
            using type = traits::optional_or_value<
            std::decay_t<decltype(boost::pfr::get<N>(s))>>;

            if constexpr (traits::is_unique_ptr_v<type>) {
                boost::pfr::get<N>(s) = std::make_unique<typename type::element_type>();
                parseFieldImpl(T::template field_info<N>::name.data(),
                               *boost::pfr::get<N>(s), doc);
            } else
            parseFieldImpl(T::template field_info<N>::name.data(),
                           boost::pfr::get<N>(s), doc);
        }
    }

    template <class T, size_t... Indexes>
    void launchParser(T &s, rapidjson::Document &doc,
                      const std::index_sequence<Indexes...> &) const {
        (parseField<T, Indexes>(s, doc), ...);
    }

};
}
