#pragma once

#include "memoria_common.hpp"

#include <vector>
#include <string_view>
#include <optional>

MEMORIA_BEGIN

class CSignature
{
public:
	std::vector<uint8_t> _payload;
	std::vector<uint8_t> _mask;
	bool _has_optionals;

public:
	CSignature() = delete;
	CSignature(const std::string_view &str);
	CSignature(const void *data, size_t size, std::optional<uint8_t> ignore_byte = std::nullopt);

	const std::vector<uint8_t> &GetPayload() const { return _payload; }
	const std::vector<uint8_t> &GetMask() const { return _mask; }

	bool IsEmpty() const;
	bool HasOptionals() const;

	std::vector<std::optional<uint8_t>> CreatePattern() const;

	bool Match(const void *addr) const;
};

MEMORIA_END